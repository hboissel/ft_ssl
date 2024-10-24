#include "ft_ssl.h"

void print_ssl(t_ssl *ssl)
{
    //print header
    printf("*** SSL ***\n");
    printf("Command: %s\n", ssl->command);
    printf("Quiet: %d\n", ssl->quiet);
    printf("Append: %d\n", ssl->append);
    printf("Reverse: %d\n", ssl->reverse);
    printf("Sum: %s\n", ssl->sum);
    if (ssl->files)
    {
        printf("Files:\n");
        for (int i = 0; ssl->files[i].name; i++)
        {
            printf("\tFile: %s\n", ssl->files[i].name);
            printf("\tContent: %s\n", ssl->files[i].content);
        }
    } else {
        printf("Files: NULL\n");
    }
    printf("Stdin content: %s\n", ssl->stdin_content);
    printf("Result:\n");
    if (ssl->result.stdin_content)
    {
        printf("\tStdin result content: %s\n", ssl->result.stdin_content);
    }
    if (ssl->result.sum)
    {
        printf("\tSum result: %s\n", ssl->result.sum);
    }
    if (ssl->result.file)
    {
        printf("\tFiles result:\n");
        for (int i = 0; ssl->result.file[i].name; i++)
        {
            printf("\t\tFile: %s\n", ssl->result.file[i].name);
            printf("\t\tContent: %s\n", ssl->result.file[i].content);
            printf("\t\tError: %d\n", ssl->result.file[i].error);
        }
    }
}

void init_ssl_result(t_ssl_result *ssl_result)
{
    ssl_result->stdin_content = NULL;
    ssl_result->sum = NULL;
    ssl_result->file = NULL;
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
    init_ssl_result(&ssl->result);
}

char    get_result(t_ssl *ssl)
{
    char *list_cmd[3] = {"md5", "sha256", NULL};
    int i = 0;
    while (list_cmd[i] && strcmp(list_cmd[i], ssl->command) != 0)
    {
        i++;
    }
    switch (i)
    {
        case 0:
            return md5(ssl);
        case 1:
            return sha256(ssl);
        default:
            return (1);
    }
}

char    process_ssl(t_ssl *ssl)
{
    if (get_result(ssl))
        return (1);
    print_ssl_result(ssl);
    return (0);
}

void free_ssl(t_ssl *ssl)
{
    if (ssl->files)
    {
        for (int i = 0; ssl->files[i].name; i++)
        {
            free(ssl->files[i].content);
        }
        free(ssl->files);
    }
    free(ssl->stdin_content);
    if (ssl->result.stdin_content)
        free(ssl->result.stdin_content);
    if (ssl->result.sum)
        free(ssl->result.sum);
    if (ssl->result.file)
    {
        for (int i = 0; ssl->result.file[i].name; i++)
        {
            free(ssl->result.file[i].content);
        }
        free(ssl->result.file);
    }
}

char ft_ssl(int argc, char **argv)
{
    t_ssl ssl;

    init_ssl(&ssl);
    if (parsing(&ssl, argc, argv))
        return (1);
    if (process_ssl(&ssl))
    {
        free_ssl(&ssl);
        return (1);
    }
    free_ssl(&ssl);
    return (0);
}