#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int customStrlen(const char *str)
{
    int length = 0;

    while (str[length] != '\0')
    {
        length++;
    }

    return length;
}

const unsigned int MULTIPLIER = 23;

char* dummyHash(char* str, int L) {
    // Convert the string to a numerical form (unsigned long long).
    unsigned long long a = 0;
    // Here we use the ASCII table
    for(int i = 0; i < customStrlen(str); i++) {
        if(str[i] > 47 && str[i] < 58) {
            a += str[i] - '0';
        } else if(str[i] > 96 && str[i] < 123) {
            a += str[i] - 'a';
        }
    }
    // Multiply by a constant
    unsigned long long b = a * MULTIPLIER;

    // Convert the unsigned long long to a hexadecimal string
    char *c = (char *)malloc(L);
    sprintf(c, "%lld", b);
    
    if(customStrlen(c) > L) {
        char *res = (char *)malloc(L + 1);
        strncpy(res, c, L);
        res[L] = '\0';
        return res;
    } else if(customStrlen(c) < L) {
        char *padded = (char *)malloc(L + 1);
        strcpy(padded, c);
        for (int i = customStrlen(c); i < L; i++)
        {
            padded[i] = '0'; // Pad with zeros
        }
        padded[L] = '\0'; // Null-terminate the padded string
        return padded;
    }

    return c;
}


int main() {
    // 9ac2659ba7ad885813586c2f47e3c3ad0987b31f974c8669a130ae753a43495c
    char *hash = dummyHash("9ac2659ba7ad885813586c2f47e3c3ad0987b31f974c8669a130ae753a43495c\0", 6);
    printf("hash: %s\n", hash);

    return 0;
}