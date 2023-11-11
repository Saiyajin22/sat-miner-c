#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>

const unsigned long MAX_NONCE = 4294967295;

struct BlockHeader
{
    int version;
    char *prevBlockHash;
    char *merkleRoot;
    long timeStamp;
    char *bits; // Shortened representation of the target difficulty
    long nonce;
};

char *concatenateBlockHeader(const struct BlockHeader *block)
{
    size_t totalLength = snprintf(NULL, 0, "%d%s%s%ld%s%ld", block->version, block->prevBlockHash, block->merkleRoot, block->timeStamp, block->bits, block->nonce);

    char *concatenated = (char *)malloc(totalLength + 1);
    if (concatenated == NULL)
    {
        // Handle memory allocation error
        return NULL;
    }

    snprintf(concatenated, totalLength + 1, "%d%s%s%ld%s%ld", block->version, block->prevBlockHash, block->merkleRoot, block->timeStamp, block->bits, block->nonce);

    return concatenated;
}

// int getSizeByInt(int num)
// {
//     if (num == 0)
//         return 1;
//     return (int)((ceil(log10(num)) + 1) * sizeof(char));
// }

// int getSizeByLong(long num)
// {
//     if (num == 0)
//         return 1;
//     return (int)((ceil(log10(num)) + 1) * sizeof(char));
// }

// ------------------------------
// Start of SHA256 Implementation
// ------------------------------

// void processMessage(char *message)
// {
//     size_t lengthInBytes = strlen(message);
//     printf("length: %d\n", lengthInBytes);
//     size_t mod64 = lengthInBytes % 64;
//     size_t offset = (64 - mod64); // TODO -8 is missing !!!

//     for (size_t i = 0; i < offset; i++)
//     {
//         if (i == 0)
//         {
//             message[lengthInBytes + i] = '1';
//         }
//         else
//         {
//             message[lengthInBytes + i] = '0';
//         }
//     }

//     // TODO Message initialization is missing Length bits
//     printf("message: %s\n", message);
// }

// // Default buffer values - hard coded constants, representing hash values
// const unsigned int A = 0x6a09e667;
// const unsigned int B = 0xbb67ae85;
// const unsigned int C = 0x3c6ef372;
// const unsigned int D = 0xa54ff53a;
// const unsigned int E = 0x510e527f;
// const unsigned int F = 0x9b05688c;
// const unsigned int G = 0x1f83d9ab;
// const unsigned int H = 0x5be0cd19;

// const unsigned int k[64] = {
//     0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
//     0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
//     0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
//     0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
//     0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
//     0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
//     0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
//     0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
// };

// void compression(char *message) {
//     size_t numberOfChunks = strlen(message) / 64;
//     printf("numof chunks: %d\n", numberOfChunks);
//     for (size_t i = 0; i < numberOfChunks; i++)
//     {
//         char chunk[65];
//         for (size_t j = 0; j < 64; j++)
//         {
//             chunk[j] = message[(i * 64) + j];
//         }
//         chunk[64] = '\0';
//         printf("chunk %s\n", chunk);

//     }
// }
#define uchar unsigned char
#define uint unsigned int

#define DBL_INT_ADD(a, b, c)  \
    if (a > 0xffffffff - (c)) \
        ++b;                  \
    a += c;
#define ROTLEFT(a, b) (((a) << (b)) | ((a) >> (32 - (b))))
#define ROTRIGHT(a, b) (((a) >> (b)) | ((a) << (32 - (b))))

#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22))
#define EP1(x) (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25))
#define SIG0(x) (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10))

typedef struct
{
    uchar data[64];
    uint datalen;
    uint bitlen[2];
    uint state[8];
} SHA256_CTX;

uint k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

void SHA256Transform(SHA256_CTX *ctx, uchar data[])
{
    uint a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

    for (i = 0, j = 0; i < 16; ++i, j += 4)
        m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
    for (; i < 64; ++i)
        m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];
    e = ctx->state[4];
    f = ctx->state[5];
    g = ctx->state[6];
    h = ctx->state[7];

    for (i = 0; i < 64; ++i)
    {
        t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
        t2 = EP0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
    ctx->state[5] += f;
    ctx->state[6] += g;
    ctx->state[7] += h;
}

void SHA256Init(SHA256_CTX *ctx)
{
    ctx->datalen = 0;
    ctx->bitlen[0] = 0;
    ctx->bitlen[1] = 0;
    ctx->state[0] = 0x6a09e667;
    ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372;
    ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f;
    ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab;
    ctx->state[7] = 0x5be0cd19;
}

void SHA256Update(SHA256_CTX *ctx, uchar data[], uint len)
{
    for (uint i = 0; i < len; ++i)
    {
        ctx->data[ctx->datalen] = data[i];
        ctx->datalen++;
        if (ctx->datalen == 64)
        {
            SHA256Transform(ctx, ctx->data);
            DBL_INT_ADD(ctx->bitlen[0], ctx->bitlen[1], 512);
            ctx->datalen = 0;
        }
    }
}

void SHA256Final(SHA256_CTX *ctx, uchar hash[])
{
    uint i = ctx->datalen;

    if (ctx->datalen < 56)
    {
        ctx->data[i++] = 0x80;
        while (i < 56)
            ctx->data[i++] = 0x00;
    }
    else
    {
        ctx->data[i++] = 0x80;
        while (i < 64)
            ctx->data[i++] = 0x00;
        SHA256Transform(ctx, ctx->data);
        memset(ctx->data, 0, 56);
    }

    DBL_INT_ADD(ctx->bitlen[0], ctx->bitlen[1], ctx->datalen * 8);
    ctx->data[63] = ctx->bitlen[0];
    ctx->data[62] = ctx->bitlen[0] >> 8;
    ctx->data[61] = ctx->bitlen[0] >> 16;
    ctx->data[60] = ctx->bitlen[0] >> 24;
    ctx->data[59] = ctx->bitlen[1];
    ctx->data[58] = ctx->bitlen[1] >> 8;
    ctx->data[57] = ctx->bitlen[1] >> 16;
    ctx->data[56] = ctx->bitlen[1] >> 24;
    SHA256Transform(ctx, ctx->data);

    for (i = 0; i < 4; ++i)
    {
        hash[i] = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 4] = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 8] = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
    }
}

char *SHA256(char *data)
{
    int strLen = strlen(data);
    SHA256_CTX ctx;
    unsigned char hash[32];
    char *hashStr = malloc(65);
    strcpy(hashStr, "");

    SHA256Init(&ctx);
    SHA256Update(&ctx, data, strLen);
    SHA256Final(&ctx, hash);

    char s[3];
    for (int i = 0; i < 32; i++)
    {
        sprintf(s, "%02x", hash[i]);
        strcat(hashStr, s);
    }

    return hashStr;
}

// ----------------------------
// End of SHA256 Implementation
// ----------------------------

unsigned int nondet_uint(void);

int main()
{
    struct BlockHeader blockHeader;

    // Initialize the fields of the struct
    blockHeader.version = 1;
    blockHeader.prevBlockHash = "000000000001b2cdb438f6324a9311fae34aceff519333d1d11164ddaa87a409";
    blockHeader.merkleRoot = "9ac2659ba7ad885813586c2f47e3c3ad0987b31f974c8669a130ae753a43495c";
    blockHeader.timeStamp = 1293883796;
    blockHeader.bits = "1fffffff";
    blockHeader.nonce = 0;

    long nonce = 0;
    long counter = 0;
    while (counter < MAX_NONCE)
    {
        // TODO Non-det nonce pick 
        counter++;
        nonce = nondet_uint();
        blockHeader.nonce = nonce;
        char *headerStr = concatenateBlockHeader(&blockHeader);

        char *hash = SHA256(SHA256(headerStr));
        // char *hash = malloc(sizeof(char) * 64);
        // printf("headerStr: %s\n", headerStr);
        // if (nonce < MAX_NONCE/2)
        // {
        //     hash[0] = '0';
        //     hash[1] = '0';
        //     hash[2] = '2';
        //     hash[3] = '5';
        // }
// TODO Implement assuming leading zeros based on Jonathans code
#ifdef CBMC
        __CPROVER_assume(hash[0] == '0' && hash[1] == '0');
#endif

        int flag = 0;
        if (hash[0] == '0' && hash[1] == '0') // TODO Try opposite check based on Jonathan's pseudocode..
        {
            flag = 1;
            printf("headerStr: %s\n", headerStr);
            printf("hash: %s\n", hash);
            printf("nonce: %li\n", nonce);
        }

        assert(flag == 0);
    }

    return 0;
}