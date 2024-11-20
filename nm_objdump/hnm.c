#include <stdio.h>
#include <stdlib.h>
#include <gelf.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "hnm.h"

/**
 * main - Entry point for the hnm program
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: 0 on success, or EXIT_FAILURE on errors
 */
int main(int argc, char **argv)
{
    int i;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <elf_file>...\n", argv[0]);
        return (EXIT_FAILURE);
    }

    if (elf_version(EV_CURRENT) == EV_NONE)
    {
        fprintf(stderr, "ELF library initialization failed.\n");
        return (EXIT_FAILURE);
    }

    for (i = 1; i < argc; i++)
    {
        printf("\n%s:\n", argv[i]);
        handle_elf_file(argv[i]);
    }

    return (EXIT_SUCCESS);
}
        
/**
 * handle_elf_file - Processes a single ELF file
 * @file_path: Path to the ELF file
 */
void handle_elf_file(const char *file_path)
{
	int fd;
	Elf *elf;

	fd = open(file_path, O_RDONLY);
	if (fd < 0)
	{
		perror(file_path);
		return;
	}

	elf = elf_begin(fd, ELF_C_READ, NULL);
	if (!elf || elf_kind(elf) != ELF_K_ELF)
	{
		fprintf(stderr, "%s: Not a valid ELF file.\n", file_path);
		if (elf)
			elf_end(elf);
		close(fd);
		return;
	}

	display_symbols(elf);

	elf_end(elf);
	close(fd);
}

/**
 * display_symbols - Extracts and prints symbols from ELF sections
 * @elf: Pointer to the ELF structure
 */
void display_symbols(Elf *elf)
{
	Elf_Scn *section = NULL;
	GElf_Shdr section_header;
	Elf_Data *data;
	GElf_Sym symbol;
	size_t section_str_index;
	size_t i;

	if (elf_getshdrstrndx(elf, &section_str_index) != 0)
	{
		fprintf(stderr, "Failed to get section header string index.\n");
		return;
	}

	while ((section = elf_nextscn(elf, section)) != NULL)
	{
        if (!gelf_getshdr(section, &section_header)) {
            fprintf(stderr, "Failed to get section header.\n");
            continue;
        }

		if (section_header.sh_type != SHT_SYMTAB && section_header.sh_type != SHT_DYNSYM)
			continue;

		data = elf_getdata(section, NULL);
		for (i = 0; i < section_header.sh_size / section_header.sh_entsize; i++)
		{
			if (gelf_getsym(data, i, &symbol) != &symbol)
				continue;

			char *name = elf_strptr(elf, section_header.sh_link, symbol.st_name);
			if (!name) {
                fprintf(stderr, "Failed to get symbol name.\n");
                name = "<no-name>";
                }

            char type = determine_symbol_type(&symbol, &section_header);

            /* Print the symbol in the required format */
            print_symbol(name, symbol.st_value, type);
        }
    }
}
			

/**
 * determine_symbol_type - Determines the type of a symbol
 * @sym: Pointer to the symbol structure
 * @shdr: Pointer to the section header structure
 *
 * Return: Character representing the symbol type
 */
char determine_symbol_type(GElf_Sym *sym, GElf_Shdr *shdr)
{
	 if (GELF_ST_BIND(sym->st_info) == STB_WEAK)
     return (GELF_ST_TYPE(sym->st_info) == STT_OBJECT) ? 'V' : 'W';
    if (sym->st_shndx == SHN_UNDEF)
        return 'U';
    if (sym->st_shndx == SHN_ABS)
        return 'A';
    if (sym->st_shndx == SHN_COMMON)
        return 'C';
    if (shdr->sh_type == SHT_NOBITS && (shdr->sh_flags & SHF_ALLOC))
        return 'B';
    if (shdr->sh_type == SHT_PROGBITS)
    {
        if (shdr->sh_flags & SHF_EXECINSTR)
            return 'T';
        return (shdr->sh_flags & SHF_ALLOC) ? 'R' : '?';
    }
    return (shdr->sh_flags & SHF_WRITE) ? 'D' : '?';
}


/**
 * output_symbol - Prints a symbol's details in the required format
 * @name: Name of the symbol
 * @addr: Address of the symbol
 * @type: Type of the symbol
 */
void output_symbol(const char *name, Elf64_Addr addr, char type)
{
    if (type == 'U' || type == 'w')
        printf("                 %c %s\n", type, name);
    else
        printf("%016lx %c %s\n", addr, type, name);
}