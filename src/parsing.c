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

int get_flag_index(char *arg, char **flags) {
    for (int j = 0; flags[j]; j++) {
        if (strcmp(flags[j], arg) == 0)
            return j;
    }
    return -1;
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
        return (0);
    while (*i < argc && argv[*i] && argv[*i][0] == '-')
    {
        int j = get_flag_index(argv[*i], flags);
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
    int i = 0;
    int nb_files = 0;
    if (!argv || !argc)
        return (0);
    if (parse_flags(ssl, argc, argv, &i))
        return (1);
    if (i >= argc)
        return (0);
    nb_files = argc - i;
    ssl->files = (t_ssl_file *)malloc((nb_files + 1) * sizeof(t_ssl_file));
    for (int j = 0; i < argc; i++, j++)
    {
        ssl->files[j].name = argv[i];
        ssl->files[j].content = NULL;
        ssl->files[j].error = 0;
    }
    ssl->files[nb_files].name = NULL;
    ssl->files[nb_files].content = NULL;
    ssl->files[nb_files].error = 0;
    return (0);
}

/// Function which get the content of the files
/// Parameters:
///   - ssl: a pointer to the t_ssl structure
/// Return:
///   - 0 if valid
///   - 1 if invalid
char get_files_content(t_ssl *ssl)
{
    int i = 0;
    while (ssl->files && ssl->files[i].name)
    {
        ssl->files[i].content = get_file_content(ssl->files[i].name);
        if (!ssl->files[i].content)
        {
            ssl->files[i].content = strdup(strerror(errno));
            if (!ssl->files[i].content)
                return print_ssl_erno(ssl);
            ssl->files[i].error = 1;
        }
        i++;
    }
    return (0);
}

char parse_stdin_content(t_ssl *ssl)
{
    if (!ssl->append && (ssl->sum || ssl->files))
        return (0);
    ssl->stdin_content = get_stdin_content();
    if (!ssl->stdin_content)
        return print_ssl_erno(ssl);
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
    if (parse_stdin_content(ssl))
        return (1);
    if (get_files_content(ssl))
        return (1);
    return (0);
}
