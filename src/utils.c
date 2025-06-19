#include "../inc/nm.h"

void sort_symbol(Symbol *symbols, File *file, int size, int arch)
{
    int i, j;
    int sort;
    
    for (i = 0; i < size - 1; i++)
    {
        sort = 0;
        
        for (j = 0; j < size - i - 1; j++)
        {
            
            char *name1 = symbols[j].name;
            char *name2 = symbols[j + 1].name;

            if (!symbols[j + 1].name)
            {
                Symbol tmp     = symbols[j];
                symbols[j]     = symbols[j + 1];
                symbols[j + 1] = tmp;
                sort           = 1;
                continue;
            }
            
            if (!symbols[j].name)
                continue;

            int cmp = ft_strcasecmp(name1, name2);
            
            if (cmp == 0)
            {
                continue;
            }

            if (cmp > 0) 
            {
                Symbol tmp     = symbols[j];
                symbols[j]     = symbols[j + 1];
                symbols[j + 1] = tmp;
                sort        = 1;
            }
        }

        if (sort == 0) 
            break;
    }
}


int ft_strcasecmp(const char *s1, const char *s2)
{
    while (*s1 && (unsigned char)*s1 == (unsigned char)*s2) 
    {
        s1++;
        s2++;
    }

    return ((unsigned char)*s1 - (unsigned char)*s2) ;
}


int is_valid_string(const char *start, const char *end, size_t max_len) 
{
    if (!start || start >= end)
    return 0;
    
    const char  *ptr = start;
    size_t      len  = 0;

    while (ptr < end && *ptr != '\0') 
    {
        ptr++;
        len++;

        if (len >= max_len)
        return 0;
    }
    
    return (ptr < end && *ptr == '\0');
}

void    free_names(int size, Symbol *symbols)
{
    for (size_t i = 0; i < size; i++)
    {
        if (symbols[i].name)
        free(symbols[i].name);
    }
}

char *ft_itoa_hex(unsigned long n)
{
    char            *hex_chars = "0123456789abcdef";
    unsigned long   temp       = n;
    int             len        = 1;
    
    while (temp > 15)
    {
        temp = temp / 16;
        len++;
    }
    
    char *result = (char *)malloc(sizeof(char) * (len + 1));
    if (!result)
    return NULL;
    
    result[len] = '\0';
    
    while (len > 0)
    {
        result[--len] = hex_chars[n % 16];
        n = n / 16;
    }
    
    return result;
}