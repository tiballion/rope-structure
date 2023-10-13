#ifndef ROPE_H
#define ROPE_H

typedef
struct Rope
{
    struct Rope *left;
    struct Rope *right;
    struct Rope *parent;
    char *str;
    int lCount;
}Rope;

void createRopeStructure(Rope **node, Rope *par, char a[], int l, int r);
void freeRope(Rope *root);
int rope_len(Rope *root);
void printString(Rope *r);
void printRopeRepresentation(Rope *root, int depth);
void generateDotCode(Rope *root, FILE *dotFile);

char getCharAtIndex(Rope *root, int index);
void insertAtIndex(Rope **root, int index, const char *insertStr);
void splitRope(Rope *root, int index, Rope **left, Rope **right);
Rope *concatenateRopes(Rope *left, Rope *right);
void deleteAtIndex(Rope **root, int index, int length);

#endif // ROPE_H