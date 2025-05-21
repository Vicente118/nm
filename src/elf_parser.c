#include "../inc/nm.h"

void init_elf_structures(File *file)
{
    if (file->arch == 1)    // 32bit
        init_elf32(file);
    else                    //64bit
        init_elf64(file); 
}

int init_elf32(File *file)
{
    file->ehdr = (Elf32_Ehdr *)file->addr;

    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)file->ehdr;

    if (ehdr->e_shoff == 0 || ehdr->e_shnum == 0)
    {
        return -1;
    }

    file->shdr = (Elf32_Shdr *)((char *)file->addr + ehdr->e_shoff);

    return 0;
}