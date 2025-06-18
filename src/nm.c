#include "../inc/nm.h"

int nm_process(File *file, const char *filename)
{
    if (init_elf_structures(file) == -1)
        return -1;

    Symbol *symbols = symbol_handler(file);

    sort_symbol(symbols, file, file->symtab_entries, file->arch);

    display_symbols(symbols, file, file->symtab_entries);

    free_names(file->symtab_entries, symbols);
    free(symbols);
    
    return 0;
}


int main(int argc, char **argv)
{
    File    file;

    ft_memset(&file, 0, sizeof(File));
    file.fd = -1;

    if (argument_checker_and_process(argc, argv, &file) == -1)
        return 1;

    return 0;
}