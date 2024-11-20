#ifndef HNM_H
#define HNM_H

#include <gelf.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/* Function prototypes */
int main(int argc, char **argv);
void handle_elf_file(const char *file_path);
void display_symbols(Elf *elf);
char determine_symbol_type(GElf_Sym *sym, GElf_Shdr *shdr);
void output_symbol(const char *name, Elf64_Addr addr, char type);

#endif /* HNM_H */
