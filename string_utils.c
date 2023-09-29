#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void stringcopy(char *destination, const char *source) {
    while ((*destination++ = *source++) != '\0');
}