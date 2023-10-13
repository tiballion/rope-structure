#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rope.h"

int main() {
    // Création d'une corde avec une chaîne initiale
    Rope *root = NULL;
    char initialString[] = "ABCDEFGHIJ";
    createRopeStructure(&root, NULL, initialString, 0, strlen(initialString) - 1);

    // Affichage de la corde initiale
    printf("Initial Rope:\n");
    printRopeRepresentation(root, 0);
    printf("String in the Rope:\n");
    printString(root);
    printf("\n\n");

    // Test de la fonction d'insertion à un index spécifié
    int indexToInsert = 4;
    char stringToInsert[] = "O";
    printf("Inserting \"%s\" at index %d:\n", stringToInsert, indexToInsert);
    insertAtIndex(&root, indexToInsert, stringToInsert);

    // Affichage de la corde après l'insertion
    printf("Rope after insertion:\n");
    printRopeRepresentation(root, 0);
    printf("String in the Rope:\n");
    printString(root);
    printf("\n");

    // Test de la fonction de suppression à un index spécifié
    int indexToDelete = 6;
    int lengthToDelete = 3; // Longueur de la plage à supprimer
    printf("Deleting character at index %d:\n", indexToDelete);
    deleteAtIndex(&root, indexToDelete, lengthToDelete);

    // Affichage de la corde après la suppression
    printf("Rope after deletion:\n");
    printRopeRepresentation(root, 0);
    printf("String in the Rope:\n");
    printString(root);
    printf("\n");

    // Génère à nouveau le fichier DOT pour la corde mise à jour
    FILE *dotFile = fopen("rope_graph_after_insert.dot", "w");
    fprintf(dotFile, "digraph Rope {\n");
    generateDotCode(root, dotFile);
    fprintf(dotFile, "}\n");
    fclose(dotFile);

    // Libération de la mémoire allouée pour la corde
    freeRope(root);

    return 0;
}