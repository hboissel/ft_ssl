#ifndef FT_SSL_H
# define FT_SSL_H

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <ctype.h>
# include <errno.h>
# include <stdint.h>

typedef struct s_ssl_file {
    char *name;
    char *content;
    char error;
} t_ssl_file;

typedef struct s_ssl_result {
    char *stdin_content;
    char *sum;
    t_ssl_file *file;
} t_ssl_result;

typedef struct s_ssl {
    char *command;
    char quiet;
    char append;
    char reverse;
    char *sum;
    t_ssl_file *files;
    char *stdin_content;
    t_ssl_result result;
} t_ssl;

typedef int (*hash_func)(const char *message, char *hash);

char    ft_ssl(int argc, char **argv);
char    parsing(t_ssl *ssl, int argc, char **argv);
void    print_help(void);
char    *get_stdin_content(void);
char    md5(t_ssl *ssl);
char    sha256(t_ssl *ssl);
char    *get_file_content(char *path);
void    print_upper(char *str);
void    print_ssl_result(t_ssl *ssl);
int     get_num_files(t_ssl_file *files);
void    print_ssl(t_ssl *ssl);
char    print_ssl_erno(t_ssl *ssl);
char    hash_prep(t_ssl *ssl, hash_func process_hash, const unsigned hash_size);

// Function for testing
void fill_with_fake_hashes(t_ssl *ssl, int num_files);
void print_ssl(t_ssl *ssl);

#endif
