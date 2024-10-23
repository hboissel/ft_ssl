#include "ft_ssl.h"

/// Function which checks the command argument
/// Parameters:
///    - ssl: a pointer to the t_ssl structure
///    - cmd: a pointer to the first argument of the command
/// Return:
///    - 0 if valid
///    - 1 if invalid
char check_command(t_ssl *ssl, char *cmd)
{
    char *commands[] = {"md5", "sha256", NULL};
    if (!cmd)
    {
        printf("usage: ft_ssl command [flags] [file/string]\n");
        return (1);
    }
    for (int i = 0; commands[i]; i++)
    {
        if (strcmp(commands[i], cmd) == 0)
        {
            ssl->command = cmd;
            return (0);
        }
    }
    printf("ft_ssl: Error: '%s' is an invalid command.\n\n", cmd);
    print_help();
    return (1);
}

/// Function which parses the flags
/// Parameters:
///   - ssl: a pointer to the t_ssl structure
///   - argc: the number of arguments
///   - argv: an array of arguments
///   - *i: a pointer to the current index of the argument
/// Return:
///   - 0 if valid
///   - 1 if invalid
char    parse_flags(t_ssl *ssl, int argc, char **argv, int *i)
{
    char *flags[] = {"-q", "-r", "-p", "-s", NULL};
    if (!argv || !argc || !i)
    {
        return (0);
    }
    while (*i < argc && argv[*i] && argv[*i][0] == '-')
    {
        int j = 0;
        while (flags[j] && strcmp(flags[j], argv[*i]) != 0)
        {
            j++;
        }
        switch (j)
        {
            case 0:
                ssl->quiet = 1;
                break;
            case 1:
                ssl->reverse = 1;
                break;
            case 2:
                ssl->append = 1;
                break;
            case 3:
                if (!argv[*i + 1])
                {
                    printf("ft_ssl: %s: %s: Flag requires an argument.\n", ssl->command, flags[j]);
                    return (1);
                }
                ssl->sum = argv[*i + 1];
                (*i) += 2;
                return (0);
            default:
                printf("ft_ssl: %s: %s: Invalid flag\n", ssl->command, argv[*i]);
                return (1);
        }
        (*i)++;
    }
    return (0);
}

/// Function which parses the flags and arguments (files/strings)
/// Parameters:
///   - ssl: a pointer to the t_ssl structure
///   - argc: the number of arguments
///   - argv: an array of arguments
/// Return:
///   - 0 if valid
///   - 1 if invalid
char parse_arguments(t_ssl *ssl, int argc, char **argv)
{
    if (!argv || !argc)
    {
        return (0);
    }
    int i = 0;
    if (parse_flags(ssl, argc, argv, &i))
        return (1);
    if (i >= argc)
    {
        return (0);
    }
    ssl->files = argv + i;
    return (0);
}

char parsing(t_ssl *ssl, int argc, char **argv)
{
    if (!argv || argc < 2)
    {
        printf("usage: ft_ssl command [flags] [file/string]\n");
        return (1);
    }
    argv++;
    if (check_command(ssl, *argv))
        return (1);
    argv++;
    argc -= 2;
    if (parse_arguments(ssl, argc, argv))
        return (1);
    ssl->stdin_content = get_stdin_content();
    if (!ssl->stdin_content)
    {
        printf("ft_ssl: Error: Failed to read from stdin\n");
        return (1);
    }
    return (0);
}