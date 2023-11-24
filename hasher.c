#include <stdio.h>
#include <stdlib.h>

// Function to convert the input to numerical form
unsigned long long convertToNumerical(const char *input)
{
    unsigned long long result = 0;
    while (*input)
    {
        result = result * 10 + (*input++ - '0');
    }
    return result;
}

// Function to multiply the result by a constant
unsigned long long multiplyByConstant(unsigned long long value, unsigned long long constant)
{
    return value * constant;
}

// Function to convert the result to hexadecimal and handle padding
void convertToHexAndPad(unsigned long long value, int desiredLength)
{
    char hexString[32]; // Assuming a maximum of 32 characters for the hexadecimal representation

    // Convert to hexadecimal
    snprintf(hexString, sizeof(hexString), "%llx", value);

    // Calculate the actual length of the hexadecimal representation
    int actualLength = snprintf(NULL, 0, "%llx", value);

    // Pad or cut the result based on the desired length
    if (actualLength < desiredLength)
    {
        printf("Hash: %0*llx\n", desiredLength, value);
    }
    else if (actualLength > desiredLength)
    {
        printf("Hash: %.*llx\n", desiredLength, value);
    }
    else
    {
        printf("Hash: %llx\n", value);
    }
}

int main()
{
    // Step a: Convert the input to numerical form
    const char *input = "HelloWorld";
    unsigned long long numericalValue = convertToNumerical(input);

    // Step b: Multiply the result by a constant
    unsigned long long constant = 13;
    unsigned long long multipliedValue = multiplyByConstant(numericalValue, constant);

    // Step c: Convert to hexadecimal and handle padding
    int desiredLength = 16; // Set your desired length
    convertToHexAndPad(multipliedValue, desiredLength);

    return 0;
}