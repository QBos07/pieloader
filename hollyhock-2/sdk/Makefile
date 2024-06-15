DOXYGEN=doxygen

AS:=sh4aeb-pic-elf-gcc
AS_FLAGS:=-fPIC -ffreestanding -ffunction-sections -fdata-sections -m4a-nofpu -Wall -Wextra -Os -I include/

CC:=sh4aeb-pic-elf-gcc
CC_FLAGS:=-fPIC -ffreestanding -ffunction-sections -fdata-sections -m4a-nofpu -Wall -Wextra -Os -I include/

CXX:=sh4aeb-pic-elf-g++
CXX_FLAGS:=-fPIC -ffreestanding -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -m4a-nofpu -Wall -Wextra -Os -I include/

AR:=sh4aeb-pic-elf-ar
AR_FLAGS:=src

AS_SOURCES:=$(wildcard *.S) $(wildcard **/*.S) $(wildcard **/**/*.S)
CC_SOURCES:=$(wildcard *.c) $(wildcard **/*.c) $(wildcard **/**/*.c)
CXX_SOURCES:=$(wildcard *.cpp) $(wildcard **/*.cpp) $(wildcard **/**/*.cpp)
OBJECTS:=$(CC_SOURCES:.c=.o) $(CXX_SOURCES:.cpp=.o) $(AS_SOURCES:.S=.o)

all: libsdk.a sdk.o Makefile

docs:
	$(DOXYGEN)

clean:
	rm -f $(OBJECTS) libsdk.a sdk.o

clean_docs:
	rm -rf doc/

libsdk.a: $(OBJECTS)

lib%.a:
	$(AR) $(AR_FLAGS) $@ $^

%.o: lib%.a
	ln -s $< $@

%.o: %.S
	$(AS) -c $< -o $@ $(AS_FLAGS) -Wa,-I -Wa,$(patsubst %/,%,$(dir $<))

%.o: %.c
	$(CC) -c $< -o $@ $(CC_FLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

.PHONY: all docs clean clean_docs
