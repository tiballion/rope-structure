#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t stringlen(char *string) {
    return sizeof(string) / sizeof(string[0]) - 1;
}