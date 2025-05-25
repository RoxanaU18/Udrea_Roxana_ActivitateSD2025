#include <stdio.h>
#include <stdlib.h>

typedef struct Nod {
    int valoare;
    struct Nod* st;
    struct Nod* dr;
} Nod;

Nod* creeazaNod(int val) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->valoare = val;
    nou->st = nou->dr = NULL;
    return nou;
}

Nod* inserare(Nod* radacina, int val) {
    if (radacina == NULL) {
        return creeazaNod(val);
    }

    if (val < radacina->valoare)
        radacina->st = inserare(radacina->st, val);
    else
        radacina->dr = inserare(radacina->dr, val);

    return radacina;
}

void inordine(Nod* radacina) {
    if (radacina) {
        inordine(radacina->st);
        printf("%d ", radacina->valoare);
        inordine(radacina->dr);
    }
}

int main() {
    Nod* arbore = NULL;

    arbore = inserare(arbore, 5);
    arbore = inserare(arbore, 3);
    arbore = inserare(arbore, 7);
    arbore = inserare(arbore, 2);
    arbore = inserare(arbore, 4);
    arbore = inserare(arbore, 6);
    arbore = inserare(arbore, 8);

    printf("Afisare inordine: ");
    inordine(arbore);

    return 0;
}
