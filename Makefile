# run `make all` to compile the .hhk and .bin file, use `make` to compile only the .bin file.
# The .hhk file is the original format, the bin file is a newer format.
APP_NAME:=STDLibTest

ifndef SDK_DIR
$(error You need to define the SDK_DIR environment variable, and point it to the sdk/ folder)
endif

AS:=sh4aeb-elf-gcc
AS_FLAGS:=-DAPPNAME_STRING=\"$(APP_NAME)\"

COMMON_FLAGS:=-flto -ffunction-sections -fdata-sections -O2 -m4a-nofpu -DAPPNAME_STRING=\"$(APP_NAME)\"
INCLUDES:=-I $(SDK_DIR)/include/
WARNINGS:=-Wall -Wextra

CC:=sh4aeb-elf-gcc
CC_FLAGS:=$(COMMON_FLAGS) $(INCLUDES) $(WARNINGS)

CXX:=sh4aeb-elf-g++
CXX_FLAGS:=-fno-exceptions -fno-rtti -Wno-write-strings $(COMMON_FLAGS) $(INCLUDES) $(WARNINGS)

LD:=sh4aeb-elf-g++
LD_FLAGS:=-nostartfiles -m4a-nofpu -Wl,--gc-sections -Wno-undef

READELF:=sh4aeb-elf-readelf
OBJCOPY:=sh4aeb-elf-objcopy

SOURCEDIR = src
BUILDDIR = obj
OUTDIR = dist

AS_SOURCES:=$(shell find $(SOURCEDIR) -name '*.S')
CC_SOURCES:=$(shell find $(SOURCEDIR) -name '*.c')
CXX_SOURCES:=$(shell find $(SOURCEDIR) -name '*.cpp')
OBJECTS := $(addprefix $(BUILDDIR)/,$(AS_SOURCES:.S=.o)) \
	$(addprefix $(BUILDDIR)/,$(CC_SOURCES:.c=.o)) \
	$(addprefix $(BUILDDIR)/,$(CXX_SOURCES:.cpp=.o))

APP_ELF:=$(OUTDIR)/$(APP_NAME).elf
APP_BIN:=$(OUTDIR)/$(APP_NAME).bin

bin: $(APP_BIN) Makefile

hhk: $(APP_ELF) Makefile

all: $(APP_ELF) $(APP_BIN) Makefile

clean:
	rm -rf $(BUILDDIR) $(OUTDIR)

$(APP_BIN): $(APP_ELF)
	$(OBJCOPY) --output-target=binary $(APP_ELF) $@

$(APP_ELF): $(OBJECTS) $(SDK_DIR)/sdk.o linker.ld
	mkdir -p $(dir $@)
	$(LD) -T linker.ld -Wl,-Map $@.map -o $@ $(LD_FLAGS) $(OBJECTS)

# We're not actually building sdk.o, just telling the user they need to do it
# themselves. Just using the target to trigger an error when the file is
# required but does not exist.
$(SDK_DIR)/libsdk.a:
	$(error You need to build the SDK before using it. Run make in the SDK directory, and check the README.md in the SDK directory for more information)

$(BUILDDIR)/%.o: %.S
	mkdir -p $(dir $@)
	$(AS) -c $< -o $@ $(AS_FLAGS)

$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CC_FLAGS)

# Break the build if global constructors are present:
# Read the sections from the object file (with readelf -S) and look for any
# called .ctors - if they exist, give the user an error message, delete the
# object file (so that on subsequent runs of make the build will still fail)
# and exit with an error code to halt the build.
$(BUILDDIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
	@$(READELF) $@ -S | grep ".ctors" > /dev/null && echo "ERROR: Global constructors aren't supported." && rm $@ && exit 1 || exit 0

.PHONY: bin hhk all clean