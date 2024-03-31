#pragma once
#include <cstddef>
#include <cstdint>
#include <elf.h>
#include <map>
#include <utility>

void *load_elf(uint8_t *file_memory, size_t length);

void phandler_load(uint8_t *file_memory, size_t length, Elf32_Phdr *phdr);
void phandler_dynamic(uint8_t *file_memory, size_t length, Elf32_Phdr *phdr);
void phandler_interp(uint8_t *file_memory, size_t length, Elf32_Phdr *phdr);
void phandler_phdr(Elf32_Ehdr *ehdr, Elf32_Phdr *phdr);

// original offset -> loaded + length
extern std::map<uint8_t *, std::pair<uint8_t *, size_t>> *file_loads;
