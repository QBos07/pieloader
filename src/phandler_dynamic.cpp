#include "elfloader.hpp"
#include "util.hpp"
#include <elf.h>
#include <cstdint>

void phandler_dynamic(uint8_t *file_memory, size_t length, Elf32_Phdr *phdr) {
  std::printf("dynamic stub\n");
}