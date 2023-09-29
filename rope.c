#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rope.h"

#define MAX_LENGTH_ROPE 4

Rope *rope_new(char *string) {
    printf("rope_new\n");
    int len = strlen(string);
    if(len <= MAX_LENGTH_ROPE) {
        Rope *rope = malloc(sizeof(Rope));
        rope->string = malloc(sizeof(char) * (len + 1));
        strcpy(rope->string, string);
        rope->length = len;
        rope->left = NULL;
        rope->right = NULL;
        return rope;
    }
    else {
        Rope *rope = malloc(sizeof(Rope));
        rope->string[1] = '\0';
        strcpy(rope->string, "");
        rope->length = MAX_LENGTH_ROPE;
        rope->left = rope_new(string);
        rope->right = rope_new(string + MAX_LENGTH_ROPE);
        return rope;
    }
    return NULL;
}

int rope_len(Rope *rope) {
    // Return the length of the rope
    return 0;
}

void rope_insert_at(Rope *rope, int index, char *string) {
    // Insert the string at the given index
}

void rope_delete(Rope *rope) {
    // Delete the rope
}