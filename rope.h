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
void concatenate(Rope **root3, Rope *root1, Rope *root2, int n1);
void printString(Rope *r);
void printRopeRepresentation(Rope *root, int depth);
void generateDotCode(Rope *root, FILE *dotFile);
int rope_len(Rope *rope);
void rope_insert_at(Rope *rope, int index, char *string);
void rope_delete(Rope *rope);

#endif // ROPE_H