#include "../inc/nm.h"

int nm_process(File *file, const char *filename)
{
    init_elf_structures(file);
    symbol_handler(file);
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