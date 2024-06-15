#include "elfloader.hpp"
#include <cstdint>

void *base_address;

void *lookup(void *original) {
  for (auto map : *file_loads) {
    if (map.first <= original && map.first + map.second.second > original)
      return map.second.first + (reinterpret_cast<std::uintptr_t>(original) - reinterpret_cast<std::uintptr_t>(map.first));
  }
  return nullptr;
}

/*void *lookup(void *original) {
  return reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(original) + reinterpret_cast<std::uintptr_t>(base_address));
}*/

void *lookup(void *absolute, void *base) {
  return lookup(reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(absolute) -
                                         reinterpret_cast<std::uintptr_t>(base)));
}