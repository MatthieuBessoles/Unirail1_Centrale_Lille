#include <stdio.h>

// Déclaration du tableau global
int liste_parcourt[50] = {43,31,50,23,52,32,50,33,29,53,11,19};/*à remplir avec le parcourt */

int main() {
    int i;

    // Affichage des éléments du tableau
    for (i = 0; i < 10; i++) {
        printf("%d\n", liste_parcourt[i]);
    }

    return 0;
}
