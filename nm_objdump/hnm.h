#ifndef _HNM_H_
#define _HNM_H_

#include <elf.h>
#include <stddef.h>
#include <stdint.h>
#include <elf.h>

/* Function Prototypes */
void check_dependencies(void);
void display_symbols(Elf *elf);
char determine_symbol_type(GElf_Sym *sym, GElf_Shdr *shdr);
void output_symbol(const char *name, Elf64_Addr addr, char type);
int compare_symbols(const void *a, const void *b);
void handle_elf_file(const char *file_path);

#endif