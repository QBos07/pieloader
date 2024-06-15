#include "elfloader.hpp"
#include <cstdio>
#include <elf.h>
#include <cstdint>

void phandler_interp(uint8_t *file_memory, std::size_t length, Elf32_Phdr *phdr) {
  std::printf("The interpreter is %s\n", (phdr->p_offset < length) ? reinterpret_cast<char *>(file_memory + phdr->p_offset) : "W: EOF");
}