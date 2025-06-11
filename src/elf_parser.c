#include "../inc/nm.h"

void init_elf_structures(File *file)
{
    if (file->arch == 1)    init_elf32(file);
    else                    init_elf64(file); 
}


/* 32 bits*/
int init_elf32(File *file)
{
    Elf32_Ehdr  *ehdr = (Elf32_Ehdr *)file->addr;
    Elf32_Shdr  *shdr = NULL;

    file->ehdr = ehdr;

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
        char    *section_name = shstrtab + shdr[i].sh_name;

        if (shdr[i].sh_type == SHT_SYMTAB && ft_strncmp(section_name, SYMTAB, ft_strlen(section_name)) == 0)
        {
            symtab          = (Elf32_Sym *)((char *)file->addr + shdr[i].sh_offset);
            symtab_entries  = shdr[i].sh_size / shdr[i].sh_entsize;

            if (shdr[i].sh_link < ehdr->e_shnum)
            {
                strtab      = (char *)file->addr + shdr[shdr[i].sh_link].sh_offset;
                strtab_size = shdr[shdr[i].sh_link].sh_size;
            }

            break;
        }
    }

    if (!file->symtab || !file->strtab) 
    {
        write(2, NM_SEM, ft_strlen(NM_SEM));
        write(2, file->filename, ft_strlen(file->filename));
        write(2, NO_SYMB, ft_strlen(NO_SYMB));
        munmap(file->addr, file->length);
        close(file->fd);
        exit(1);
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