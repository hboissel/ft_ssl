#include "ft_ssl.h"

void print_ssl(t_ssl *ssl)
{
    printf("Command: %s\n", ssl->command);
    printf("Quiet: %d\n", ssl->quiet);
    printf("Append: %d\n", ssl->append);
    printf("Reverse: %d\n", ssl->reverse);
    printf("Sum: %s\n", ssl->sum);
    if (ssl->files)
    {
        printf("Files: ");
        for (int i = 0; ssl->files[i]; i++)
        {
            printf("%s ", ssl->files[i]);
        }
        printf("\n");
    } else {
        printf("Files: NULL\n");
    }
    printf("Stdin content: %s\n", ssl->stdin_content);
}

void init_ssl(t_ssl *ssl)
{
    ssl->command = NULL;
    ssl->quiet = 0;
    ssl->append = 0;
    ssl->reverse = 0;
    ssl->sum = NULL;
    ssl->files = NULL;
    ssl->stdin_content = NULL;
}

char ft_ssl(int argc, char **argv)
{
    t_ssl ssl;

    init_ssl(&ssl);
    if (parsing(&ssl, argc, argv))
        return (1);
    print_ssl(&ssl);

    free(ssl.stdin_content);
    return (0);
}