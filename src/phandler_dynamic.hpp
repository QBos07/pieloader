#include <elf.h>
#include <cstddef>

typedef struct {
  Elf32_Rela *rela;
  size_t rela_count;
  Elf32_Sym *symtab;
  char *strtab;
  size_t strtab_size;
} relocs_t;

void relocate(relocs_t *relocs, uint8_t *file_memory, size_t length);