#include "../inc/nm.h"

int init_elf_structures(File *file)
{
    if (file->arch == ARCH_32BIT)
    {
        if (init_elf32(file) == -1)
            return -1;
    }
    else
    {                    
        if (init_elf64(file) == -1)
            return -1;
    }
    return 0; 
}


/* 32 bits*/
int init_elf32(File *file)
{
    Elf32_Ehdr  *ehdr = (Elf32_Ehdr *)file->addr;
    Elf32_Shdr  *shdr = NULL;

    file->ehdr = ehdr;

    if (ehdr->e_shstrndx >= ehdr->e_shnum)
    {
        write(2, NM_SEM, ft_strlen(NM_SEM));
        write(2, file->filename, ft_strlen(file->filename));
        write(2, INV_SHSTI, ft_strlen(INV_SHSTI));
        munmap(file->addr, file->length);
        close(file->fd);
        return -1;
    }
    
    /* Init section table */

    shdr       = (Elf32_Shdr *)((char *)file->addr + ehdr->e_shoff);
    file->shdr = shdr;

    /* Find  .symtab section: Symbole Table Section */
    /* .strtab section: String Table associated to .symtab -> Contains name of symbols */
    Elf32_Sym   *symtab        = NULL;
    char        *strtab        = NULL;
    size_t      symtab_entries = 0;
    size_t      strtab_size    = 0;

    char        *shstrtab = (char *)file->addr + shdr[ehdr->e_shstrndx].sh_offset;

    for (int i = 0; i < ehdr->e_shnum; i++)
    {
        if (shdr[i].sh_name >= shdr[ehdr->e_shstrndx].sh_size) {
            write(2, NM_SEM, ft_strlen(NM_SEM));
            write(2, file->filename, ft_strlen(file->filename));
            write(2, WR_FORM, ft_strlen(WR_FORM));
            munmap(file->addr, file->length);
            close(file->fd);
            return -1;
        }
    }
    
    if (shdr[ehdr->e_shstrndx].sh_offset >= file->length || shdr[ehdr->e_shstrndx].sh_offset + shdr[ehdr->e_shstrndx].sh_size > file->length) 
    {
        write(2, NM_SEM, ft_strlen(NM_SEM));
        write(2, file->filename, ft_strlen(file->filename));
        write(2, INV_SST, ft_strlen(INV_SST));
        return -1;
    }

    for (int i = 0; i < ehdr->e_shnum; i++)
    {
        char    *section_name = shstrtab + shdr[i].sh_name;

        if (shdr[i].sh_type == SHT_SYMTAB && ft_strncmp(section_name, SYMTAB, ft_strlen(section_name)) == 0)
        {
            symtab          = (Elf32_Sym *)((char *)file->addr + shdr[i].sh_offset);    // Pointer to .symtab
            symtab_entries  = shdr[i].sh_size / shdr[i].sh_entsize;                     // Numbers of symbols
         
            if (shdr[i].sh_link < ehdr->e_shnum)
            {
                strtab      = (char *)file->addr + shdr[shdr[i].sh_link].sh_offset; // "String1\0String2\0...."
                strtab_size = shdr[shdr[i].sh_link].sh_size;                        // Size of .strtab section
            }
            break;
        }
    }

    if (ehdr->e_shoff >= file->length || ehdr->e_shnum * ehdr->e_shentsize > file->length - ehdr->e_shoff)
    {
        write(2, NM_SEM, ft_strlen(NM_SEM));
        write(2, file->filename, ft_strlen(file->filename));
        write(2, INV_SHO, ft_strlen(INV_SHO));
        return -1;
    }

    if (!symtab || !strtab) 
    {
        write(2, NM_SEM, ft_strlen(NM_SEM));
        write(2, file->filename, ft_strlen(file->filename));
        write(2, NO_SYMB, ft_strlen(NO_SYMB));
        munmap(file->addr, file->length);
        close(file->fd);
        return (-1);
    }

    file->symtab         = symtab;
    file->strtab         = strtab;
    file->symtab_entries = symtab_entries;
    file->strtab_size    = strtab_size;

    return 0;
}



/* 64 bits */
int init_elf64(File *file)
{
    Elf64_Ehdr  *ehdr = (Elf64_Ehdr *)file->addr;
    Elf64_Shdr  *shdr = NULL;

    file->ehdr = ehdr;

    if (ehdr->e_shstrndx >= ehdr->e_shnum)
    {
        write(2, NM_SEM, ft_strlen(NM_SEM));
        write(2, file->filename, ft_strlen(file->filename));
        write(2, INV_SHSTI, ft_strlen(INV_SHSTI));
        munmap(file->addr, file->length);
        close(file->fd);
        return -1;
    }
    
    /* Init section table */

    shdr       = (Elf64_Shdr *)((char *)file->addr + ehdr->e_shoff);
    file->shdr = shdr;

    /* Find  .symtab section: Symbole Table Section */
    /* .strtab section: String Table associated to .symtab -> Contains name of symbols */
    Elf64_Sym   *symtab        = NULL;
    char        *strtab        = NULL;
    size_t      symtab_entries = 0;
    size_t      strtab_size    = 0;

    char        *shstrtab = (char *)file->addr + shdr[ehdr->e_shstrndx].sh_offset;      // Index of shrd to .shstrtab

    for (int i = 0; i < ehdr->e_shnum; i++)
    {
        if (shdr[i].sh_name >= shdr[ehdr->e_shstrndx].sh_size) {
            write(2, NM_SEM, ft_strlen(NM_SEM));
            write(2, file->filename, ft_strlen(file->filename));
            write(2, WR_FORM, ft_strlen(WR_FORM));
            munmap(file->addr, file->length);
            close(file->fd);
            return -1;
        }
    }
    
    if (shdr[ehdr->e_shstrndx].sh_offset >= file->length || shdr[ehdr->e_shstrndx].sh_offset + shdr[ehdr->e_shstrndx].sh_size > file->length) 
    {
        write(2, NM_SEM, ft_strlen(NM_SEM));
        write(2, file->filename, ft_strlen(file->filename));
        write(2, INV_SST, ft_strlen(INV_SST));
        return -1;
    }

    for (int i = 0; i < ehdr->e_shnum; i++)         // Loop through each sections headers until finding .symtab
    {
        char    *section_name = shstrtab + shdr[i].sh_name;

        if (shdr[i].sh_type == SHT_SYMTAB && ft_strncmp(section_name, SYMTAB, ft_strlen(section_name)) == 0) // Find .symtab
        {
            symtab          = (Elf64_Sym *)((char *)file->addr + shdr[i].sh_offset);     // Save symtab offset
            symtab_entries  = shdr[i].sh_size / shdr[i].sh_entsize;                      // Save number of symbols

            if (shdr[i].sh_link < ehdr->e_shnum)        // check if the index of .strtab of shdr is less than the number of section
            {
                strtab      = (char *)file->addr + shdr[shdr[i].sh_link].sh_offset;
                strtab_size = shdr[shdr[i].sh_link].sh_size;
            }
            break;
        }
    }

    if (ehdr->e_shoff >= file->length || ehdr->e_shnum * ehdr->e_shentsize > file->length - ehdr->e_shoff)
    {
        write(2, NM_SEM, ft_strlen(NM_SEM));
        write(2, file->filename, ft_strlen(file->filename));
        write(2, INV_SHO, ft_strlen(INV_SHO));
        return -1;
    }

    if (!symtab || !strtab) 
    {
        write(2, NM_SEM, ft_strlen(NM_SEM));
        write(2, file->filename, ft_strlen(file->filename));
        write(2, NO_SYMB, ft_strlen(NO_SYMB));
        munmap(file->addr, file->length);
        close(file->fd);
        return -1;
    }

    file->symtab         = symtab;
    file->strtab         = strtab;
    file->symtab_entries = symtab_entries;
    file->strtab_size    = strtab_size;

    return 0;
}