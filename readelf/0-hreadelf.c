// 0-hreadelf.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "hreadelf.h"

/* Function to open the ELF file */
int open_file(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return -1;
    }
    return fd;
}

/* Function to check if the file is an ELF file */
int check_elf(Elf *elf) {
    return (elf_kind(elf) == ELF_K_ELF) ? 1 : (fprintf(stderr, "The file is not an ELF file.\n"), 0);
}

/* Function to print the ELF header */
void print_header(const GElf_Ehdr *ehdr) {
    printf("ELF Header:\n  Magic:   ");
    for (int i = 0; i < EI_NIDENT; i++) {
        printf("%02x ", ehdr->e_ident[i]);
    }
    printf("\n  Class:                             %s\n",
           (ehdr->e_ident[EI_CLASS] == ELFCLASS64) ? "ELF64" : "ELF32");
    printf("  Data:                              %s\n",
           (ehdr->e_ident[EI_DATA] == ELFDATA2LSB) ? "Little Endian" : "Big Endian");
    printf("  Version:                           %d\n", ehdr->e_ident[EI_VERSION]);
    printf("  OS/ABI:                            %d\n", ehdr->e_ident[EI_OSABI]);
    printf("  ABI Version:                       %d\n", ehdr->e_ident[EI_ABIVERSION]);
    printf("  Type:                              0x%x\n", ehdr->e_type);
    printf("  Machine:                           0x%x\n", ehdr->e_machine);
    printf("  Entry point address:               0x%lx\n", (unsigned long)ehdr->e_entry);
    printf("  Start of program headers:          %ld\n", (long)ehdr->e_phoff);
    printf("  Start of section headers:          %ld\n", (long)ehdr->e_shoff);
    printf("  Flags:                             0x%x\n", ehdr->e_flags);
    printf("  Size of this header:               %d\n", ehdr->e_ehsize);
    printf("  Size of program headers:           %d\n", ehdr->e_phentsize);
    printf("  Number of program headers:         %d\n", ehdr->e_phnum);
    printf("  Size of section headers:           %d\n", ehdr->e_shentsize);
    printf("  Number of section headers:         %d\n", ehdr->e_shnum);
    printf("  Section header string table index: %d\n", ehdr->e_shstrndx);
}

void print_elf_header(const char *filename) {
    int fd;
    Elf *elf;
    GElf_Ehdr ehdr;

    /* Initialize the ELF library */
    if (elf_version(EV_CURRENT) == EV_NONE) {
        fprintf(stderr, "ELF library initialization failed: %s\n", elf_errmsg(-1));
        exit(EXIT_FAILURE);
    }

    /* Open the file */
    fd = open_file(filename);
    if (fd == -1) exit(EXIT_FAILURE);

    /* Initialize ELF file descriptor */
    elf = elf_begin(fd, ELF_C_READ, NULL);
    if (!elf) {
        fprintf(stderr, "elf_begin failed: %s\n", elf_errmsg(-1));
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* Check if the file is an ELF file */
    if (!check_elf(elf)) {
        elf_end(elf);
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* Get the ELF header */
    if (gelf_getehdr(elf, &ehdr) == NULL) {
        fprintf(stderr, "gelf_getehdr failed: %s\n", elf_errmsg(-1));
        elf_end(elf);
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* Print the ELF header details */
    print_header(&ehdr);

    /* Clean up */
    elf_end(elf);
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Call function to print the ELF header information */
    print_elf_header(argv[1]);

    return EXIT_SUCCESS;
}