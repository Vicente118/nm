#include "../inc/nm.h"

int main(int argc, char **argv)
{
    File    file;

    if (argc == 1)
    {
        if (no_args(&file, A_OUT) == -1)
            return 1;
    }
    
        
    return 0;
}