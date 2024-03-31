#include "elfloader.hpp"
#include "hhk.h"
#include <cstdio>
#include <list>
#include <stdlib.h>
#include "util.hpp"

std::list<void *> *dealloc_list;

extern "C"
void cxx_main() {
  dealloc_list = new std::list<void *>();
  atexit([]() {
    for (auto ptr : *dealloc_list)
      free(ptr);
  });

  constexpr auto filename = "\\fls0\\pie.elf";

  std::printf("PIE Loader by QBos07\n");
  std::printf("Opening file %s... ", filename);
  FILE *file = std::fopen(filename, "rb");
  if (!file) error("Error!\n");
  std::printf("Done\n");
  std::printf("Getting length... ");
  std::fseek(file, 0, SEEK_END);
  size_t length = std::ftell(file);
  std::rewind(file);
  std::printf("%zu\n", length);

  std::printf("Getting memory... ");
  auto file_memory = new uint8_t[length];
  dealloc_list->push_back(file_memory);
  if (!file_memory) error("Error!\n");
  std::printf("Done\n");
  std::printf("Copying file... ");
  std::fread(file_memory, 1, length, file);
  std::printf("Done\n");
  std::printf("Closing file... ");
  std::fclose(file);
  std::printf("Done\n");

  auto mainPtr =
      reinterpret_cast<void (*)(void)>(load_elf(file_memory, length));
  std::printf("Press any key to execute... \n");
  std::fflush(stdout);
  Debug_WaitKey();
  mainPtr();
}