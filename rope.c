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

    // Si la longueur de la chaîne est plus grande que MAX_LENGTH_ROPE
    if (currentLength > MAX_LENGTH_ROPE) {
        tmp->str = NULL;
        tmp->lCount = currentLength / 2;
        *node = tmp;
        int m = (l + r) / 2;

        // Ajuste la longueur de gauche si la longueur totale est impaire
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
        // Si le nœud est une feuille, retourne la longueur de la chaîne
        return strlen(root->str);
    } else {
        // Sinon, c'est un nœud interne, retourne la somme des longueurs des sous-arbres
        return rope_len(root->left) + rope_len(root->right);
    }

    freeRope(root);
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

char getCharAtIndex(Rope *root, int index) {
    if (root == NULL || index < 0 || index >= rope_len(root)) {
        // Gérer les cas d'erreur, par exemple, retourner un caractère spécial ou signaler une erreur.
        return '\0';
    }

    if (root->str != NULL) {
        // Si le nœud est une feuille, renvoyer le caractère à l'index spécifié.
        return root->str[index];
    } else {
        // Sinon, déterminer dans quel sous-arbre se trouve l'index.
        if (index < root->lCount) {
            // L'index est dans le sous-arbre gauche.
            return getCharAtIndex(root->left, index);
        } else {
            // L'index est dans le sous-arbre droit.
            return getCharAtIndex(root->right, index - root->lCount);
        }
    }

    freeRope(root);
}

void insertAtIndex(Rope **root, int index, const char *insertStr) {
    if (index < 0 || index > rope_len(*root)) {
        // Gérer les cas d'erreur, par exemple, signaler une erreur.
        printf("Index out of bounds.\n");
        return;
    }

    // Crée une nouvelle corde pour la chaîne à insérer
    Rope *insertedNode = NULL;
    createRopeStructure(&insertedNode, NULL, insertStr, 0, strlen(insertStr) - 1);

    // Si la corde est vide, remplace la corde vide par la nouvelle corde à insérer
    if (*root == NULL) {
        *root = insertedNode;
        return;
    }

    // Cas spécial : insertion au début de la corde
    if (index == 0) {
        // Crée une nouvelle corde pour la corde existante
        Rope *existingNode = NULL;
        createRopeStructure(&existingNode, NULL, "", 0, 0);

        // Met la corde existante à droite de la nouvelle corde insérée
        insertedNode->right = existingNode;

        // Met la nouvelle corde insérée comme racine
        *root = insertedNode;
        return;
    }

    // Cas général : divise la corde existante à l'index spécifié
    Rope *leftSubtree = NULL;
    Rope *rightSubtree = NULL;
    splitRope(*root, index, &leftSubtree, &rightSubtree);

    // Le nœud interne créé lors de l'insertion doit avoir le lCount ajusté
    insertedNode->lCount = rope_len(leftSubtree);

    // Concatène la corde gauche avec la nouvelle corde insérée
    Rope *newRoot = concatenateRopes(leftSubtree, insertedNode);

    // Concatène la nouvelle corde insérée avec la corde droite
    *root = concatenateRopes(newRoot, rightSubtree);
}

void splitRope(Rope *root, int index, Rope **left, Rope **right) {
    if (root == NULL) {
        *left = *right = NULL;
        return;
    }

    if (index == 0) {
        // L'index est à la première position, donc la corde de gauche est vide
        *left = NULL;
        *right = root;
    } else if (index >= rope_len(root)) {
        // L'index est à la dernière position ou au-delà, donc la corde de droite est vide
        *left = root;
        *right = NULL;
    } else if (root->str != NULL) {
        // La corde est une feuille, divise la chaîne à l'index spécifié
        *left = (Rope *)malloc(sizeof(Rope));
        (*left)->left = (*left)->right = NULL;
        (*left)->str = strndup(root->str, index);
        (*left)->lCount = strlen((*left)->str);

        *right = (Rope *)malloc(sizeof(Rope));
        (*right)->left = (*right)->right = NULL;
        (*right)->str = strndup(root->str + index, strlen(root->str) - index);
        (*right)->lCount = strlen((*right)->str);
    } else {
        // La corde est un nœud interne, détermine dans quel sous-arbre se trouve l'index
        if (index <= root->lCount) {
            // L'index est dans le sous-arbre gauche
            *right = root;
            splitRope(root->left, index, left, &((*right)->left));
            (*right)->lCount -= rope_len(*left);  // Ajustement du lCount
        } else {
            // L'index est dans le sous-arbre droit
            *left = root;
            splitRope(root->right, index - root->lCount, &((*left)->right), right);
            (*left)->lCount -= rope_len(*right);  // Ajustement du lCount
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

    concatenated->str = NULL; // La corde concaténée est un nœud interne sans chaîne associée

    return concatenated;

    freeRope(left);
    freeRope(right);
}

void deleteAtIndex(Rope **root, int index, int length) {
    if (*root == NULL || index < 0 || length <= 0 || index + length > rope_len(*root)) {
        // Gérer les cas d'erreur, par exemple, signaler une erreur.
        printf("Invalid index or length for deletion.\n");
        return;
    }

    // Cas spécial : suppression de caractères au début
    if (index == 0) {
        Rope *newRoot = NULL;
        splitRope(*root, length, &newRoot, root);
        freeRope(newRoot);
        return;
    }

    // Cas général : diviser la corde à l'index spécifié
    Rope *leftSubtree = NULL;
    Rope *rightSubtree = NULL;
    splitRope(*root, index, &leftSubtree, &rightSubtree);

    // Diviser la corde de droite pour obtenir la plage de caractères à supprimer
    Rope *deletedChars = NULL;
    splitRope(rightSubtree, length, &deletedChars, &rightSubtree);

    // Concaténer les deux parties restantes
    Rope *newRoot = concatenateRopes(leftSubtree, rightSubtree);

    // Libérer la mémoire des caractères supprimés
    freeRope(deletedChars);

    *root = newRoot;
}