#include "ft_ssl.h"

char    sha256(t_ssl *ssl)
{
    int nb_files;

    nb_files = 0;
    while (ssl->files && ssl->files[nb_files].name)
        nb_files++;
    fill_with_fake_hashes(ssl, nb_files);
    return (0);
}