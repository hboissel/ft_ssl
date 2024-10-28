#include "ft_ssl.h"

char hash_and_store(char *message, char **destination, hash_func_process process_hash, const unsigned hash_size)
{
    char hash[hash_size + 1];

    if (process_hash(message, hash))
        return 1;
    *destination = strdup(hash);
    if (!destination)
        return 1;
    return 0;
}

char hash_and_store_files(t_ssl *ssl, hash_func_process process_hash, const unsigned hash_size)
{
    int num_files = get_num_files(ssl->files);

    ssl->result.file = (t_ssl_file *)malloc((num_files + 1) * sizeof(t_ssl_file));
    if (!ssl->result.file)
        return (1);
    for (int i = 0; ssl->files[i].name; i++)
    {
        ssl->result.file[i].name = ssl->files[i].name;
        if (ssl->files[i].error)
        {
            ssl->result.file[i].content = strdup(ssl->files[i].content);
            if (!ssl->result.file[i].content)
                return (1);
            ssl->result.file[i].error = 1;
            continue;
        }
        if (hash_and_store(ssl->files[i].content, &ssl->result.file[i].content, process_hash, hash_size))
            return (1);
        ssl->result.file[i].error = 0;
    }
    ssl->result.file[num_files].content = NULL;
    ssl->result.file[num_files].name = NULL;
    ssl->result.file[num_files].error = 0;
    return 0;
}

char hash_prep(t_ssl *ssl, hash_func_process process_hash, const unsigned hash_size)
{
    if (ssl->stdin_content && hash_and_store(ssl->stdin_content, &ssl->result.stdin_content, process_hash, hash_size))
        return print_ssl_erno(ssl);
    if (ssl->sum && hash_and_store(ssl->sum, &ssl->result.sum, process_hash, hash_size))
        return print_ssl_erno(ssl);
    if (ssl->files && hash_and_store_files(ssl, process_hash, hash_size))
        return print_ssl_erno(ssl);
    return 0;
}

void add_block_len_little_endian(const uint64_t *numBlocks, const uint64_t bitLen, uint8_t *blocks)
{
    for (int i = 0; i < 8; i++)
        blocks[(*numBlocks * 64) - 8 + i] = (uint8_t)(bitLen >> (i * 8));
}

void add_block_len_big_endian(const uint64_t *numBlocks, const uint64_t bitLen, uint8_t *blocks)
{
    for (int i = 0; i < 8; i++)
        blocks[(*numBlocks * 64) - 8 + i] = (uint8_t)(bitLen >> (56 - i * 8));
}

char convert_str_to_512bits_blocks(const char *msg, uint32_t **blocks, uint64_t *numBlocks, char bigEndian)
{
    uint64_t msgLen = strlen(msg);
    uint64_t bitLen = msgLen * 8;

    *numBlocks = (msgLen + 8) / 64 + 1;
    *blocks = (uint32_t *)malloc(*numBlocks * 16 * sizeof(uint32_t));
    if (*blocks == NULL)
        return (1);
    memset(*blocks, 0, *numBlocks * 16 * sizeof(uint32_t));
    for (uint64_t i = 0; i < msgLen; i++)
        ((uint8_t *)*blocks)[i] = (uint8_t)msg[i];
    ((uint8_t *)*blocks)[msgLen] = 0x80;
    uint64_t paddingStart = msgLen + 1;
    while (paddingStart < (*numBlocks * 64) - 8)
        ((uint8_t *)*blocks)[paddingStart++] = 0;
    if (bigEndian)
        add_block_len_big_endian(numBlocks, bitLen, (uint8_t *)*blocks);
    else
        add_block_len_little_endian(numBlocks, bitLen, (uint8_t *)*blocks);
    return (0);
}

void convert_hash_to_hex(uint32_t *H, char *hash, unsigned num_hash_words, unsigned hash_output_size) {
    const char *hex_chars = "0123456789abcdef";
    for (unsigned i = 0; i < num_hash_words; i++) {
        for (int j = 0; j < 4; j++) {
            unsigned char byte = (H[i] >> (j * 8)) & 0xff;
            hash[i * 8 + j * 2] = hex_chars[byte >> 4];
            hash[i * 8 + j * 2 + 1] = hex_chars[byte & 0x0F];
        }
    }
    hash[hash_output_size] = '\0';
}
