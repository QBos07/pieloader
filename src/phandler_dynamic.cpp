#include "phandler_dynamic.hpp"
#include "elfloader.hpp"
#include "util.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <elf.h>
#include <sys/_stdint.h>

void phandler_dynamic(uint8_t *file_memory, std::size_t length, Elf32_Phdr *phdr) {
  std::printf("Overlaying dynamic section... ");
  if (phdr->p_filesz > length)
    error("EOF");
  std::printf("Running\n");

  auto relocs = reinterpret_cast<relocs_t *>(std::calloc(1, sizeof(relocs_t)));
  dealloc_list->push_back(relocs);

  for (auto dyn = reinterpret_cast<Elf32_Dyn *>(file_memory + phdr->p_offset);
       dyn->d_tag != DT_NULL; ++dyn) {
    if (reinterpret_cast<std::uintptr_t>(dyn) -
            reinterpret_cast<std::uintptr_t>(file_memory) >
        length)
      error("EOF");
    // if (reinterpret_cast<std::uintptr_t>(dyn) -
    // reinterpret_cast<std::uintptr_t>(file_memory) > phdr->p_filesz)
    //   error("Overrun");
    switch (dyn->d_tag) {
    case DT_NEEDED:
      error("E: DT_NEEDED: %s\n", file_memory + dyn->d_un.d_val);
    case DT_INIT:
    case DT_FINI:
    case DT_INIT_ARRAY:
    case DT_INIT_ARRAYSZ:
    case DT_FINI_ARRAY:
    case DT_FINI_ARRAYSZ:
    case DT_GNU_HASH:
    case DT_HASH:
    case DT_DEBUG:
    case DT_FLAGS:
    case DT_FLAGS_1:
    case DT_RELACOUNT:
    case DT_PLTGOT:
      break;
    case DT_RELA:
      if (relocs->rela)
        error("Multiple DT_RELA\n");
      relocs->rela =
          reinterpret_cast<Elf32_Rela *>(dyn->d_un.d_ptr);
      std::printf("DT_RELA: %#08lx\n",
                  reinterpret_cast<unsigned long>(relocs->rela));
      break;
    case DT_RELASZ:
      if (relocs->rela_count)
        error("Multiple DT_RELASZ\n");
      relocs->rela_count = dyn->d_un.d_val / sizeof(Elf32_Rela);
      std::printf("DT_RELASZ: %lu\n",
                  static_cast<unsigned long>(dyn->d_un.d_val));
      break;
    case DT_RELAENT:
      std::printf("DT_RELAENT: ");
      if (dyn->d_un.d_val != sizeof(Elf32_Rela))
        error("E: %lu\n", static_cast<unsigned long>(dyn->d_un.d_val));
      std::printf("%lu\n", sizeof(Elf32_Rela));
      break;
    case DT_SYMTAB:
      if (relocs->symtab)
        error("Multiple DT_SYMTAB\n");
      relocs->symtab =
          reinterpret_cast<Elf32_Sym *>(dyn->d_un.d_ptr);
      std::printf("DT_SYMTAB: %#08lx\n",
                  reinterpret_cast<unsigned long>(relocs->symtab));
      break;
    case DT_SYMENT:
      std::printf("DT_SYMENT: ");
      if (dyn->d_un.d_val != sizeof(Elf32_Sym))
        error("E: %lu\n", static_cast<unsigned long>(dyn->d_un.d_val));
      std::printf("%lu\n", sizeof(Elf32_Sym));
      break;
    case DT_STRTAB:
      if (relocs->strtab)
        error("Multiple DT_STRTAB\n");
      relocs->strtab = reinterpret_cast<char *>(dyn->d_un.d_ptr);
      std::printf("DT_STRTAB: %#08lx\n",
                  reinterpret_cast<unsigned long>(relocs->strtab));
      break;
    case DT_STRSZ:
      if (relocs->strtab_size)
        error("Multiple DT_STRSZ\n");
      relocs->strtab_size = dyn->d_un.d_val;
      std::printf("DT_STRSZ: %lu\n",
                  static_cast<unsigned long>(dyn->d_un.d_val));
      break;
    default:
      error("Unknown dynamic tag: %ld\n", static_cast<long>(dyn->d_tag));
    }
  }

  relocate(relocs, file_memory, length);

  std::printf("Done!\n");

  dealloc_list->remove(relocs);
  std::free(relocs);
}