#include <elf.h>
#include <cstdint>

typedef struct {
  Elf32_Rela *rela;
  std::size_t rela_count;
  Elf32_Sym *symtab;
  char *strtab;
  std::size_t strtab_size;
} relocs_t;

void relocate(relocs_t *relocs, uint8_t *file_memory, std::size_t length);