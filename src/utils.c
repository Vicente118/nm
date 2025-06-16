#include "../inc/nm.h"

void    free_names(int size, Symbol *symbols)
{
    for (size_t i = 0; i < size; i++)
    {
        if (symbols[i].name)
            free(symbols[i].name);
    }
}