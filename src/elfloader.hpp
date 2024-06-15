#pragma once
#include <cstdint>
#include <elf.h>
#include <map>
#include <utility>
#include <vector>

void *load_elf(uint8_t *file_memory, std::size_t length);

void phandler_load(uint8_t *file_memory, std::size_t length, Elf32_Phdr *phdr);
void phandler_dynamic(uint8_t *file_memory, std::size_t length, Elf32_Phdr *phdr);
void phandler_interp(uint8_t *file_memory, std::size_t length, Elf32_Phdr *phdr);
void phandler_phdr(Elf32_Ehdr *ehdr, Elf32_Phdr *phdr);

// original virt addr -> loaded + length
extern std::map<uint8_t *, std::pair<uint8_t *, std::size_t>> *file_loads;

extern void *base_address;

void *lookup(void *original);
void *lookup(void *absolute, void *base);