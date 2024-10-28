#include "ft_ssl.h"

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

char get_result(t_ssl *ssl)
{
    const char *list_cmd[3] = {"md5", "sha256", NULL};
    hash_func hash_functions[3] = {md5, sha256, NULL};

    int i = 0;
    while (list_cmd[i] && strcmp(list_cmd[i], ssl->command) != 0)
        i++;
    if (hash_functions[i])
        return hash_functions[i](ssl);
    return 1;
}

char    process_ssl(t_ssl *ssl)
{
    if (get_result(ssl))
        return (1);
    print_ssl_result(ssl);
    return (0);
}

void free_and_nullify(char **ptr) {
    if (*ptr) {
        free(*ptr);
        *ptr = NULL;
    }
}

void free_ssl_file_array(t_ssl_file *files) {
    if (files) {
        for (int i = 0; files[i].name; i++) {
            free(files[i].content);
            files[i].content = NULL;
        }
        free(files);
    }
}

void free_ssl(t_ssl *ssl)
{
    free_ssl_file_array(ssl->files);
    ssl->files = NULL;
    free_ssl_file_array(ssl->result.file);
    ssl->result.file = NULL;
    free_and_nullify(&ssl->stdin_content);
    free_and_nullify(&ssl->result.stdin_content);
    free_and_nullify(&ssl->result.sum);
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
