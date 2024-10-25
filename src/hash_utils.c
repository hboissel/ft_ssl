#include "ft_ssl.h"

char hash_prep(t_ssl *ssl, hash_func process_hash, const unsigned hash_size)
{
    char *message;
    char hash[hash_size + 1];
    int num_files = get_num_files(ssl->files);

    if (ssl->stdin_content)
    {
        message = ssl->stdin_content;
        if (process_hash(message, hash))
            return print_ssl_erno(ssl);
        ssl->result.stdin_content = strdup(hash);
    }
    if (ssl->sum)
    {
        message = ssl->sum;
        if (process_hash(message, hash))
            return print_ssl_erno(ssl);
        ssl->result.sum = strdup(hash);
    }
    if (ssl->files)
    {
        ssl->result.file = (t_ssl_file *)malloc((num_files + 1) * sizeof(t_ssl_file));
        if (!ssl->result.file)
            return print_ssl_erno(ssl);
        for (int i = 0; ssl->files[i].name; i++)
        {
            ssl->result.file[i].name = ssl->files[i].name;
            if (ssl->files[i].error)
            {
                ssl->result.file[i].content = strdup(ssl->files[i].content);
                if (!ssl->result.file[i].content)
                    return print_ssl_erno(ssl);
                ssl->result.file[i].error = 1;
                continue;
            }
            message = ssl->files[i].content;
            if (process_hash(message, hash))
                return print_ssl_erno(ssl);
            ssl->result.file[i].content = strdup(hash);
            if (!ssl->result.file[i].content)
                return print_ssl_erno(ssl);
            ssl->result.file[i].error = 0;
        }
        ssl->result.file[num_files].content = NULL;
        ssl->result.file[num_files].name = NULL;
        ssl->result.file[num_files].error = 0;
    }
    return 0;
}