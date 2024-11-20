#ifndef _HNM_H_
#define _HNM_H_

#include <elf.h>
#include <stddef.h>
#include <stdint.h>
#include <gelf.h>

/* Function Prototypes */
int main(int argc, char **argv);
void handle_elf_file(const char *file_path);
void display_symbols(Elf *elf);
char determine_symbol_type(GElf_Sym *sym, GElf_Shdr *shdr, Elf_Class class);
void output_symbol(const char *name, Elf64_Addr addr, char type);

#endif