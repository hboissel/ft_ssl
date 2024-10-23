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
    char    buffer[1024];
    int     len;
    int     total_len;

    total_len = 0;
    content = NULL;
    while ((len = read(0, buffer, 1024)) > 0)
    {
        content = realloc(content, total_len + len + 1);
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
    }
    content[total_len] = '\0';
    return (content);
}