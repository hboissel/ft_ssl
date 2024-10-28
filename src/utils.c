#include "ft_ssl.h"

/// Function that print the help menu
void    print_help(void)
{
    printf("help:\n\n");
    printf("Message Digest commands\n");
    printf("md5\nsha256\n\n");
    printf("Flags\n");
    printf("-q        : quiet mode\n");
    printf("-r        : reverse the output\n");
    printf("-s STRING : print the sum of the given string\n");
    printf("-p        : echo STDIN to STDOUT and append the checksum to STDOUT\n");
}

/// Function that gets all the content from the stdin until a EOF
/// Return:
///    - a pointer to the content
char    *get_stdin_content(void)
{
    char    *content;
    char    *tmp;
    char    buffer[1024];
    int     len;
    int     total_len;

    total_len = 0;
    content = NULL;
    while ((len = read(0, buffer, 1024)) > 0)
    {
        tmp = realloc(content, total_len + len + 1);
        if (!tmp)
        {
            free(content);
            return (NULL);
        }
        content = tmp;
        memcpy(content + total_len, buffer, len);
        total_len += len;
    }
    if (len < 0)
    {
        free(content);
        return (NULL);
    } else if (len == 0)
    {
        tmp = realloc(content, total_len + 1);
        if (!tmp)
        {
            free(content);
            return (NULL);
        }
        content = tmp;
    }
    content[total_len] = '\0';
    return (content);
}

/// Function that from a path to a file, reads the content of the file
/// Parameters:
///     - path: the path to the file
/// Return:
///     - a pointer to the content of the file
char    *get_file_content(char *path)
{
    int     fd;
    char    *content;
    char    buffer[1024];
    int     len;
    int     total_len;

    if ((fd = open(path, O_RDONLY)) < 0)
        return (NULL);
    total_len = 0;
    content = NULL;
    while ((len = read(fd, buffer, 1024)) > 0)
    {
        content = realloc(content, total_len + len + 1);
        if (!content)
        {
            free(content);
            return (NULL);
        }
        memcpy(content + total_len, buffer, len);
        total_len += len;
    }
    if (len < 0)
    {
        free(content);
        return (NULL);
    } else if (len == 0)
    {
        content = realloc(content, total_len + 1);
        if (!content)
        {
            free(content);
            return (NULL);
        }
    }
    content[total_len] = '\0';
    close(fd);
    return (content);
}

/// Function that returns the number of files thanks to a t_ssl_file pointer
int    get_num_files(t_ssl_file *files)
{
    int i;

    if (!files)
        return (0);
    i = 0;
    while (files[i].name)
        i++;
    return (i);
}

// Function that print a string toupper
void    print_upper(char *str)
{
    for (int i = 0; str[i]; i++)
        printf("%c", toupper(str[i]));
}

/// Function that print error messages based on the t_ssl structure
char print_ssl_erno(t_ssl *ssl)
{
    printf("ft_ssl: %s: %s\n", ssl->command, strerror(errno));
    return (1);
}
