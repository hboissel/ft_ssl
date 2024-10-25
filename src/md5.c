#include "ft_ssl.h"

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))
#define F(B, C, D) ((B & C) | ((~B) & D))
#define G(B, C, D) ((B & D) | ((~D) & C))
#define H(B, C, D) (B ^ C ^ D)
#define I(B, C, D) (C ^ (B | (~D)))
#define GET_INDEX(i) (i < 16 ? i : (i < 32 ? (5 * i + 1) % 16 : (i < 48 ? (3 * i + 5) % 16 : (7 * i) % 16)))

#define MD5_HASH_SIZE 32
#define MD5_BLOCK_SIZE 64

static const uint32_t K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

static const uint32_t r[64] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

void combine(uint32_t *A, uint32_t *B, uint32_t *C, uint32_t *D, uint32_t M[], uint32_t i, uint32_t md5_index)
{
    uint32_t F_value;
    uint32_t A_prime = *D;
    uint32_t B_prime;
    uint32_t C_prime = *B;
    uint32_t D_prime = *C;

    if (i < 16)
        F_value = F(*B, *C, *D);
    else if (i < 32)
        F_value = G(*B, *C, *D);
    else if (i < 48)
        F_value = H(*B, *C, *D);
    else
        F_value = I(*B, *C, *D);

    B_prime = LEFTROTATE((F_value + *A + M[md5_index] + K[i]), r[i]) + *B;

    *A = A_prime;
    *B = B_prime;
    *C = C_prime;
    *D = D_prime;
}

void process_block(uint32_t state[4], uint32_t M[16])
{
    uint32_t A = state[0], B = state[1], C = state[2], D = state[3];
    uint32_t md5_index = 0;

    for (uint32_t i = 0; i < MD5_BLOCK_SIZE; i++)
    {
        md5_index = GET_INDEX(i);
        combine(&A, &B, &C, &D, M, i, md5_index);
    }

    state[0] += A;
    state[1] += B;
    state[2] += C;
    state[3] += D;
}

// Function to convert a string to a list of 512-bit blocks
char convert_to_proper_512_bits_blocks(const char *msg, uint32_t **blocks, uint64_t *numBlocks)
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
    while (paddingStart < (*numBlocks * 64))
        ((uint8_t *)*blocks)[paddingStart++] = 0;
    for (int i = 0; i < 8; i++)
        ((uint8_t *)*blocks)[(*numBlocks * 64) - 8 + i] = (uint8_t)(bitLen >> (i * 8));
    return (0);
}

int md5_process(const char *msg, char hash[33])
{
    uint32_t state[4];
    uint32_t *blocks;
    uint64_t numBlocks;

    if (convert_to_proper_512_bits_blocks(msg, &blocks, &numBlocks))
        return (1);

    state[0] = 0x67452301; // A
    state[1] = 0xefcdab89; // B
    state[2] = 0x98badcfe; // C
    state[3] = 0x10325476; // D

    for (uint64_t i = 0; i < numBlocks; i++)
    {
        process_block(state, &blocks[i * 16]);
    }

    free(blocks);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            unsigned char byte = (state[i] >> (j * 8)) & 0xff;
            hash[i * 8 + j * 2] = "0123456789abcdef"[byte >> 4];
            hash[i * 8 + j * 2 + 1] = "0123456789abcdef"[byte & 0x0F];
        }
    }
    hash[32] = '\0';
    return (0);
}

char md5(t_ssl *ssl)
{
    if (hash_prep(ssl, md5_process, MD5_HASH_SIZE))
        return (1);
    return (0);
}
