#include "../inc/nm.h"

void sort_symbol(Symbol *symbols, File *file, int size, int arch)
{
    int i, j;
    int swapped;
    
    for (i = 0; i < size; i++)
    {
        if (symbols[i].name)
        {
            const char *name = symbols[i].name;
            while (*name == '_' || *name == '.')
                name++;
            
            if (name != symbols[i].name)
                symbols[i].trim_name = ft_strdup(name);
            else
                symbols[i].trim_name = ft_strdup(symbols[i].name);
            
            if (!symbols[i].trim_name)
            {
                for (int j = i - 1; i >= 0; i--)
                    free(symbols[j].trim_name);

                write(2, MALLOC_FAIL, ft_strlen(MALLOC_FAIL));
                cleanup_file(file);
                exit(1);
            }
        }
    }

    for (i = 0; i < size - 1; i++)
    {
        swapped = 0;
        
        for (j = 0; j < size - i - 1; j++)
        {
            
            char *name1 = symbols[j].trim_name;
            char *name2 = symbols[j + 1].trim_name;

            if (!symbols[j + 1].name)
            {
                Symbol tmp     = symbols[j];
                symbols[j]     = symbols[j + 1];
                symbols[j + 1] = tmp;
                swapped        = 1;
                continue;
            }
            
            if (!symbols[j].name)
                continue;

            int cmp = ft_strcasecmp(name1, name2);
            
            if (cmp == 0)
            {
                int l = 0;
                int sum_x = 0;
                int sum_y = 0;

                while (symbols[j].name[l++] == '_')
                    sum_x++;

                l = 0;

                while (symbols[j + 1].name[l++] == '_')
                    sum_y++;

                if (sum_x < sum_y)
                {
                    Symbol tmp     = symbols[j];
                    symbols[j]     = symbols[j + 1];
                    symbols[j + 1] = tmp;
                    swapped        = 1;
                    continue;
                }
            }

            if (cmp > 0) 
            {
                Symbol tmp     = symbols[j];
                symbols[j]     = symbols[j + 1];
                symbols[j + 1] = tmp;
                swapped        = 1;
            }
        }

        if (swapped == 0) 
            break;
    }

    for (i = 0; i < size; i++)
    {
        if (symbols[i].trim_name)
        {
            free(symbols[i].trim_name);
            symbols[i].trim_name = NULL;
        }
    }
}


int ft_strcasecmp(const char *s1, const char *s2)
{
    while (*s1 && ft_tolower(((unsigned char)*s1)) == ft_tolower(((unsigned char)*s2)))
    {
        s1++;
        s2++;
    }

    if (*s1 == '_' && *s2 != '_')
            return 1;

    if (*s2 == '_' && *s1 != '_')
            return -1;

    return (ft_tolower(((unsigned char)*s1)) - ft_tolower(((unsigned char)*s2)));
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