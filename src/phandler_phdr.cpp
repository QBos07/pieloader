#include "elfloader.hpp"
#include "util.hpp"
#include <elf.h>
#include <cstdint>
#include <cinttypes>
#include <cstdio>

void phandler_phdr(Elf32_Ehdr *ehdr, Elf32_Phdr *phdr) {
  std::printf("Checking program header offset... ");
  if (phdr->p_offset != ehdr->e_phoff)
    error("E: %#08" PRIx32 "\n", phdr->p_offset);
  std::printf("%#08" PRIx32 "\n", ehdr->e_phoff);

  std::printf("Checking program header size... ");
  if (phdr->p_filesz != sizeof(Elf32_Phdr) * ehdr->e_phnum)
    error("E: %" PRIu32 " != %zu\n", phdr->p_filesz, sizeof(Elf32_Ehdr) * ehdr->e_phnum);
  std::printf("%zu\n", ehdr->e_phnum * sizeof(Elf32_Phdr));
}