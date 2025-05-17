#include "../inc/nm.h"

int    file_mapping(File *file, const char *filename)
{
    if (open(filename, O_DIR) >= 0)
    {
        write(2, NM_WARN, ft_strlen(NM_WARN));
        write(2, filename, ft_strlen(filename));
        write(2, IS_DIR, ft_strlen(IS_DIR));
        return -1;
    }

    file->fd = open(filename, O_RDONLY);

    if (file->fd == -1)
    {
        write(2, NM_SEM, ft_strlen(NM_SEM));
        write(2, filename, ft_strlen(filename));
        write(2, ": ", 2);
        perror("");
        return -1;
    }

    if (fstat(file->fd, &file->sb) == -1)
    {
        perror("fstat");
        close(file->fd);
        return -1;
    }

    file->addr = mmap(NULL, file->sb.st_size, PROT_READ, MAP_PRIVATE, file->fd, 0);

    if (file->addr == MAP_FAILED)
    {
        perror("mmap");
        close(file->fd);
        return -1;
    }

    file->length = file->sb.st_size;

    if (file->length < sizeof(Elf64_Ehdr) || ft_memcmp(file->addr, ELFMAG, SELFMAG) != 0)
    {
        write(2, NM_SEM, ft_strlen(NM_SEM));
        write(2, filename, ft_strlen(filename));
        write(2, WR_FORM, ft_strlen(WR_FORM));
        munmap(file->addr, file->length);
        close(file->fd);
        return -1;
    }

    Elf64_Ehdr  *ehdr = (Elf64_Ehdr *)file->addr;

    if (ehdr->e_ident[EI_CLASS] == ELFCLASS32)
        file->arch = 1;
    else if (ehdr->e_ident[EI_CLASS] == ELFCLASS64)
        file->arch = 2;
    else
    {
        write(2, NM_SEM, ft_strlen(NM_SEM));
        write(2, filename, ft_strlen(filename));
        write(2, BAD_ARCH, ft_strlen(BAD_ARCH));
        return -1;
    }

    return 0;
}


int argument_checker(int argc, char **argv, File *file)
{
    if (argc == 1) 
    {
        if (file_mapping(file, A_OUT) == -1)
            return -1;
    }
    
    else if (argc == 2) 
    {
        if (file_mapping(file, argv[1]) == -1)
            return -1;
    }

    else 
    {
        write(2, EXC_ARG, ft_strlen(EXC_ARG));
        return -1;
    }

    return 0;
}


void    cleanup_file(File *file)
{
    if (file->addr)
        munmap(file->addr, file->length);
    
    if (file->fd != -1)
        close(file->fd);
}