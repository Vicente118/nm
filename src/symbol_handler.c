#include "../inc/nm.h"

void    symbol_handler(File *file)
{
    // char    **symbols_array = malloc(sizeof(char *) * file->symtab_entries + 1);
    
    // if (!symbols_array)
    // {
    //     write(2, MALLOC_FAIL, ft_strlen(MALLOC_FAIL));
    //     munmap(file->addr, file->length);
    //     close(file->fd);
    //     exit(1);
    // }

    Elf64_Sym *symtab = (Elf64_Sym *)file->symtab;

    for (size_t i = 0; i < file->symtab_entries; i++)
    {
        if (symtab[i].st_name == 0 || (ELF64_ST_TYPE(symtab[i].st_info) == STT_FILE))
                continue;

        char *name = (char *)file->strtab + symtab[i].st_name;
        printf("%s\n", name);
    }
}

char    symbol_associate_letter()
{

}