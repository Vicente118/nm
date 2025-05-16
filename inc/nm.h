#include <elf.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include "../libft/libft.h"

#define A_OUT       "a.out"
#define NM_WARN     "nm: Warning: '"
#define IS_DIR      "' is a directory\n"
#define O_DIR        00200000


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
int    no_args(File *file, const char *filename);











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