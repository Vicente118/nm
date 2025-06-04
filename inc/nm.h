#include <elf.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include "../libft/libft.h"

#define A_OUT       "a.out"
#define NM_SEM      "nm: "
#define NM_WARN     "nm: Warning: '"
#define WR_FORM     ": file format not recognized\n"
#define IS_DIR      "' is a directory\n"
#define BAD_ARCH    ": Unsupported ELF file class\n"
#define TRUNCATED   ": File truncated or corrupted\n"
#define NO_SYMB     ": No symbols\n"
#define O_DIR        00200000
#define ARCH_32BIT   1
#define ARCH_64BIT   2


typedef unsigned char uchar;

typedef struct
{
    char        *addr;
    size_t      length;
    int         prot;
    int         flags;
    int         fd;
    off_t       offset;

    struct stat sb;

    int         arch;
    void        *ehdr;
    void        *shdr;
    void        *symtab;
    void        *strtab;

    size_t      symtab_entries;
    size_t      strtab_size;

}   File;

typedef struct 
{
    char        *name;          // Symbol name
    uint64_t    address;        // Symbol address
    uint64_t    size;           // Symbole size in bytes
    uchar       type;           // Symbole type
    uchar       bind;           // Binding (LOCAL, GLOBAL, WEAK)
    uchar       visibility;     // Visibility
    uint16_t    section_index;  // Index of the associate function

}   Symbol;


typedef struct
{
    Symbol      *symbols;
    size_t      count;
    size_t      capacity;

}   SymbolTable;



/// Parsing
int     argument_checker_and_process(int argc, char **argv, File *file);
int     file_mapping(File *file, const char *filename);
int     nm_process(File *file, const char *filename);
void    init_elf_structures(File *file);
int     init_elf32(File *file);
int     init_elf64(File *file);





/// Cleanup
void    cleanup_file(File *file);



/*
ELF Header Struct:

typedef struct
{
    unsigned char   e_ident[EI_NIDENT];     Magic number and other info 
    Elf64_Half      e_type;			        Object file type 
    Elf64_Half      e_machine;		        Architecture 
    Elf64_Word      e_version;		        Object file version 
    Elf64_Addr      e_entry;		        Entry point virtual address 
    Elf64_Off       e_phoff;		        Program header table file offset 
    Elf64_Off       e_shoff;		        Section header table file offset 
    Elf64_Word	    e_flags;		        Processor-specific flags 
    Elf64_Half	    e_ehsize;		        ELF header size in bytes 
    Elf64_Half	    e_phentsize;		    Program header table entry size 
    Elf64_Half	    e_phnum;		        Program header table entry count 
    Elf64_Half	    e_shentsize;		    Section header table entry size 
    Elf64_Half	    e_shnum;		        Section header table entry count 
    Elf64_Half	    e_shstrndx;		        Section header string table index 
}   Elf64_Ehdr;
*/