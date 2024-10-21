#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>

#define EI_NIDENT 16

/* Structure for ELF header */
typedef struct {
    unsigned char e_ident[EI_NIDENT]; /* ELF identification */
    uint16_t e_type;                  /* Object file type */
    uint16_t e_machine;               /* Machine type */
    uint32_t e_version;               /* Object file version */
    uint32_t e_entry;                 /* Entry point address */
    uint32_t e_phoff;                 /* Program header offset */
    uint32_t e_shoff;                 /* Section header offset */
    uint32_t e_flags;                 /* Processor-specific flags */
    uint16_t e_ehsize;                /* ELF header size */
    uint16_t e_phentsize;             /* Program header entry size */
    uint16_t e_phnum;                 /* Number of program header entries */
    uint16_t e_shentsize;             /* Section header entry size */
    uint16_t e_shnum;                 /* Number of section header entries */
    uint16_t e_shstrndx;              /* Section header string table index */
} Elf32_Ehdr;

/* Function to create a Solaris ELF file */
void create_solaris_elf(const char *filename) {
    Elf32_Ehdr header;  /* ELF header structure */
    memset(&header, 0, sizeof(header)); /* Initialize the header to zero */

    /* Fill the ELF header fields */
    header.e_ident[0] = 0x7f; /* EI_MAG0 */
    header.e_ident[1] = 'E';  /* EI_MAG1 */
    header.e_ident[2] = 'L';  /* EI_MAG2 */
    header.e_ident[3] = 'F';  /* EI_MAG3 */
    header.e_ident[4] = 1;     /* EI_CLASS (ELF32) */
    header.e_ident[5] = 1;     /* EI_DATA (little endian) */
    header.e_ident[6] = 1;     /* EI_VERSION */
    header.e_ident[7] = 0;     /* EI_OSABI (UNIX - Solaris) */
    header.e_ident[8] = 1;     /* EI_ABIVERSION */
    header.e_type = 2;         /* ET_EXEC (Executable) */
    header.e_machine = 3;      /* EM_386 (Intel 80386) */
    header.e_version = 1;      /* EV_CURRENT */
    header.e_entry = 0x8048400; /* Entry point address */
    header.e_phoff = sizeof(header); /* Program header offset */
    header.e_shoff = 0;        /* Section header offset */
    header.e_flags = 0;        /* No flags */
    header.e_ehsize = sizeof(header); /* ELF header size */
    header.e_phentsize = 0;   /* No program headers */
    header.e_phnum = 0;        /* No program headers */
    header.e_shentsize = 0;   /* No section headers */
    header.e_shnum = 0;        /* No section headers */
    header.e_shstrndx = 0;    /* No section header string table */

    /* Create the ELF file */
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror("open"); /* Print error if file open fails */
        exit(EXIT_FAILURE);
    }

    /* Write the ELF header */
    if (write(fd, &header, sizeof(header)) != sizeof(header)) {
        perror("write"); /* Print error if write fails */
        close(fd); /* Close file descriptor */
        exit(EXIT_FAILURE);
    }

    close(fd); /* Close file descriptor */
    printf("ELF file '%s' created successfully.\n", filename);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s elf_filename\n", argv[0]); /* Print usage message */
        return EXIT_FAILURE;
    }

    create_solaris_elf(argv[1]); /* Create the ELF file */
    return EXIT_SUCCESS;
}