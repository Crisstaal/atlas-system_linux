#ifndef _HNM_H_
#define _HNM_H_

#include <elf.h>
#include <stddef.h>
#include <stdint.h>
#include <gelf.h>

* Function Prototypes */
int main(int argc, char **argv);
void display_symbols_32(Elf *elf);
void display_symbols_64(Elf *elf);
char determine_symbol_type32(GElf_Sym32 *sym, GElf_Shdr *shdr);
char determine_symbol_type64(GElf_Sym *sym, GElf_Shdr *shdr);
void output_symbol(const char *name, Elf64_Addr addr, char type, int elf_class);

#endif