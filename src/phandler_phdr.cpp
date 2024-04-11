#include "elfloader.hpp"
#include "util.hpp"
#include <elf.h>
#include <cstdint>

void phandler_phdr(Elf32_Ehdr *ehdr, Elf32_Phdr *phdr) {
  std::printf("Checking program header offset... ");
  if (phdr->p_offset != ehdr->e_phoff)
    error("E: %#08lx\n", static_cast<unsigned long>(phdr->p_offset));
  std::printf("%#08lx\n", static_cast<unsigned long>(ehdr->e_phoff));

  std::printf("Checking program header size... ");
  if (phdr->p_filesz != sizeof(Elf32_Phdr) * ehdr->e_phnum)
    error("E: %lu != %lu\n", static_cast<size_t>(phdr->p_filesz), sizeof(Elf32_Ehdr) * ehdr->e_phnum);
  std::printf("%lu\n", ehdr->e_phnum * sizeof(Elf32_Phdr));
}