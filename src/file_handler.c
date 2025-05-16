#include "../inc/nm.h"

int    no_args(File *file, const char *filename)
{
    if (open(filename, O_DIR) >= 0)
    {
        write(2, NM_WARN, 15);
        write(2, filename, ft_strlen(filename));
        write(2, IS_DIR, 18);
        return -1;
    }

    file->fd = open(filename, O_RDONLY);

    if (file->fd == -1)
    {
        perror("nm: a.out");
        return -1;
    }

    if (fstat(file->fd, &file->sb) == -1)
    {
        perror("fstat");
        return -1;
    }
    return 0;
}