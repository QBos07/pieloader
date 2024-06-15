#include "../util.hpp"
#include <cstdint>
#include <cstdlib>

// freestanding aligned malloc
extern "C" void *aligned_alloc(std::size_t size, std::size_t alignment) {
  if (__builtin_popcount(alignment) > 1) {
    alignment = 1 << (sizeof(alignment) - __builtin_clz(alignment));
  }
  void *ptr = std::malloc(size + alignment);
  if (!ptr) return nullptr;
  dealloc_list->push_back(ptr);
  if (alignment < 2) return ptr;
  void *aligned = reinterpret_cast<void *>(
      (reinterpret_cast<std::uintptr_t>(ptr) + alignment) & ~(alignment - 1));
  return aligned;
}