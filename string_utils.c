#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void stringcopy(char *destination, const char *source) {
    while ((*destination++ = *source++) != '\0');
}

size_t stringlen(char *string) {
    size_t i = 0;
    while (string[i] != '\0') {
        i++;
    }
    return i;
}

void splitString(const char *input, char *firstHalf, char *secondHalf) {
    size_t length = stringlen(input);
    size_t halfLength = length / 2;

    // Copy the first half of the string to firstHalf
    strncpy(firstHalf, input, halfLength);
    firstHalf[halfLength] = '\0';  // Null-terminate the string

    // Copy the second half of the string to secondHalf
    strcpy(secondHalf, input + halfLength);
}