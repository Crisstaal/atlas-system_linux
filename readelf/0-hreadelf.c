#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <gelf.h>
#include <libelf.h>
#include <byteswap.h>

#define ELF_MAGIC_LEN 16

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

/* Function to return a string for the OS/ABI value */
const char* osabi_to_string(unsigned char osabi) {
    switch (osabi) {
        case ELFOSABI_SYSV: return "UNIX - System V";
        case ELFOSABI_NETBSD: return "UNIX - NetBSD";
        case ELFOSABI_LINUX: return "UNIX - Linux";
        case ELFOSABI_SOLARIS: return "UNIX - Solaris";
        default: return "Unknown";
    }
}

/* Function to return a string for the machine type */
const char* machine_to_string(unsigned int machine) {
    switch (machine) {
        case EM_X86_64: return "Advanced Micro Devices X86-64";
        case EM_386: return "Intel 80386";
        case EM_SPARC: return "Sparc";
        // Add other machine types as needed
        default: return "Unknown";
    }
}

/* Function to print the ELF header */
void print_header(const GElf_Ehdr *ehdr) {
    printf("ELF Header:\n");
    printf("  Magic:                             ");
    for (int i = 0; i < ELF_MAGIC_LEN; i++) {
        printf("%02x ", ehdr->e_ident[i]);
    }
    printf("\n");
    
    printf("  Class:                             %s\n", 
           (ehdr->e_ident[EI_CLASS] == ELFCLASS64) ? "ELF64" : "ELF32");
    
    printf("  Data:                              %s\n", 
           (ehdr->e_ident[EI_DATA] == ELFDATA2LSB) ? "2's complement, little endian" : "2's complement, big endian");
    
    printf("  Version:                           %d (current)\n", ehdr->e_ident[EI_VERSION]);
    printf("  OS/ABI:                            %s\n", osabi_to_string(ehdr->e_ident[EI_OSABI]));
    printf("  ABI Version:                       %d\n", ehdr->e_ident[EI_ABIVERSION]);

    // Updated printf for Type
    printf("  Type:                              ");
    switch (ehdr->e_type) {
        case ET_EXEC: 
            printf("EXEC (Executable file)\n");
            break;
        case ET_DYN:
            printf("DYN (Shared object file)\n");
            break;
        case ET_REL:
            printf("REL (Relocatable file)\n");
            break;
        default:
            printf("0x%x\n", ehdr->e_type);
            break;
    }

    if (ehdr->e_ident[EI_DATA] == ELFDATA2LSB) /* little endian */
    {
        printf("  Machine:                           %s\n", machine_to_string(ehdr->e_machine));
        printf("  Entry point address:               0x%lx\n", (unsigned long)ehdr->e_entry);
        printf("  Start of program headers:          %ld (bytes into file)\n", (long)ehdr->e_phoff);
        printf("  Start of section headers:          %ld (bytes into file)\n", (long)ehdr->e_shoff);
        printf("  Flags:                             0x%x\n", ehdr->e_flags);
        printf("  Size of this header:               %d (bytes)\n", ehdr->e_ehsize);
        printf("  Size of program headers:           %d (bytes)\n", ehdr->e_phentsize);
        printf("  Number of program headers:         %d\n", ehdr->e_phnum);
        printf("  Size of section headers:           %d (bytes)\n", ehdr->e_shentsize);
        printf("  Number of section headers:         %d\n", ehdr->e_shnum);
        printf("  Section header string table index: %d\n", ehdr->e_shstrndx);
    }
    else /* big endian, need to swap byte order multi-byte integers */
    {
        printf("  Machine:                           %s\n", machine_to_string(ehdr->e_machine));
        printf("  Entry point address:               0x%lx\n", (unsigned long)bswap_32(ehdr->e_entry));
        printf("  Start of program headers:          %ld (bytes into file)\n", (long)bswap_32(ehdr->e_phoff));
        printf("  Start of section headers:          %ld (bytes into file)\n", (long)bswap_32(ehdr->e_shoff));
        printf("  Flags:                             0x%x\n", bswap_32(ehdr->e_flags));
        printf("  Size of this header:               %d (bytes)\n", bswap_32(ehdr->e_ehsize));
        printf("  Size of program headers:           %d (bytes)\n", bswap_32(ehdr->e_phentsize));
        printf("  Number of program headers:         %d\n", bswap_32(ehdr->e_phnum));
        printf("  Size of section headers:           %d (bytes)\n", bswap_32(ehdr->e_shentsize));
        printf("  Number of section headers:         %d\n", bswap_32(ehdr->e_shnum));
        printf("  Section header string table index: %d\n", bswap_32(ehdr->e_shstrndx));
    }
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