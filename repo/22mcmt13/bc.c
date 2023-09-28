#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to convert a decimal number to another base
void decimalToBase(int decimalNum, int base) {
    char res[32]; // Assumes a maximum of 32 characters for the result
    int index = 0;

    while (decimalNum > 0) {
        int rem = decimalNum % base;
        res[index++] = (rem < 10) ? (rem + '0') : (rem - 10 + 'A');
        decimalNum /= base;
    }

    if (index == 0) {
        res[index++] = '0'; // Handle the case when the input is 0
    }

    res[index] = '\0';

    // Reverse the result string
    for (int i = 0, j = index - 1; i < j; i++, j--) {
        char temp = res[i];
        res[i] = res[j];
        res[j] = temp;
    }

    printf("Result in base %d: %s\n", base, res);
}

int main() {
    int decimalNum, base;

    printf("Enter a decimal number: ");
    scanf("%d", &decimalNum);

    printf("Enter the base to convert (2 for binary, 16 for hexadecimal): ");
    scanf("%d", &base);

    if (base < 2 || base > 36) {
        printf("Invalid base.  enter a base between 2 and 36.\n");
        return 1;
    }

    decimalToBase(decimalNum, base);

    return 0;
}
