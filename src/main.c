#include "ft_ssl.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("usage: ft_ssl command [flags] [file/string]\n");
        print_help();
        return (0);
    }
    return (ft_ssl(argc, argv));
}