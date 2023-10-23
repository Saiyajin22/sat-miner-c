#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

// #include <openssl/sha.h>

// Function to compute SHA-256 hash
// void sha256(const unsigned char *data, size_t data_len, char output[65])
// {
//       SHA256_CTX sha256;
//       SHA256_Init(&sha256);
//       SHA256_Update(&sha256, data, data_len);
//       SHA256_Final((unsigned char *)output, &sha256);
//       output[64] = '\0'; // Null-terminate the string
// }

struct BlockHeader
{
      int version;
      char* prevBlockHash;
      char* merkleRoot;
      long timeStamp;
      char* bits; // Shortened representation of the target difficulty
      long nonce;
};

char *concatenateBlockHeader(const struct BlockHeader *block)
{
      // Calculate the total length needed for the concatenated string
      size_t totalLength = snprintf(NULL, 0, "%d%s%s%ld%s%ld",
                                    block->version, block->prevBlockHash, block->merkleRoot, block->timeStamp, block->bits, block->nonce);

      // Allocate memory for the concatenated string
      char *concatenated = (char *)malloc(totalLength + 1); // +1 for the null-terminator

      if (concatenated == NULL)
      {
            printf("memory allocation problem");
            // Handle memory allocation error
            return NULL;
      }

      // Use snprintf to create the concatenated string
      snprintf(concatenated, totalLength + 1, "%d%s%s%ld%s%ld",
               block->version, block->prevBlockHash, block->merkleRoot, block->timeStamp, block->bits, block->nonce);

      printf("ss: %s", concatenated);

      return concatenated;
}

char* getHeaderString(const struct BlockHeader block){
      char versionStr[20];
      printf("header: %d\n", block.version);
      sprintf(versionStr, "%d", block.version);
      printf("version as str: %s", versionStr);

      return "s";
}

// int getSizeForStringByInt(int num)
// {
//       return (int)malloc(sizeof(char) * ceil(log10(num)));
// }

// int getSizeForStringByLong(long num)
// {
//       return (int)malloc(sizeof(char) * ceil(log10(num)));
// }

// char *toStringBlockHeader(struct BlockHeader blockheader)
// {
//       char versionStr[2];
//       sprintf(versionStr, "%d", blockheader.version);

//       // char timestampStr[getSizeForStringByLong(blockheader.timeStamp)];
//       // sprintf(timestampStr, "%ld", blockheader.timeStamp);

//       printf("version= %d", versionStr);
// }

int main()
{
      struct BlockHeader blockHeader;

      // Initialize the fields of the struct
      blockHeader.version = 1;
      strcpy(blockHeader.prevBlockHash, "000000000001b2cdb438f6324a9311fae34aceff519333d1d11164ddaa87a409");
      strcpy(blockHeader.merkleRoot, "9ac2659ba7ad885813586c2f47e3c3ad0987b31f974c8669a130ae753a43495c");
      blockHeader.timeStamp = 1293883796;
      strcpy(blockHeader.bits, "1fffffff");
      blockHeader.nonce = 0;

      printf("headeer.prevhash: %s\n", blockHeader.prevBlockHash);

      getHeaderString(blockHeader);
      // char *blockHeaderStr = concatenateBlockHeader(&blockHeader);
      // printf("concat: %s", blockHeaderStr);

      // long nce = -1;
      // while (nce < 600)
      // {
      //       int flag = 0;
      //       nce++;

      //       blockHeader.nonce = nce;

      //       char sha256hex[65];
      //       sha256((unsigned char *)input, input_len, sha256_hash);

      //       if (sha256hex[0] == '0' && sha256hex[1] == '0')
      //       {
      //             flag = 1;
      //       }

      //       assert(flag == 0);
      // }

      return 0;
}