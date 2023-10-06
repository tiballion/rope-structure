#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rope.h"

int main() {
    // Crée un arbre Rope pour la première chaîne
    Rope *root1 = NULL;
    char a[] = "111122223333";
    int n1 = sizeof(a) / sizeof(a[0]) - 1; // Exclut le caractère nul
    createRopeStructure(&root1, NULL, a, 0, n1 - 1);

    // Génère le fichier DOT
    FILE *dotFile = fopen("rope_graph.dot", "w");
    fprintf(dotFile, "digraph Rope {\n");
    generateDotCode(root1, dotFile);
    fprintf(dotFile, "}\n");
    fclose(dotFile);

    return 0;
}