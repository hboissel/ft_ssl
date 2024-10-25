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

// Function to fill the struct with fake data
void fill_with_fake_hashes(t_ssl *ssl, int num_files) {
    char *fake_hash = "1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef";
    t_ssl_file *files = ssl->files;
    t_ssl_result *result = &ssl->result;

    if (ssl->stdin_content) {
        result->stdin_content = (char *)malloc(strlen(fake_hash) * sizeof(char));
        strcpy(result->stdin_content, fake_hash);
    }
    if (ssl->sum) {
        result->sum = (char *)malloc(strlen(fake_hash) * sizeof(char));
        strcpy(result->sum, fake_hash);
    }
    if (!num_files) {
        return;
    }
    result->file = (t_ssl_file *)malloc((num_files + 1) * sizeof(t_ssl_file));
    for (int i = 0; i < num_files; i++) {
        result->file[i].name = files[i].name;
        if (files[i].error) {
            result->file[i].content = strdup(files[i].content);
            result->file[i].error = 1;
            continue;
        }
        result->file[i].content = (char *)malloc(strlen(fake_hash) * sizeof(char));
        strcpy(result->file[i].content, fake_hash);
        result->file[i].error = 0;
    }
    result->file[num_files].content = NULL;
    result->file[num_files].name = NULL;
    result->file[num_files].error = 0;
}
