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
    int fd;
    Elf32_Ehdr *ehdr32;
    Elf64_Ehdr *ehdr64;
    Elf *elf;

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
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0)
        {
            perror(argv[i]);
            continue;
        }

        elf = elf_begin(fd, ELF_C_READ, NULL);
        if (!elf || elf_kind(elf) != ELF_K_ELF)
        {
            fprintf(stderr, "%s: Not a valid ELF file.\n", argv[i]);
            elf_end(elf);
            close(fd);
            continue;
        }

        if (elf_getheader(elf, &ehdr64) == 0)
        {
            if (ehdr64->e_ident[EI_CLASS] == ELFCLASS32)
            {
                display_symbols_32(elf);
            }
            else if (ehdr64->e_ident[EI_CLASS] == ELFCLASS64)
            {
                display_symbols_64(elf);
            }
            else
            {
                fprintf(stderr, "%s: Unknown ELF class.\n", argv[i]);
            }
        }

        elf_end(elf);
        close(fd);
    }

    return (EXIT_SUCCESS);
}

/**
 * display_symbols_32 - Extracts and prints symbols from ELF sections
 * @elf: Pointer to the ELF structure
 */
void display_symbols_32(Elf *elf)
{
	Elf_Scn *section = NULL;
	GElf_Shdr section_header;
	Elf_Data *data;
	GElf_Sym32 symbol;
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
            GElf_Sym symbol;
			if (gelf_getsym32(data, i, &symbol) != &symbol)
				continue;

			char *name = elf_strptr(elf, section_header.sh_link, symbol.st_name);
			if (!name) {
                fprintf(stderr, "Failed to get symbol name.\n");
                name = "<no-name>";
                }

            char type = determine_symbol_type(&symbol, &section_header);

            /* Print the symbol in the required format */
            output_symbol(name, symbol.st_value, type);
        }
    }
}
			
/**
 * display_symbols_64 - Extracts and prints symbols from ELF sections (64-bit)
 * @elf: Pointer to the ELF structure
 */
void display_symbols_64(Elf *elf)
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
        if (!gelf_getshdr(section, &section_header))
        {
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
            if (!name)
            {
                name = "<no-name>";
            }

            char type = determine_symbol_type64(&symbol, &section_header);
            output_symbol(name, symbol.st_value, type, ELFCLASS64);
        }
    }
}

//**
 * determine_symbol_type32 - Determines the type of a symbol (32-bit)
 * @sym: Pointer to the symbol structure
 * @shdr: Pointer to the section header structure
 *
 * Return: Character representing the symbol type
 */
char determine_symbol_type32(GElf_Sym32 *sym, GElf_Shdr *shdr)
{
    if (GELF_ST_BIND(sym->st_info) == STB_WEAK)
    {
        return (GELF_ST_TYPE(sym->st_info) == STT_OBJECT) ? 'V' : 'W';
    }
    if (sym->st_shndx == SHN_UNDEF)
    {
        return ('U');
    }
    if (sym->st_shndx == SHN_ABS)
    {
        return ('A');
    }
    if (sym->st_shndx == SHN_COMMON)
    {
        return ('C');
    }
    if (shdr == NULL)
    {
        return ('?');
    }

    if (shdr->sh_type == SHT_PROGBITS && (shdr->sh_flags & SHF_EXECINSTR))
    {
        return ('T');
    }

    if (shdr->sh_flags & SHF_WRITE)
    {
        if (shdr->sh_type == SHT_NOBITS)
        {
            return ('B');
        }
        return ('D');
    }

    if (shdr->sh_flags & SHF_ALLOC)
    {
        return ('R');
    }

    return ('?');
}

/**
 * determine_symbol_type64 - Determines the type of a symbol (64-bit)
 * @sym: Pointer to the symbol structure
 * @shdr: Pointer to the section header structure
 *
 * Return: Character representing the symbol type
 */
char determine_symbol_type64(GElf_Sym *sym, GElf_Shdr *shdr)
{
    if (GELF_ST_BIND(sym->st_info) == STB_WEAK)
    {
        return (GELF_ST_TYPE(sym->st_info) == STT_OBJECT) ? 'V' : 'W';
    }
    if (sym->st_shndx == SHN_UNDEF)
    {
        return ('U');
    }
    if (sym->st_shndx == SHN_ABS)
    {
        return ('A');
    }
    if (sym->st_shndx == SHN_COMMON)
    {
        return ('C');
    }
    if (shdr == NULL)
    {
        return ('?');
    }

    if (shdr->sh_type == SHT_PROGBITS && (shdr->sh_flags & SHF_EXECINSTR))
    {
        return ('T');
    }

    if (shdr->sh_flags & SHF_WRITE)
    {
        if (shdr->sh_type == SHT_NOBITS)
        {
            return ('B');
        }
        return ('D');
    }

    if (shdr->sh_flags & SHF_ALLOC)
    {
        return ('R');
    }

    return ('?');
}

/**
 * output_symbol - Prints a symbol's details in the required format
 * @name: Name of the symbol
 * @addr: Address of the symbol
 * @type: Type of the symbol
 * @elf_class: ELF class (32-bit or 64-bit)
 */
void output_symbol(const char *name, Elf64_Addr addr, char type, int elf_class)
{
    if (elf_class == ELFCLASS64)
    {
        printf("%016lx %c %s\n", addr, type, name);  // For 64-bit addresses
    }
    else if (elf_class == ELFCLASS32)
    {
        printf("%08lx %c %s\n", addr, type, name);  // For 32-bit addresses
    }
}