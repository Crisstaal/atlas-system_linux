
#ifndef H_READ_ELF_H
#define H_READ_ELF_H

#include <libelf.h>
#include <gelf.h>


int open_file(const char *filename);
int check_elf(Elf *elf);
void print_header(const GElf_Ehdr *ehdr);
void print_elf_header(const char *filename);

#endif
