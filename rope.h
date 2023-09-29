#ifndef ROPE_H
#define ROPE_H

typedef
struct Rope
{
    char *string;
    int length;
    Rope *left;
    Rope *right;
}Rope;

Rope *rope_new(char *string);
int rope_len(Rope *rope);
void rope_insert_at(Rope *rope, int index, char *string);
void rope_delete(Rope *rope);

#endif // ROPE_H