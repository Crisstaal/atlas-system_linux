#include <stdio.h>
#include <stdlib.h>
#include <libelf.h>
#include <gelf.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/**
 * print_symbol - prints symbol information in the required format.
 * @name: symbol name.
 * @addr: symbol address.
 * @type: symbol type.
 * 
 * Prints the symbol name, address, and type as per the `nm -p` output format.
 */
void print_symbol(const char *name, Elf64_Addr addr, char type)
{
	if (type == 'U' || type == 'w')
		printf("                %c %s\n", type, name);
	else
		printf("%016lx %c %s\n", (unsigned long)addr, type, name);
}

/**
 * get_symbol_type - determines the symbol type character based on ELF info.
 * @sym: pointer to the symbol structure.
 * @shdr: pointer to the section header structure.
 * 
 * Return: character representing the symbol type.
 */
char get_symbol_type(GElf_Sym *sym, GElf_Shdr *shdr)
{
	if (GELF_ST_BIND(sym->st_info) == STB_WEAK)
		return (GELF_ST_TYPE(sym->st_info) == STT_OBJECT) ? 'V' : 'W';
	if (GELF_ST_BIND(sym->st_info) == STB_GLOBAL && sym->st_shndx == SHN_UNDEF)
		return 'U';
	if (sym->st_shndx == SHN_ABS)
		return 'A';
	if (sym->st_shndx == SHN_COMMON)
		return 'C';
	if (shdr->sh_type == SHT_NOBITS && shdr->sh_flags == (SHF_ALLOC | SHF_WRITE))
		return 'B';
	if (shdr->sh_type == SHT_PROGBITS && shdr->sh_flags == SHF_ALLOC)
		return 'R';
	if (shdr->sh_type == SHT_PROGBITS && shdr->sh_flags == (SHF_ALLOC | SHF_WRITE))
		return 'D';
	if (shdr->sh_type == SHT_PROGBITS && shdr->sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		return 'T';
	return '?';
}

/**
 * process_symbols - processes and prints symbols from ELF file sections.
 * @elf: pointer to the ELF structure.
 * 
 * Iterates through symbol tables to print symbol information in each section.
 */
void process_symbols(Elf *elf)
{
	size_t shstrndx, i;
	Elf_Scn *scn = NULL;
	GElf_Shdr shdr;
	Elf_Data *data;
	GElf_Sym sym;
	char *name;
	char type;

	if (elf_getshdrstrndx(elf, &shstrndx) != 0)
		return;

	while ((scn = elf_nextscn(elf, scn)) != NULL)
	{
		if (gelf_getshdr(scn, &shdr) != &shdr)
			continue;

		if (shdr.sh_type != SHT_SYMTAB && shdr.sh_type != SHT_DYNSYM)
			continue;

		data = elf_getdata(scn, NULL);
		for (i = 0; i < shdr.sh_size / shdr.sh_entsize; i++)
		{
			if (gelf_getsym(data, i, &sym) != &sym)
				continue;

			name = elf_strptr(elf, shdr.sh_link, sym.st_name);
			if (name && *name != '\0')
			{
				type = get_symbol_type(&sym, &shdr);
				print_symbol(name, sym.st_value, type);
			}
		}
	}
}

/**
 * main - entry point for the hnm program.
 * @argc: argument count.
 * @argv: argument vector.
 * 
 * Processes each file and outputs symbol information.
 * Return: 0 on success, or 1 if errors occur.
 */
int main(int argc, char **argv)
{
	Elf *elf;
	int fd, i;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <objfile>...\n", argv[0]);
		return (EXIT_FAILURE);
	}

	if (elf_version(EV_CURRENT) == EV_NONE)
	{
		fprintf(stderr, "ELF library initialization failed.\n");
		return (EXIT_FAILURE);
	}

	for (i = 1; i < argc; i++)
	{
		fd = open(argv[i], O_RDONLY, 0);
		if (fd < 0)
		{
			perror(argv[i]);
			continue;
		}

		elf = elf_begin(fd, ELF_C_READ, NULL);
		if (!elf || elf_kind(elf) != ELF_K_ELF)
		{
			fprintf(stderr, "%s: Not an ELF file.\n", argv[i]);
			elf_end(elf);
			close(fd);
			continue;
		}

		process_symbols(elf);

		elf_end(elf);
		close(fd);
	}

	return (EXIT_SUCCESS);
}
