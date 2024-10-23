#ifndef FT_SSL_H
# define FT_SSL_H

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

typedef struct s_ssl {
    char *command;
    char quiet;
    char append;
    char reverse;
    char *sum;
    char **files;
    char *stdin_content;
} t_ssl;

char    ft_ssl(int argc, char **argv);
char    parsing(t_ssl *ssl, int argc, char **argv);
void    print_help(void);
char    *get_stdin_content(void);

#endif