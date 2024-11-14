#ifndef _HNM_H_
#define _HNM_H_

#include <elf.h>
#include <stdint.h>
#include <elf.h>

void print_symbol(const char *name, Elf64_Addr addr, char type);
char get_symbol_type(GElf_Sym *sym, GElf_Shdr *shdr);
void process_symbols(Elf *elf);


#endif