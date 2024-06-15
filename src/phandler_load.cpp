#include "elfloader.hpp"
#include "util.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <elf.h>
#include <utility>

void phandler_load(uint8_t *file_memory, std::size_t length, Elf32_Phdr *phdr) {
  std::printf("Checking memsize >= filesize... ");
  if (phdr->p_filesz > phdr->p_memsz)
    error("no");
  std::printf("yes\n");

  std::printf("Checking memsize > 0... ");
  if (phdr->p_memsz == 0) {
    std::printf("no\n");
    return;
  }
  std::printf("yes\n");

  std::printf("Copying data... ");
  if (phdr->p_offset + phdr->p_filesz > length)
    error("EOF");
  // const auto memory = lookup(reinterpret_cast<void *>(phdr->p_vaddr));
  const auto memory = reinterpret_cast<void *>(
      phdr->p_vaddr + reinterpret_cast<std::uintptr_t>(base_address));
  std::memcpy(memory, file_memory + phdr->p_offset, phdr->p_filesz);
  std::memset(reinterpret_cast<void *>(
                  reinterpret_cast<std::uintptr_t>(memory) + phdr->p_filesz),
              0, phdr->p_memsz - phdr->p_filesz);
  std::printf("%i ", std::memcmp(memory, file_memory + phdr->p_offset, phdr->p_filesz));
  std::printf("Done\n");

  std::printf("Loaded %p (%p) on %p - %p\n", file_memory - phdr->p_offset,
              reinterpret_cast<void *>(phdr->p_vaddr), memory,
              reinterpret_cast<void *>(
                  reinterpret_cast<std::uintptr_t>(memory) + phdr->p_memsz));
  (*file_loads)[reinterpret_cast<uint8_t *>(phdr->p_vaddr)] =
      std::make_pair(reinterpret_cast<uint8_t *>(memory), phdr->p_memsz);
}