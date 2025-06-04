#include "../inc/nm.h"

int nm_process(File *file, const char *filename)
{
    init_elf_structures(file);

    // if (!file->symtab || !file->strtab) 
    // {
    //     write(2, NM_SEM, ft_strlen(NM_SEM));
    //     write(2, filename, ft_strlen(filename));
    //     write(2, NO_SYMB, ft_strlen(NO_SYMB));
    //     munmap(file->addr, file->length);
    //     close(file->fd);
    //     return -1;
    // }

    return 0;
}


int main(int argc, char **argv)
{
    File    file;

    ft_memset(&file, 0, sizeof(File));
    file.fd = -1;

    if (argument_checker_and_process(argc, argv, &file) == -1)
        return 1;

    cleanup_file(&file);

    return 0;
}