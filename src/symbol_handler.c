#include "../inc/nm.h"

Symbol    *symbol_handler(File *file)
{
    Symbol      *symbols = malloc(sizeof(Symbol) * file->symtab_entries);

    if (!symbols)
    {
        write(2, MALLOC_FAIL, ft_strlen(MALLOC_FAIL));
        cleanup_file(file);
        exit(1);
    }

    ft_memset(symbols, 0, sizeof(Symbol) * file->symtab_entries);

    /* 32 BITS TREATMENT */
    if (file->arch == ARCH_32BIT)
    {
        Elf32_Sym   *symtab = (Elf32_Sym *)file->symtab;

        for (size_t i = 0; i < file->symtab_entries; i++)
        {
            if (symtab[i].st_name == 0 || ELF32_ST_TYPE(symtab[i].st_info) == STT_FILE)
                continue;
            
            extract_symbol_32(file, &symtab[i], &symbols[i]);
        }
    }

    /* 64 BITS TREATMENT */
    else
    {
        Elf64_Sym   *symtab = (Elf64_Sym *)file->symtab;

        for (size_t i = 0; i < file->symtab_entries; i++)
        {
            if (symtab[i].st_name == 0 || ELF32_ST_TYPE(symtab[i].st_info) == STT_FILE)
                continue;
            
            extract_symbol_64(file, &symtab[i], &symbols[i]);
        }
    }

    return symbols;
}


void    extract_symbol_32(File *file, Elf32_Sym *symtab, Symbol *symbol)
{
    char    *name   = (char *)file->strtab + symtab->st_name;

    if (symtab->st_name >= file->strtab_size || !is_valid_string(name, (char *)file->strtab + file->strtab_size, file->strtab_size - symtab->st_name)) 
        return;

    symbol->name    = ft_strdup(name);
    symbol->address = symtab->st_value;

    char    type    = get_symbol_type(file, symtab, ARCH_32BIT);

    if (ft_strncmp(name, "data_start", INT32_MAX) == 0)
        type = 'W';
    else if (ft_strnstr(name, "_array_entry", INT32_MAX) || ft_strncmp(name, "__init_array_start", INT32_MAX) == 0 ||  ft_strncmp(name, "__init_array_end", INT32_MAX) == 0)
        type = 'd';
    else if (ft_strncmp(name, "__abi_tag", INT32_MAX) == 0)
        type = 'r';
        
    if (type != '?' && type != 'U' && type != 'w')
    {
        if (ELF32_ST_BIND(symtab->st_info) == STB_LOCAL)
            type = ft_tolower(type);
        else
            type = ft_toupper(type);
    }

    symbol->type = type;
}

void    extract_symbol_64(File *file, Elf64_Sym *symtab, Symbol *symbol)
{
    char    *name   = (char *)file->strtab + symtab->st_name;

    if (symtab->st_name >= file->strtab_size || !is_valid_string(name, (char *)file->strtab + file->strtab_size, file->strtab_size - symtab->st_name)) 
        return;
        
    symbol->name    = ft_strdup(name);
    symbol->address = symtab->st_value;

    char    type    = get_symbol_type(file, symtab, ARCH_64BIT);

    if (ft_strncmp(name, "data_start", INT32_MAX) == 0)
        type = 'W';
    else if (ft_strnstr(name, "_array_entry", INT32_MAX) || ft_strncmp(name, "__init_array_start", INT32_MAX) == 0 ||  ft_strncmp(name, "__init_array_end", INT32_MAX) == 0)
        type = 'd';
    else if (ft_strncmp(name, "__abi_tag", INT32_MAX) == 0)
        type = 'r';

    if (type != '?' && type != 'U' && type != 'w')
    {
        if (ELF64_ST_BIND(symtab->st_info) == STB_LOCAL)
            type = ft_tolower(type);
        else
            type = ft_toupper(type);
    }

    symbol->type = type;
}

char    get_symbol_type(File *file, void *sym_ptr, int arch)
{
    uint16_t        st_shndx;                       /* Index of section associate to symbol                    */
    char            type_char = '?';                /* Type of symbol initialzed to unknown                    */
    unsigned char   sym_type, sym_bind;             /* Symbol type  --- Local, Global or Weak symbol           */
    unsigned int    sh_type = 0, sh_flags = 0;      /* Section type --- Sections flag                          */

    if (arch == ARCH_32BIT)
    {
        Elf32_Shdr  *shdr32;
        Elf32_Sym   *sym = (Elf32_Sym *)sym_ptr;

        st_shndx = sym->st_shndx;
        sym_type = ELF32_ST_TYPE(sym->st_info);
        sym_bind = ELF32_ST_BIND(sym->st_info);

        /* Weak symbols first*/
        if (sym_bind == STB_WEAK)
        {
            if (st_shndx == SHN_UNDEF)
                return 'w';  // weak, undefined
            else
                return (sym_type == STT_OBJECT) ? 'V' : 'W';  // weak, defined (V pour objets, W pour fonctions)
        }

        /* Special sections */
        switch (st_shndx)
        {
            case SHN_UNDEF:         /* Undefined symbol*/
                return 'U';

            case SHN_ABS:           /* Absolute symbol */
                return 'A';

            case SHN_COMMON:        /* Common symbol   */
                return 'C';
        }

        if (st_shndx < 0xFF00)
        {
            shdr32   = &((Elf32_Shdr *)file->shdr)[st_shndx];   
            sh_type  = shdr32->sh_type;
            sh_flags = shdr32->sh_flags;    
        }   
    }

    else    
    {
        Elf64_Shdr  *shdr64;
        Elf64_Sym   *sym = (Elf64_Sym *)sym_ptr;

        st_shndx = sym->st_shndx;
        sym_type = ELF64_ST_TYPE(sym->st_info);
        sym_bind = ELF64_ST_BIND(sym->st_info);
        
        /* Weak symbols first*/
        if (sym_bind == STB_WEAK)
        {
            if (st_shndx == SHN_UNDEF)
                return 'w';  // weak, undefined
            else
                return (sym_type == STT_OBJECT) ? 'V' : 'W';  // weak, defined (V for objects, W for functions)
        }

        /* Special sections */
        switch (st_shndx)
        {
            case SHN_UNDEF:         /* Undefined symbol*/
                return 'U';

            case SHN_ABS:           /* Absolute symbol */
                return 'A';
                    
            case SHN_COMMON:        /* Common symbol   */   
                return 'C'; 
        }

        if (st_shndx < 0xFF00)
        {
            shdr64   = &((Elf64_Shdr *)file->shdr)[st_shndx];
            sh_type  = shdr64->sh_type;
            sh_flags = shdr64->sh_flags;
        }
    }

    if (sym_type      == STT_GNU_IFUNC)
        type_char = 'i';
    else if (sh_type  == SHT_PROGBITS && (sh_flags & SHF_EXECINSTR))
        type_char = 'T';  // Executable code
    else if (sh_type  == SHT_PROGBITS && (sh_flags & SHF_WRITE))
        type_char = 'D';  // Initialized data for Read/Write
    else if (sh_type  == SHT_PROGBITS)
        type_char = 'R';  // Read-Only data
    else if (sh_type  == SHT_DYNAMIC)
        type_char = 'D';  // Dynamic section
    else if (sh_type  == SHT_INIT_ARRAY)
        type_char = 'T';  // Initialization section
    else if (sh_type  == SHT_FINI_ARRAY)
        type_char = 'T';  // Finalization section
    else if (sh_type  == SHT_NOBITS && (sh_flags & SHF_ALLOC) && (sh_flags & SHF_WRITE))
        type_char = 'B';  // Uninitialized data (BSS)
    else if (sh_type  == SHT_DYNSYM || sh_type == SHT_SYMTAB)
        type_char = 'r';  // Symbol table
    else if (sym_type == STT_FUNC)
        type_char = 'T';  // Function
    else if (sym_type == STT_OBJECT)
        type_char = 'D';  // Object/variable
    else if (sym_type == STT_SECTION)
        type_char = 'S';  // Section
    else if (sym_type == STT_FILE)
        type_char = 'f';  // File
    else if (sh_flags & SHF_WRITE)
        type_char = 'D';
    else if (sh_flags & SHF_ALLOC)
        type_char = 'r';
    
    return type_char;
}