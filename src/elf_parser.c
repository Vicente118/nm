#include "../inc/nm.h"

void init_elf_structures(File *file)
{
    if (file->arch == 0)    // 32bit
        init_elf32(file);
    else                    //64bit
        init_elf64(file); 
}