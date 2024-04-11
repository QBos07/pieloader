#include "elfloader.hpp"
#include <cstdint>

void *lookup(void *original) {
  for (auto map : *file_loads) {
    if (map.first < original && map.first + map.second.second > original)
      return map.second.first + (reinterpret_cast<uintptr_t>(original) - reinterpret_cast<uintptr_t>(map.first));
  }
  return nullptr;
}

void *lookup(void *absolute, void *base) {
  return lookup(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(absolute) -
                                         reinterpret_cast<uintptr_t>(base)));
}