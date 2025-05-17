#include "../inc/nm.h"


int main(int argc, char **argv)
{
    File    file;

    ft_memset(&file, 0, sizeof(File));
    file.fd = -1;

    if (argument_checker(argc, argv, &file) == -1)
        return 1;
    

    cleanup_file(&file);

    return 0;
}