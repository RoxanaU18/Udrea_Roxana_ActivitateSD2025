//1. Sa se realizeze o implementare de arbore binar de cautare echilibrat cu informatie utila de tip articol(din orice domeniu),
//care sa includa minim 3 atribute, dintre care minim unul de tip char* .Se vor implementa mecanismele de inserare cu date preluate
//din fisier text, traversari preordine, inordine, postordine, dezalocare. 
//
//2. Sa se implementeze functia pentru afisarea elementelor de pe un anumit nivel din arbore, iar apoi sa se traverseze arborele
//pe niveluri. 
//
//3. Sa se implementeze functia pentru determinarea elementelor din nodurile frunza ale arborelui si salvarea acestora
//intr - o alta structura de date. 
//
//4. Sa se stearga un nod din arbore pe baza atributului de tip char* din structura articol, care nu reprezinta cheia de inserare
//in arbore. 

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct exemplu {
    int cod;
    char* denumire;
    float pret;
    float cantitate;
};

struct nodArb {
    int BF;
    struct exemplu inf;
    struct nodArb* stanga;
    struct nodArb* dreapta;
};

int maxim(int a, int b) {
    return (a > b) ? a : b;
}

int nr_niveluri(struct nodArb* rad) {
    if (rad) {
        return 1 + maxim(nr_niveluri(rad->stanga), nr_niveluri(rad->dreapta));
    }
    return 0;
}

void calculBF(struct nodArb* rad) {
    if (rad) {
        rad->BF = nr_niveluri(rad->dreapta) - nr_niveluri(rad->stanga);
        calculBF(rad->stanga);
        calculBF(rad->dreapta);
    }
}

struct nodArb* creareNod(struct nodArb* stanga, struct nodArb* dreapta, struct exemplu p) {
    struct nodArb* nou = (struct nodArb*)malloc(sizeof(struct nodArb));
    nou->inf.cod = p.cod;
    nou->inf.denumire = (char*)malloc(strlen(p.denumire) + 1);
    strcpy(nou->inf.denumire, p.denumire);
    nou->inf.pret = p.pret;
    nou->inf.cantitate = p.cantitate;
    nou->stanga = stanga;
    nou->dreapta = dreapta;
    nou->BF = 0;
    return nou;
}

// ROTATII
struct nodArb* rotatie_dreapta(struct nodArb* rad) {
    struct nodArb* nod1 = rad->stanga;
    rad->stanga = nod1->dreapta;
    nod1->dreapta = rad;
    return nod1;
}

struct nodArb* rotatie_stanga(struct nodArb* rad) {
    struct nodArb* nod1 = rad->dreapta;
    rad->dreapta = nod1->stanga;
    nod1->stanga = rad;
    return nod1;
}

struct nodArb* rotatie_stanga_dreapta(struct nodArb* rad) {
    rad->stanga = rotatie_stanga(rad->stanga);
    return rotatie_dreapta(rad);
}

struct nodArb* rotatie_dreapta_stanga(struct nodArb* rad) {
    rad->dreapta = rotatie_dreapta(rad->dreapta);
    return rotatie_stanga(rad);
}

struct nodArb* reechilibrare(struct nodArb* rad) {
    calculBF(rad);
    if (rad->BF <= -2) {
        if (rad->stanga->BF <= 0)
            rad = rotatie_dreapta(rad);
        else
            rad = rotatie_stanga_dreapta(rad);
    }
    else if (rad->BF >= 2) {
        if (rad->dreapta->BF >= 0)
            rad = rotatie_stanga(rad);
        else
            rad = rotatie_dreapta_stanga(rad);
    }
    calculBF(rad);
    return rad;
}

struct nodArb* inserare(struct exemplu p, struct nodArb* rad) {
    if (!rad) return creareNod(NULL, NULL, p);
    if (p.cod < rad->inf.cod)
        rad->stanga = inserare(p, rad->stanga);
    else if (p.cod > rad->inf.cod)
        rad->dreapta = inserare(p, rad->dreapta);
    return reechilibrare(rad);
}

void traversare_inordine(struct nodArb* rad) {
    if (rad) {
        traversare_inordine(rad->stanga);
        printf("cod: %d, denumire: %s, pret: %.2f, cantitate: %.2f\n",
            rad->inf.cod, rad->inf.denumire, rad->inf.pret, rad->inf.cantitate);
        traversare_inordine(rad->dreapta);
    }
}

void dezalocare(struct nodArb* rad) {
    if (rad) {
        dezalocare(rad->stanga);
        dezalocare(rad->dreapta);
        free(rad->inf.denumire);
        free(rad);
    }
}

int main() {
    struct nodArb* rad = NULL;
    FILE* f = fopen("fisier.txt", "r");
    if (!f) {
        printf("Eroare deschidere fisier.\n");
        return 1;
    }

    int nr, i;
    fscanf(f, "%d", &nr);

    struct exemplu p;
    char buffer[100];

    for (i = 0; i < nr; i++) {
        fscanf(f, "%d %s %f %f", &p.cod, buffer, &p.pret, &p.cantitate);
        p.denumire = (char*)malloc(strlen(buffer) + 1);
        strcpy(p.denumire, buffer);
        rad = inserare(p, rad);
        free(p.denumire);
    }

    fclose(f);

    printf("----- Traversare inordine -----\n");
    traversare_inordine(rad);

    dezalocare(rad);
    return 0;
}

