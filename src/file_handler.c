#include "../inc/nm.h"

int    file_mapping(File *file, const char *filename)
{
    /*
        1. Check if filename is DIR
        2. Check if open() failed
        3. Check is fstat failed
        4. Check if mmap()m failed
        5. Check the magic bytes
        6. Check Architecture
        7.  
    */

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
        write(2, "'", 2);
        write(2, filename, ft_strlen(filename));
        write(2, "'", 1);
        write(2, ": ", 2);
        write(2, SUCH_FILE, ft_strlen(SUCH_FILE));
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

    Elf32_Ehdr  *ehdr = (Elf32_Ehdr *)file->addr;

    if (ehdr->e_ident[EI_CLASS] == ELFCLASS32)
        file->arch = ARCH_32BIT;
    else if (ehdr->e_ident[EI_CLASS] == ELFCLASS64)
        file->arch = ARCH_64BIT;
    else
    {
        write(2, NM_SEM, ft_strlen(NM_SEM));
        write(2, filename, ft_strlen(filename));
        write(2, BAD_ARCH, ft_strlen(BAD_ARCH));
        munmap(file->addr, file->length);
        close(file->fd);
        return -1;
    }

    if (ehdr->e_shoff + ehdr->e_shnum * ehdr->e_shentsize > file->length) 
    {
        write(2, NM_SEM, ft_strlen(NM_SEM));
        write(2, filename, ft_strlen(filename));
        write(2, TRUNCATED, ft_strlen(TRUNCATED));
        munmap(file->addr, file->length);
        close(file->fd);
        return -1;
    }

    file->filename = filename;

    return 0;
}


int argument_checker_and_process(int argc, char **argv, File *file)
{
    int result = 0; 

    if (argc == 1) 
    {
        if (file_mapping(file, A_OUT) == -1)
            return -1;
        if (nm_process(file, A_OUT) == -1)
            result = -1;
        cleanup_file(file);
    }
    else if (argc == 2) 
    {
        if (file_mapping(file, argv[1]) == -1)
            return -1;
        if (nm_process(file, argv[1]) == -1)
            result = -1;
        cleanup_file(file);
    }
    else 
    {
        int success_count = 0;
        for (int i = 1; i < argc; i++)
        {
            if (file_mapping(file, argv[i]) == -1)
                continue;
                
            if (success_count > 0)
                write(1, "\n", 1);
                
            write(1, argv[i], ft_strlen(argv[i]));
            write(1, ":\n", 2);
            
            if (nm_process(file, argv[i]) == -1)
                result = -1;
            else
                success_count++;
                
            cleanup_file(file);
        }

        if (success_count == 0)
            return -1;
    }

    return result;
}


void    cleanup_file(File *file)
{
    if (file->addr)
        munmap(file->addr, file->length);
    
    if (file->fd != -1)
        close(file->fd);

    ft_memset(file, 0, sizeof(File));
    file->fd = -1;
}