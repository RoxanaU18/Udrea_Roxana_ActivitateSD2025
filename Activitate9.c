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
    nou->st = NULL;
    nou->dr = NULL;
    return nou;
}

Nod* inserarePeStanga(Nod* parinte, int val) {
    if (!parinte) return creeazaNod(val);
    parinte->st = inserarePeStanga(parinte->st, val);
    return parinte;
}

Nod* inserarePeDreapta(Nod* parinte, int val) {
    if (!parinte) return creeazaNod(val);
    parinte->dr = inserarePeDreapta(parinte->dr, val);
    return parinte;
}

void afisarePreordine(Nod* rad) {
    if (rad) {
        printf("%d ", rad->valoare);
        afisarePreordine(rad->st);
        afisarePreordine(rad->dr);
    }
}

int main() {
    Nod* rad = creeazaNod(1);
    rad = inserarePeStanga(rad, 2);
    rad = inserarePeDreapta(rad, 3);
    rad->st = inserarePeStanga(rad->st, 4);
    rad->dr = inserarePeDreapta(rad->dr, 5);

    printf("Afisare preordine: ");
    afisarePreordine(rad);

    return 0;
}
