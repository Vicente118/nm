#include "../inc/nm.h"

static int handle_error(File *file, const char *filename, const char *error_msg)
{
    if (file->addr)
        munmap(file->addr, file->length);
    if (file->fd != -1)
        close(file->fd);
        
    write(2, NM_SEM, ft_strlen(NM_SEM));
    write(2, filename, ft_strlen(filename));
    write(2, error_msg, ft_strlen(error_msg));
    
    return -1;
}

int    file_mapping(File *file, const char *filename)
{
    if (open(filename, O_DIR) >= 0)
    {
        write(2, NM_WARN, ft_strlen(NM_WARN));
        write(2, filename, ft_strlen(filename));
        write(2, IS_DIR, ft_strlen(IS_DIR));
        return -1;
    }

    if ((file->fd = open(filename, O_RDONLY)) == -1)
    {
        write(2, NM_SEM, ft_strlen(NM_SEM));
        write(2, "'", 1);
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

    if (file->sb.st_size == 0)
        return -1;

    file->addr = mmap(NULL, file->sb.st_size, PROT_READ, MAP_PRIVATE, file->fd, 0);

    if (file->addr == MAP_FAILED)
    {
        perror("mmap");
        close(file->fd);
        return -1;
    }
    
    file->length = file->sb.st_size;


    if (file->length < sizeof(Elf64_Ehdr) || ft_memcmp(file->addr, ELFMAG, SELFMAG) != 0)
        return handle_error(file, filename, WR_FORM);

    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)file->addr;

    if (ehdr->e_ident[EI_CLASS] == ELFCLASS32)
        file->arch = ARCH_32BIT;
    else if (ehdr->e_ident[EI_CLASS] == ELFCLASS64)
        file->arch = ARCH_64BIT;
    else
        return handle_error(file, filename, BAD_ARCH);

    if (file->arch == ARCH_64BIT)
    {
        Elf64_Ehdr *ehdr64 = (Elf64_Ehdr *)file->addr;

        if (ehdr64->e_shnum > 0 && (                                /* File contains at least one section */          
            ehdr64->e_shentsize == 0 ||                             /* Check if the section entry size is 0 */
            ehdr64->e_shoff > file->length ||                       /* Check if offset until section table is valid (Inside file) */
            ehdr64->e_shnum > (SIZE_MAX - ehdr64->e_shoff) / ehdr64->e_shentsize ||     /* Protect against integer overflow */
            ehdr64->e_shoff + ehdr64->e_shnum * ehdr64->e_shentsize > file->length))    /* Check that the entire section table is in the file */
        {
            write(1, "bfd plugin: ", ft_strlen("bfd plugin: "));
            write(1, filename, ft_strlen(filename));
            write(1, ": file too short\n", ft_strlen(": file too short\n"));
            return handle_error(file, filename, WR_FORM);
        }
    }
    else 
    {
        Elf32_Ehdr *ehdr32 = (Elf32_Ehdr *)file->addr;
        
        if (ehdr32->e_shnum > 0 && (
            ehdr32->e_shentsize == 0 || 
            ehdr32->e_shoff > file->length ||
            ehdr32->e_shnum > (SIZE_MAX - ehdr32->e_shoff) / ehdr32->e_shentsize ||
            ehdr32->e_shoff + ehdr32->e_shnum * ehdr32->e_shentsize > file->length))
        {
            write(1, "bfd plugin: ", ft_strlen("bfd plugin: "));
            write(1, filename, ft_strlen(filename));
            write(1, ": file too short\n", ft_strlen(": file too short\n"));
            return handle_error(file, filename, WR_FORM);
        }
    }

    if ((ehdr->e_ident[EI_DATA] != 1 && ehdr->e_ident[EI_DATA] != 2) || (ehdr->e_ident[EI_VERSION] != 1))
        return handle_error(file, filename, WR_FORM);

    if (ehdr->e_shoff + ehdr->e_shnum * ehdr->e_shentsize > file->length)
        return handle_error(file, filename, TRUNCATED);

    if (ehdr->e_shoff == 0 && ehdr->e_shnum > 0)        /* No section header offset but at least one section exist*/
        return handle_error(file, filename, WR_FORM);

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
        multiple = 1;
        int success_count = 0;
        
        for (int i = 1; i < argc; i++)
        {
            if (file_mapping(file, argv[i]) == -1)
                continue;
                
            if (success_count > 0)
                write(1, "\n", 1);
            
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