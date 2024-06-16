#include "elfloader.hpp"
#include "phandler_dynamic.hpp"
#include "util.hpp"
#include <cstdint>
#include <cinttypes>
#include <cstdio>
#include <cstring>
#include <elf.h>

void relocate(relocs_t *relocs, uint8_t *file_memory, std::size_t length) {
  if (!relocs->rela)
    return;

  std::size_t symtab_size = 0;
  if (relocs->symtab && relocs->strtab && relocs->strtab_size) {
    std::printf("Calculation SYMTAB size... ");
    if (relocs->strtab_size + reinterpret_cast<std::uintptr_t>(relocs->strtab) -
            reinterpret_cast<std::uintptr_t>(file_memory) <
        length)
      error("EOF");
    for (auto ptr = relocs->strtab; ptr < relocs->strtab + relocs->strtab_size;
         ++ptr) {
      if (*ptr == '\0')
        ++symtab_size;
    }
    std::printf("%zu\n", symtab_size);
  }

  std::printf("Relocating... ");
  if (reinterpret_cast<std::uintptr_t>(relocs->rela + relocs->rela_count) -
          reinterpret_cast<std::uintptr_t>(file_memory) <
      length)
    error("EOF\n");
  std::printf("Running\n");
  for (auto rela = relocs->rela; rela < relocs->rela + relocs->rela_count;
       ++rela) {
    if (reinterpret_cast<std::uintptr_t>(rela) -
            reinterpret_cast<std::uintptr_t>(file_memory) <
        length)
      error("EOF\n");
    auto addr = reinterpret_cast<uint32_t *>(
        lookup(reinterpret_cast<void *>(rela->r_offset)));
    switch (ELF32_R_TYPE(rela->r_info)) {
    case R_SH_RELATIVE:
      *addr = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(
          lookup(reinterpret_cast<void *>(rela->r_addend))));
      std::printf("R_SH_RELATIVE: *%p = %#" PRIx32 "\n", reinterpret_cast<void *>(addr), *addr);
      break;
    case R_SH_DIR32:
      *addr = reinterpret_cast<std::uintptr_t>(lookup(reinterpret_cast<void *>(
                  relocs->symtab[ELF32_M_SYM(rela->r_info)].st_value))) +
              rela->r_addend;
      std::printf("R_SH_DIR32: *%p = %#" PRIx32 "\n", reinterpret_cast<void *>(addr), *addr);
      break;
    case R_SH_NONE:
      break;
    default:
      error("Unknown relocation type: %" PRIi32 "\n", ELF32_R_TYPE(rela->r_info));
    }
  }
}