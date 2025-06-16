#include "../inc/nm.h"

void display_symbols(Symbol *symbols, File *file, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (symbols[i].name == NULL)
            continue;

        char type[2] = {symbols[i].type, '\0'};

        if (symbols[i].type == 'U' || symbols[i].type == 'w')
        {
            write(1, "                 ", 17);
            write(1, type, 1);
            write(1, " ", 1);
            write(1, symbols[i].name, ft_strlen(symbols[i].name));
            write(1, "\n", 1);
        }
        else
        {
            char *str = ft_itoa_hex(symbols[i].address); 
            if (!str)
            {
                write(2, MALLOC_FAIL, ft_strlen(MALLOC_FAIL));
                cleanup_file(file);
                exit(1);
            }

            int addr_len = (file->arch == ARCH_32BIT) ? 8 : 16;
            int str_len  = ft_strlen(str);

            char zeros[17] = {0}; 

            for (int j = 0; j < addr_len - str_len; j++)
                zeros[j] = '0';

            zeros[addr_len - str_len] = '\0';

            write(1, zeros, ft_strlen(zeros));
            write(1, str, str_len);
            write(1, " ", 1);
            write(1, type, 1);
            write(1, " ", 1);
            write(1, symbols[i].name, ft_strlen(symbols[i].name));
            write(1, "\n", 1);

            free(str);
        }
    }
}

