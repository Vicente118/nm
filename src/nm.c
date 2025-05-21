#include "../inc/nm.h"

int nm_process(File *file)
{
    init_elf_structures(file);

    return 0;
}


int main(int argc, char **argv)
{
    File    file;

    ft_memset(&file, 0, sizeof(File));
    file.fd = -1;

    if (argument_checker(argc, argv, &file) == -1)
        return 1;
    
    nm_process(&file);

    cleanup_file(&file);

    return 0;
}