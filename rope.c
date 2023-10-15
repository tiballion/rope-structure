#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rope.h"
#include "string_utils.h"

#define MAX_LENGTH_ROPE 4

void createRopeStructure(Rope **node, Rope *par, char a[], int l, int r) {
    Rope *tmp = (Rope*)malloc(sizeof(Rope));
    tmp->left = tmp->right = NULL;

    // Calcul de la longueur de la chaîne actuelle
    int currentLength = r - l + 1;

    if (currentLength > MAX_LENGTH_ROPE) {
        // Si la chaine est trop longue, on la divise en deux
        tmp->str = NULL;
        tmp->lCount = currentLength / 2;
        *node = tmp;
        int m = (l + r) / 2;

        // On ajuste la longueur
        if ((currentLength % 2 == 1) && (tmp->lCount + l == r - tmp->lCount)) {
            tmp->lCount++;
        }

        createRopeStructure(&((*node)->left), *node, a, l, l + tmp->lCount - 1);
        createRopeStructure(&((*node)->right), *node, a, l + tmp->lCount, r);
    } else {
        *node = tmp;
        tmp->lCount = currentLength;
        tmp->str = (char*)malloc(currentLength * sizeof(char));
        for (int i = l; i <= r; i++)
            tmp->str[i - l] = a[i];
    }
}

void freeRope(Rope *root) {
    if (root != NULL) {
        freeRope(root->left);
        freeRope(root->right);
        free(root->str);
        free(root);
    }
}

int rope_len(Rope *root) {
    if (root == NULL) {
        return 0;
    }

    if (root->str != NULL) {
        // retourne la taille de la chaîne si c'est une feuille
        return strlen(root->str);
    } else {
        // retourne la somme des tailles des sous-arbres
        return rope_len(root->left) + rope_len(root->right);
    }

    freeRope(root);
}


void printString(Rope *r) {
    // print le string dans la rope
    if (r == NULL)
        return;
    if (r->left == NULL && r->right == NULL)
        printf("%s", r->str);
    printString(r->left);
    printString(r->right);
}

// Affichage de la rope
void printRopeRepresentation(Rope *root, int depth) {
    if (root != NULL) {
        // Indentation
        for (int i = 0; i < depth; ++i) {
            printf("  ");
        }

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

// génération du graphe
void generateDotCode(Rope *root, FILE *dotFile) {
    if (root != NULL) {
        fprintf(dotFile, "node%p [label=\"", root);
        if (root->str != NULL) {
            fprintf(dotFile, "%s", root->str);
        } else {
            fprintf(dotFile, "Count=%d", root->lCount);
        }
        fprintf(dotFile, "\"];\n");

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

char getCharAtIndex(Rope *root, int index) {
    if (root == NULL || index < 0 || index >= rope_len(root)) {
        // gérer cas erreurs
        return '\0';
    }

    if (root->str != NULL) {
        return root->str[index];
    } else {
        if (index < root->lCount) {
            // gauche
            return getCharAtIndex(root->left, index);
        } else {
            // droite
            return getCharAtIndex(root->right, index - root->lCount);
        }
    }

    freeRope(root);
}

void insertAtIndex(Rope **root, int index, const char *insertStr) {
    if (index < 0 || index > rope_len(*root)) {
        printf("Index out of bounds.\n");
        return;
    }

    Rope *insertedNode = NULL;
    createRopeStructure(&insertedNode, NULL, insertStr, 0, strlen(insertStr) - 1);

    if (*root == NULL) {
        *root = insertedNode;
        return;
    }
    
    if (index == 0) {
        Rope *existingNode = NULL;
        createRopeStructure(&existingNode, NULL, "", 0, 0);
        insertedNode->right = existingNode;
        *root = insertedNode;
        return;
    }

    Rope *leftSubtree = NULL;
    Rope *rightSubtree = NULL;
    splitRope(*root, index, &leftSubtree, &rightSubtree);

    insertedNode->lCount = rope_len(leftSubtree);

    Rope *newRoot = concatenateRopes(leftSubtree, insertedNode);

    *root = concatenateRopes(newRoot, rightSubtree);
}

void splitRope(Rope *root, int index, Rope **left, Rope **right) {
    if (root == NULL) {
        *left = *right = NULL;
        return;
    }

    if (index == 0) {
        *left = NULL;
        *right = root;
    } else if (index >= rope_len(root)) {
        *left = root;
        *right = NULL;
    } else if (root->str != NULL) {
        *left = (Rope *)malloc(sizeof(Rope));
        (*left)->left = (*left)->right = NULL;
        (*left)->str = strndup(root->str, index);
        (*left)->lCount = strlen((*left)->str);

        *right = (Rope *)malloc(sizeof(Rope));
        (*right)->left = (*right)->right = NULL;
        (*right)->str = strndup(root->str + index, strlen(root->str) - index);
        (*right)->lCount = strlen((*right)->str);
    } else {
        if (index <= root->lCount) {
            *right = root;
            splitRope(root->left, index, left, &((*right)->left));
            (*right)->lCount -= rope_len(*left);
        } else {
            *left = root;
            splitRope(root->right, index - root->lCount, &((*left)->right), right);
            (*left)->lCount -= rope_len(*right);
        }
    }
    
}

Rope *concatenateRopes(Rope *left, Rope *right) {
    Rope *concatenated = (Rope *)malloc(sizeof(Rope));
    concatenated->left = left;
    concatenated->right = right;

    if (left != NULL) {
        concatenated->lCount = rope_len(left);
    }else{
        concatenated->lCount = 0;
    }

    concatenated->str = NULL;

    return concatenated;

    freeRope(left);
    freeRope(right);
}

void deleteAtIndex(Rope **root, int index, int length) {
    if (*root == NULL || index < 0 || length <= 0 || index + length > rope_len(*root)) {
        printf("Invalid index or length for deletion.\n");
        return;
    }

    // caractères au début
    if (index == 0) {
        Rope *newRoot = NULL;
        splitRope(*root, length, &newRoot, root);
        freeRope(newRoot);
        return;
    }

    // on divise la rope
    Rope *leftSubtree = NULL;
    Rope *rightSubtree = NULL;
    splitRope(*root, index, &leftSubtree, &rightSubtree);

    Rope *deletedChars = NULL;
    splitRope(rightSubtree, length, &deletedChars, &rightSubtree);

    Rope *newRoot = concatenateRopes(leftSubtree, rightSubtree);

    // libere la mémoire
    freeRope(deletedChars);

    *root = newRoot;
}