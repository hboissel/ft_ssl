#include "ft_ssl.h"

#define ROTR(x, n) ((x >> n) | (x << (32 - n)))
#define CH(e,f,g) (((e) & (f)) ^ (~(e) & (g)))
#define MAJ(a,b,c) (((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c)))
#define SIG0(x) (ROTR(x,7) ^ ROTR(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTR(x,17) ^ ROTR(x,19) ^ ((x) >> 10))
#define BSIG0(x) (ROTR(x,2) ^ ROTR(x,13) ^ ROTR(x,22))
#define BSIG1(x) (ROTR(x,6) ^ ROTR(x,11) ^ ROTR(x,25))

#define SHA256_BLOCK_SIZE 32
#define SHA256_HASH_SIZE 64

// Constants for the compression function (K)
static const uint32_t K[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

// Initial hash values (H)
static const uint32_t H_initial[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

// SHA-256 compression function
void sha256_compress(uint32_t H[8], const uint32_t M[16], const uint32_t K[64])
{
    uint32_t W[64]; // Message schedule array
    uint32_t a, b, c, d, e, f, g, h;
    uint32_t T1, T2;

    // Step 1: Prepare the message schedule
    for (int t = 0; t < 16; t++)
        W[t] = (M[t] << 24) | ((M[t] << 8) & 0x00ff0000) | ((M[t] >> 8) & 0x0000ff00) | (M[t] >> 24);

    for (int t = 16; t < 64; t++)
        W[t] = SIG1(W[t - 2]) + W[t - 7] + SIG0(W[t - 15]) + W[t - 16];

    // Step 2: Initialize the working variables
    a = H[0];
    b = H[1];
    c = H[2];
    d = H[3];
    e = H[4];
    f = H[5];
    g = H[6];
    h = H[7];

    // Step 3: Perform the 64 rounds of the SHA-256 compression
    for (int t = 0; t < 64; t++)
    {
        T1 = h + BSIG1(e) + CH(e, f, g) + K[t] + W[t];
        T2 = BSIG0(a) + MAJ(a, b, c);

        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }

    // Step 4: Update the hash values
    H[0] += a;
    H[1] += b;
    H[2] += c;
    H[3] += d;
    H[4] += e;
    H[5] += f;
    H[6] += g;
    H[7] += h;

}

// Function to convert a string to a list of 512-bit (64-byte) blocks for SHA-256
char convert_to_512bit_blocks_sha256(const char *msg, uint32_t **blocks, uint64_t *numBlocks)
{
    uint64_t msgLen = strlen(msg);      // Length of the message in bytes
    uint64_t bitLen = msgLen * 8;       // Length of the message in bits

    // Calculate the number of 512-bit (64-byte) blocks required, including padding and length encoding
    *numBlocks = ((msgLen + 8) / 64) + 1;

    // Allocate memory for the blocks (each block is 64 bytes, i.e., 16 uint32_t words)
    *blocks = (uint32_t *)malloc(*numBlocks * 16 * sizeof(uint32_t));
    if (*blocks == NULL)
        return (1);  // Memory allocation failure

    // Initialize the blocks to zero
    memset(*blocks, 0, *numBlocks * 16 * sizeof(uint32_t));

    // Copy the message into the blocks
    for (uint64_t i = 0; i < msgLen; i++)
        ((uint8_t *)*blocks)[i] = (uint8_t)msg[i];

    // Append the '1' bit followed by zeros (0x80 is the binary representation for 10000000)
    ((uint8_t *)*blocks)[msgLen] = 0x80;

    // Pad with zeros until the last 8 bytes of the final block
    uint64_t paddingStart = msgLen + 1;
    while (paddingStart < (*numBlocks * 64) - 8)
        ((uint8_t *)*blocks)[paddingStart++] = 0;

    // Append the length of the original message as a 64-bit big-endian integer at the end
    for (int i = 0; i < 8; i++)
        ((uint8_t *)*blocks)[(*numBlocks * 64) - 8 + i] = (uint8_t)(bitLen >> (56 - 8 * i));

    return (0);  // Success
}

// Example usage
int sha256_process(const char *msg, char hash[65])
{
    uint32_t *M;
    uint64_t N;
    uint32_t H[8];

    for (int i = 0; i < 8; i++)
        H[i] = H_initial[i];
    if (convert_to_512bit_blocks_sha256(msg, &M, &N))
        return (1);
    for (uint64_t i = 0; i < N; i++)
        sha256_compress(H, M + (i * 16), K);
    // switch H to big-endian
    for (int i = 0; i < 8; i++)
        H[i] = (H[i] << 24) | ((H[i] << 8) & 0x00ff0000) | ((H[i] >> 8) & 0x0000ff00) | (H[i] >> 24);
    free(M);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            unsigned char byte = (H[i] >> (j * 8)) & 0xff;
            hash[i * 8 + j * 2] = "0123456789abcdef"[byte >> 4];
            hash[i * 8 + j * 2 + 1] = "0123456789abcdef"[byte & 0x0F];
        }
    }
    hash[64] = '\0';
    return 0;
}

char    sha256(t_ssl *ssl)
{
    if (hash_prep(ssl, sha256_process, SHA256_HASH_SIZE))
        return (1);
    return (0);
}
