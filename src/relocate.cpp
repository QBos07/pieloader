#include "elfloader.hpp"
#include "phandler_dynamic.hpp"
#include "util.hpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <elf.h>
#include <type_traits>

void relocate(relocs_t *relocs, uint8_t *file_memory, std::size_t length) {
  if (!relocs->rela)
    return;

  relocs->rela = reinterpret_cast<decltype(relocs->rela)>(lookup(relocs->rela));
  relocs->strtab =
      reinterpret_cast<decltype(relocs->strtab)>(lookup(relocs->strtab));
  relocs->symtab =
      reinterpret_cast<decltype(relocs->symtab)>(lookup(relocs->symtab));

  std::size_t symtab_size = 0;
  if (relocs->symtab && relocs->strtab && relocs->strtab_size) {
    std::printf("Calculation SYMTAB size... ");
    /*if (relocs->strtab_size + reinterpret_cast<std::uintptr_t>(relocs->strtab) -
            reinterpret_cast<std::uintptr_t>(file_memory) >
        length)
      error("EOF");*/ // TODO: Make this work again
    for (auto ptr = relocs->strtab; ptr < relocs->strtab + relocs->strtab_size;
         ++ptr) {
      if (*ptr == '\0')
        ++symtab_size;
    }
    std::printf("%lu\n", symtab_size);
  }

  std::printf("Relocating... ");
  /*if (reinterpret_cast<std::uintptr_t>(relocs->rela + relocs->rela_count) -
          reinterpret_cast<std::uintptr_t>(file_memory) >
      length)
    error("EOF\n");*/ // TODO: Make this work again
  std::printf("Running\n");
  for (auto rela = relocs->rela; rela < relocs->rela + relocs->rela_count;
       ++rela) {
    /*if (reinterpret_cast<std::uintptr_t>(rela) -
            reinterpret_cast<std::uintptr_t>(file_memory) >
        length)
      error("EOF\n");*/ // TODO: Make this work again
    auto addr = reinterpret_cast<uint32_t *>(
        lookup(reinterpret_cast<void *>(rela->r_offset)));
    switch (ELF32_R_TYPE(rela->r_info)) {
    case R_SH_RELATIVE:
      *addr = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(
          lookup(reinterpret_cast<void *>(rela->r_addend))));
      std::printf("R_SH_RELATIVE: *%#08lx = %#08lx\n",
                  reinterpret_cast<unsigned long>(addr),
                  static_cast<unsigned long>(*addr));
      break;
    case R_SH_DIR32:
      *addr = reinterpret_cast<std::uintptr_t>(lookup(reinterpret_cast<void *>(
                  relocs->symtab[ELF32_M_SYM(rela->r_info)].st_value))) +
              rela->r_addend;
      std::printf("R_SH_DIR32: *%#08lx = %#08lx\n",
                  reinterpret_cast<unsigned long>(addr),
                  static_cast<unsigned long>(*addr));
      break;
    case R_SH_NONE:
      break;
    default:
      error("Unknown relocation type: %ld\n",
            static_cast<unsigned long>(ELF32_R_TYPE(rela->r_info)));
    }
  }
}