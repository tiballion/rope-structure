#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rope.h"
#include "string_utils.h"

#define MAX_LENGTH_ROPE 4

void createRopeStructure(Rope **node, Rope *par, char a[], int l, int r) {
    Rope *tmp = (Rope*)malloc(sizeof(Rope));
    tmp->left = tmp->right = NULL;

    // Nous mettons la moitié des nœuds dans le sous-arbre gauche
    tmp->parent = par;

    // Si la longueur de la chaîne est plus grande
    if ((r - l) > MAX_LENGTH_ROPE) {
        tmp->str = NULL;
        tmp->lCount = (r - l) / 2;
        *node = tmp;
        int m = (l + r) / 2;
        createRopeStructure(&((*node)->left), *node, a, l, m);
        createRopeStructure(&((*node)->right), *node, a, m + 1, r);
    } else {
        *node = tmp;
        tmp->lCount = (r - l);
        int j = 0;
        tmp->str = (char*)malloc(MAX_LENGTH_ROPE * sizeof(char));
        for (int i = l; i <= r; i++)
            tmp->str[j++] = a[i];
    }
}

void concatenate(Rope **root3, Rope *root1, Rope *root2, int n1) {
    // Crée un nouveau nœud Rope et fait de root1 et root2 ses enfants.
    Rope *tmp = (Rope*)malloc(sizeof(Rope));
    tmp->parent = NULL;
    tmp->left = root1;
    tmp->right = root2;
    root1->parent = root2->parent = tmp;
    tmp->lCount = n1;

    // Rend la chaîne de tmp vide et met à jour la référence root3
    tmp->str = NULL;
    *root3 = tmp;
}

// Fonction qui imprime la chaîne (feuilles)
void printString(Rope *r) {
    if (r == NULL)
        return;
    if (r->left == NULL && r->right == NULL)
        printf("%s", r->str);
    printString(r->left);
    printString(r->right);
}

// Fonction pour afficher la représentation de la Rope
void printRopeRepresentation(Rope *root, int depth) {
    if (root != NULL) {
        // Affiche l'indentation basée sur la profondeur
        for (int i = 0; i < depth; ++i) {
            printf("  ");
        }

        // Affiche les informations du nœud
        if (root->str != NULL) {
            printf("Leaf: \"%s\"\n", root->str);
        } else {
            printf("Internal Node: Count=%d\n", root->lCount);
        }

        // Appelle récursivement la fonction pour les sous-arbres gauche et droit
        printRopeRepresentation(root->left, depth + 1);
        printRopeRepresentation(root->right, depth + 1);
    }
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

// Fonction pour générer le code DOT de la représentation de la Rope
void generateDotCode(Rope *root, FILE *dotFile) {
    if (root != NULL) {
        // Affiche les informations du nœud dans le fichier DOT
        fprintf(dotFile, "node%p [label=\"", root);
        if (root->str != NULL) {
            fprintf(dotFile, "%s", root->str);
        } else {
            fprintf(dotFile, "Count=%d", root->lCount);
        }
        fprintf(dotFile, "\"];\n");

        // Appelle récursivement la fonction pour les sous-arbres gauche et droit
        if (root->left != NULL) {
            fprintf(dotFile, "node%p -> node%p [label=\"left\"];\n", root, root->left);
            generateDotCode(root->left, dotFile);
        }
        if (root->right != NULL) {
            fprintf(dotFile, "node%p -> node%p [label=\"right\"];\n", root, root->right);
            generateDotCode(root->right, dotFile);
        }
    }
}


















/*
#define MAX_LENGTH_ROPE 4

Rope *rope_new(char *string) {
    printf("rope_new\n");
    int len = strlen(string);
    if(len <= MAX_LENGTH_ROPE) {
        Rope *rope = malloc(sizeof(Rope));
        rope->string = malloc(sizeof(char) * (len + 1));
        strcpy(rope->string, "");
        rope->length = len;
        rope->left = NULL;
        rope->right = NULL;
        return rope;
    }
    else {
        printf("sa crash la oeoe\n");
        Rope *rope = malloc(sizeof(Rope));
        //rope->string[1] = '\0'; // SA MARCHE PAS
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
}*/