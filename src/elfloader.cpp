#include "elfloader.hpp"
#include "hhk.h"
#include "util.hpp"
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <elf.h>
#include <utility>

std::map<uint8_t *, std::pair<uint8_t *, std::size_t>> *file_loads;

void nop() {
  std::printf("main exec\n");
  Debug_WaitKey();
}

void *load_elf(uint8_t *file_memory, std::size_t length) {
  std::printf("Checking if ELF file... ");
  if (length < SELFMAG)
    error("EOF");
  if (std::memcmp(file_memory, ELFMAG, SELFMAG) != 0)
    error("no");
  std::printf("yes\n");

  std::printf("Checking SYSV OS ABI... ");
  if (length < EI_OSABI)
    error("EOF");
  if (file_memory[EI_OSABI] != ELFOSABI_SYSV)
    error("no");
  std::printf("yes\n");

  std::printf("Checking bitness... ");
  switch (file_memory[EI_CLASS]) {
  case ELFCLASS32:
    std::printf("32\n");
    break;
  case ELFCLASS64:
    error("E: 64\n");
  default:
    error("unknown");
  }

  std::printf("Overlaying ELF header... ");
  if (length < sizeof(Elf32_Ehdr))
    error("EOF");
  auto ehdr = reinterpret_cast<Elf32_Ehdr *>(file_memory);
  std::printf("Done\n");

  std::printf("Checking ELF type... ");
  if (ehdr->e_type != ET_DYN)
    error("no");
  std::printf("yes\n");

  std::printf("Checking ELF machine... ");
  switch (ehdr->e_machine) {
  case EM_M32:
    error("E: M32\n");
  case EM_SPARC:
    error("E: SPARC\n");
  case EM_386:
    error("E: 386\n");
  case EM_68K:
    error("E: 68K\n");
  case EM_88K:
    error("E: 88K\n");
  case EM_860:
    error("E: 860\n");
  case EM_MIPS:
    error("E: MIPS\n");
  case EM_PARISC:
    error("E: PARISC\n");
  case EM_SPARC32PLUS:
    error("E: SPARC32PLUS\n");
  case EM_PPC:
    error("E: PPC\n");
  case EM_PPC64:
    error("E: PPC64\n");
  case EM_S390:
    error("E: S390\n");
  case EM_ARM:
    error("E: ARM\n");
  case EM_SH:
    std::printf("SH\n");
    break;
  case EM_SPARCV9:
    error("E: SPARCV9\n");
  case EM_IA_64:
    error("E: IA_64\n");
  case EM_X86_64:
    error("E: X86_64\n");
  case EM_VAX:
    error("E: VAX\n");
  default:
    error("unknown");
  }

  std::printf("Checking ELF version... ");
  if (ehdr->e_version != EV_CURRENT)
    error("no");
  std::printf("yes\n");

  std::printf("Checking ELF entry... ");
  if (ehdr->e_entry == 0)
    error("E: 0\n");
  std::printf("%#08" PRIx32 "\n", ehdr->e_entry);

  std::printf("Checking program header offset... ");
  if (ehdr->e_phoff == 0)
    error("E: 0\n");
  std::printf("%#08" PRIx32 "\n", ehdr->e_phoff);

  std::printf("Checking program header size... ");
  if (ehdr->e_phentsize != sizeof(Elf32_Phdr))
    error("E: %" PRIu16 " != %zu\n", ehdr->e_phentsize, sizeof(Elf32_Phdr));
  std::printf("%zu\n", sizeof(Elf32_Phdr));

  std::printf("Checking program header count... ");
  if (ehdr->e_phnum == 0)
    error("E: 0\n");
  std::printf("%" PRIu16 "\n", ehdr->e_phnum);

  if (length < ehdr->e_phnum * sizeof(Elf32_Phdr) + ehdr->e_phoff)
    error("phdrs are over EOF\n");

  file_loads = new std::map<uint8_t *, std::pair<uint8_t *, std::size_t>>();
  dealloc_list->push_back(file_loads);

  {
    decltype(Elf32_Phdr::p_vaddr) load_start = -1;
    decltype(Elf32_Phdr::p_vaddr) load_end = 0;
    decltype(Elf32_Phdr::p_align) alignment = 0;
    for (auto phdr =
             reinterpret_cast<Elf32_Phdr *>(file_memory + ehdr->e_phoff);
         phdr < reinterpret_cast<Elf32_Phdr *>(file_memory + ehdr->e_phoff) +
                    ehdr->e_phnum;
         phdr++) {
      std::printf("Pre-Checking program header type LOAD... ");
      if (phdr->p_type != PT_LOAD) {
        std::printf("no\n");
        continue;
      }
      std::printf("yes\n");

      if (phdr->p_vaddr < load_start)
        load_start = phdr->p_vaddr;
      if (phdr->p_vaddr + phdr->p_memsz > load_end)
        load_end = phdr->p_vaddr + phdr->p_memsz;
      if (phdr->p_align % 2 != 0 &&
          (phdr->p_vaddr - phdr->p_offset) % phdr->p_align != 0)
        error("E: alignment %#08" PRIx32 "\n", phdr->p_align);
      if (phdr->p_align > alignment)
        alignment = phdr->p_align;
    }
    std::printf("Getting memory... ");
    base_address =
        reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(aligned_alloc(
                                     alignment, load_end - load_start)) -
                                 load_start);
    if (!base_address)
      error("Error!\n");
    std::printf("Done\n");
    std::printf("Base address = %p\n", base_address);
  }

  for (auto phdr = reinterpret_cast<Elf32_Phdr *>(file_memory + ehdr->e_phoff);
       phdr < reinterpret_cast<Elf32_Phdr *>(file_memory + ehdr->e_phoff) +
                  ehdr->e_phnum;
       phdr++) {
    std::printf("Checking program header type... ");
    switch (phdr->p_type) {
    case PT_NULL:
      std::printf("NULL\n");
      break;
    case PT_LOAD:
      std::printf("LOAD\n");
      phandler_load(file_memory, length, phdr);
      break;
    case PT_DYNAMIC:
      std::printf("DYNAMIC\n");
      phandler_dynamic(file_memory, length, phdr);
      break;
    case PT_INTERP:
      std::printf("INTERP\n");
      phandler_interp(file_memory, length, phdr);
      break;
    case PT_NOTE:
      std::printf("NOTE\n");
      break;
    case PT_SHLIB:
      std::printf("W: SHLIB\n");
      break;
    case PT_PHDR:
      std::printf("PHDR\n");
      phandler_phdr(ehdr, phdr);
      break;
    default:
      if (phdr->p_type >= PT_LOOS && phdr->p_type <= PT_HIOS)
        std::printf("OS\n");
      else if (phdr->p_type >= PT_LOPROC && phdr->p_type <= PT_HIPROC)
        std::printf("PROC\n");
      else
        error("unknown\n@offset %#08" PRIxPTR "\n",
              reinterpret_cast<std::uintptr_t>(phdr) -
                  reinterpret_cast<std::uintptr_t>(file_memory));
    }
  }

  return lookup(reinterpret_cast<void *>(ehdr->e_entry));
}