#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rope.h"

int main() {
    char example_string[] = "Hello, Rope!";
    Rope *my_rope = rope_new(example_string);

    free(my_rope);
    return 0;
}