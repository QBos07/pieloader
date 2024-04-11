#include "elfloader.hpp"
#include "util.hpp"
#include <cstdint>
#include <cstdio>
#include <elf.h>
#include <stdlib.h>
#include <cstring>
#include <utility>

// freestanding aligned malloc
void *aligned_malloc(size_t size, size_t alignment) {
  if (__builtin_popcount(alignment) > 1) {
    alignment = 1 << (sizeof(alignment) - __builtin_clz(alignment));
  }
  void *ptr = malloc(size + alignment);
  if (!ptr) return nullptr;
  dealloc_list->push_back(ptr);
  if (alignment < 2) return ptr;
  void *aligned = reinterpret_cast<void *>(
      (reinterpret_cast<uintptr_t>(ptr) + alignment) & ~(alignment - 1));
  return aligned;
}

void phandler_load(uint8_t *file_memory, size_t length, Elf32_Phdr *phdr) {
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

  std::printf("Checking alignment... %lu\n",
              static_cast<unsigned long>(phdr->p_align));

  std::printf("Getting memory... ");
  auto memory = reinterpret_cast<uint8_t *>(aligned_malloc(phdr->p_memsz, phdr->p_align));
  if (!memory) error("Error!\n");
  std::printf("Done\n");

  std::printf("Copying data... ");
  if (phdr->p_offset + phdr->p_filesz > length)
   error("EOF");
  std::memcpy(memory, file_memory + phdr->p_offset, phdr->p_filesz);
  std::memset(memory + phdr->p_filesz, 0, phdr->p_memsz - phdr->p_filesz);
  std::printf("Done\n");

  std::printf("Loaded %p (%p) on %p - %p\n", file_memory - phdr->p_offset, reinterpret_cast<void *>(phdr->p_vaddr), memory, memory + phdr->p_memsz);
  (*file_loads)[reinterpret_cast<uint8_t *>(phdr->p_vaddr)] = std::make_pair(memory, phdr->p_memsz);
}