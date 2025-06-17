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
#define NO_SYMB     ": no symbols\n"
#define INV_SHO     ": Invalid section header offset\n"
#define INV_SST     ": Invalid section string table\n"
#define SUCH_FILE   "No such file\n"
#define MALLOC_FAIL "malloc failed\n"
#define SYMTAB      ".symtab"
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

    const char  *filename;

}   File;

typedef struct 
{
    char        *name;          // Symbol name
    char        *trim_name;
    uint64_t    address;        // Symbol address
    uchar       type;           // Symbol type

    int         size;

}   Symbol;



/// Parsing
int     argument_checker_and_process(int argc, char **argv, File *file);
int     file_mapping(File *file, const char *filename);
int     nm_process(File *file, const char *filename);
int     init_elf_structures(File *file);
int     init_elf32(File *file);
int     init_elf64(File *file);
void    extract_symbol_32(File *file, Elf32_Sym *sym, Symbol *symbol);
void    extract_symbol_64(File *file, Elf64_Sym *sym, Symbol *symbol);
char    get_symbol_type(File *file, void *sym_ptr, int arch);

/// Symbol 
Symbol  *symbol_handler(File *file);
void    sort_symbol(Symbol *symbols, int size, int arch);

/// Display
void    display_symbols(Symbol *symbols, File *file, int size);

/// Utils
int     ft_strcasecmp(const char *s1, const char *s2);
char    *ft_itoa_hex(unsigned long n);
int     is_valid_string(const char *start, const char *end, size_t max_len);
/// Cleanup
void    cleanup_file(File *file);
void    free_names(int size, Symbol *symbols);


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