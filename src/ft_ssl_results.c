#include "ft_ssl.h"

void print_ssl_result_file_error(t_ssl *ssl, int i)
{
    printf("ft_ssl: %s: %s: %s\n", ssl->command, ssl->result.file[i].name, ssl->result.file[i].content);
}

void print_ssl_result_quiet(t_ssl *ssl)
{
    if (ssl->result.stdin_content)
    {
        if (ssl->append)
        {
            printf("%s", ssl->stdin_content);
            printf("%s\n", ssl->result.stdin_content);
        }
        else
            printf("%s\n", ssl->result.stdin_content);
    }
    if (ssl->result.sum)
        printf("%s\n", ssl->result.sum);
    if (ssl->result.file)
    {
        for (int i = 0; ssl->result.file[i].name; i++)
        {
            if (ssl->result.file[i].error)
            {
                print_ssl_result_file_error(ssl, i);
                continue;
            }
            printf("%s\n", ssl->result.file[i].content);
        }
    }
}

void print_ssl_result_stdin(t_ssl *ssl)
{
    if (!ssl->result.stdin_content)
        return;
    if (ssl->append)
    {
        if (ssl->stdin_content[strlen(ssl->stdin_content) - 1] == '\n')
            ssl->stdin_content[strlen(ssl->stdin_content) - 1] = '\0';
        printf("(\"%s\")= %s\n", ssl->stdin_content, ssl->result.stdin_content);
    }
    else if (ssl->reverse)
        printf("%s *stdin\n", ssl->result.stdin_content);
    else
        printf("(stdin)= %s\n", ssl->result.stdin_content);
}

void print_ssl_result_sum(t_ssl *ssl)
{
    if (!ssl->result.sum)
        return;
    if (ssl->reverse)
        printf("%s \"%s\"\n", ssl->result.sum, ssl->sum);
    else
    {
        print_upper(ssl->command);
        printf(" (\"%s\") = %s\n", ssl->sum, ssl->result.sum);
    }
}

void print_ssl_result_files(t_ssl *ssl)
{
    if (!ssl->result.file)
        return;

    for (int i = 0; ssl->result.file[i].name; i++)
    {
        if (ssl->result.file[i].error)
        {
            print_ssl_result_file_error(ssl, i);
            continue;
        }
        if (ssl->reverse)
            printf("%s %s\n", ssl->result.file[i].content, ssl->result.file[i].name);
        else
        {
            print_upper(ssl->command);
            printf(" (%s) = %s\n", ssl->result.file[i].name, ssl->result.file[i].content);
        }
    }
}

void print_ssl_result(t_ssl *ssl)
{
    if (ssl->quiet)
    {
        print_ssl_result_quiet(ssl);
        return;
    }
    print_ssl_result_stdin(ssl);
    print_ssl_result_sum(ssl);
    print_ssl_result_files(ssl);
}
