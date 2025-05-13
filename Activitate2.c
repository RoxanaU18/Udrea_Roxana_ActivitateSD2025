//1. Sa se construiasca si sa se afiseze o lista simpla cu n noduri continand informatii despre agentiile unei banci.
//Informatiile aferente nodurilor sunt citite dintr - un fisier text, 
//iar fiecare agentie ar trebui sa fie identificata prin cod(int), denumire(char*)*, profit(float), 
//precum si alte campuri la alegere(adresa, numar de clienti, etc).
//
//2. Sa se scrie functia pentru stergerea agentiilor cu coduri impare din lista simpla cu n agentii ale unei banci, 
//iar agentiile cu coduri pare se vor salva intr - un fisier text.
//
//3. Sa se realizeze conversia elementelor din lista simpla anterioara intr - o structura de tip lista de liste 
//prin gruparea elementelor initiale dupa un criteriu la alegere.
//
//4. Sa se dezaloce toate structurile de date dinamice construite.Operatia de dezalocare 
//va fi insotita de afisarea continutului structurilor in fisiere text, inainte de dezalocarea memoriei heap.


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Structura Agentie
typedef struct Agentie {
    int cod;
    char* denumire;
    float profit;
    char* adresa;
    int nrClienti;
} Agentie;

// Lista simpla
typedef struct Nod {
    Agentie ag;
    struct Nod* next;
} Nod;

// Lista de liste
typedef struct NodSublista {
    Agentie ag;
    struct NodSublista* next;
} NodSublista;

typedef struct NodLista {
    int prag; // grupa: 0 <50, 1 = 50–99, 2 >=100
    NodSublista* sublista;
    struct NodLista* next;
} NodLista;

// Citire lista simpla din fisier
Nod* citesteListaDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        perror("Eroare la deschiderea fisierului");
        return NULL;
    }

    Nod* cap = NULL;
    char linie[256];

    while (fgets(linie, sizeof(linie), f)) {
        Agentie a;
        char denumire[100], adresa[100];

        if (sscanf(linie, "%d,%99[^,],%f,%99[^,],%d",
            &a.cod, denumire, &a.profit, adresa, &a.nrClienti) == 5) {

            a.denumire = _strdup(denumire);
            a.adresa = _strdup(adresa);

            Nod* nou = (Nod*)malloc(sizeof(Nod));
            nou->ag = a;
            nou->next = cap;
            cap = nou;
        }
    }

    fclose(f);
    return cap;
}

// Afisare lista
void afiseazaLista(Nod* cap) {
    while (cap) {
        printf("Cod: %d | Denumire: %s | Profit: %.2f | Adresa: %s | Nr clienti: %d\n",
            cap->ag.cod, cap->ag.denumire, cap->ag.profit,
            cap->ag.adresa, cap->ag.nrClienti);
        cap = cap->next;
    }
}

void stergeImpareSiSalveazaPare(Nod** cap, const char* numeFisier) {
    FILE* f = fopen(numeFisier, "w");
    if (!f) {
        perror("Eroare la scrierea fisierului");
        return;
    }

    Nod* curent = *cap;
    Nod* anterior = NULL;

    while (curent) {
        if (curent->ag.cod % 2 == 0) {
            fprintf(f, "%d,%s,%.2f,%s,%d\n",
                curent->ag.cod, curent->ag.denumire,
                curent->ag.profit, curent->ag.adresa,
                curent->ag.nrClienti);

            anterior = curent;
            curent = curent->next;
        }
        else {
            Nod* deSters = curent;
            if (curent == *cap) {
                *cap = curent->next;
                curent = *cap;
            }
            else {
                anterior->next = curent->next;
                curent = curent->next;
            }

            free(deSters->ag.denumire);
            free(deSters->ag.adresa);
            free(deSters);
        }
    }

    fclose(f);
}

int getGrupa(int nrClienti) {
    if (nrClienti < 50) return 0;
    if (nrClienti < 100) return 1;
    return 2;
}

void adaugaInListaDeListe(NodLista** capLista, Agentie ag) {
    int grupa = getGrupa(ag.nrClienti);
    NodLista* p = *capLista;

    while (p && p->prag != grupa)
        p = p->next;

    if (!p) {
        NodLista* grupNou = (NodLista*)malloc(sizeof(NodLista));
        grupNou->prag = grupa;
        grupNou->sublista = NULL;
        grupNou->next = *capLista;
        *capLista = grupNou;
        p = grupNou;
    }


    NodSublista* nou = (NodSublista*)malloc(sizeof(NodSublista));
    nou->ag.cod = ag.cod;
    nou->ag.profit = ag.profit;
    nou->ag.nrClienti = ag.nrClienti;
    nou->ag.denumire = _strdup(ag.denumire);
    nou->ag.adresa = _strdup(ag.adresa);

    nou->next = p->sublista;
    p->sublista = nou;
}

NodLista* transformaInListaDeListe(Nod* cap) {
    NodLista* capLista = NULL;
    while (cap) {
        adaugaInListaDeListe(&capLista, cap->ag);
        cap = cap->next;
    }
    return capLista;
}

void afiseazaListaDeListe(NodLista* cap) {
    while (cap) {
        printf("\nGrupa %d:\n", cap->prag);
        NodSublista* p = cap->sublista;
        while (p) {
            printf("  Cod: %d | Denumire: %s | Profit: %.2f | Adresa: %s | Nr clienti: %d\n",
                p->ag.cod, p->ag.denumire, p->ag.profit,
                p->ag.adresa, p->ag.nrClienti);
            p = p->next;
        }
        cap = cap->next;
    }
}

void salveazaSiElibereazaLista(Nod** cap, const char* numeFisier) {
    FILE* f = fopen(numeFisier, "w");
    if (!f) return;

    Nod* p = *cap;
    while (p) {
        fprintf(f, "%d,%s,%.2f,%s,%d\n",
            p->ag.cod, p->ag.denumire, p->ag.profit,
            p->ag.adresa, p->ag.nrClienti);

        free(p->ag.denumire);
        free(p->ag.adresa);
        Nod* temp = p;
        p = p->next;
        free(temp);
    }

    fclose(f);
    *cap = NULL;
}

void salveazaSiElibereazaListaDeListe(NodLista** cap, const char* numeFisier) {
    FILE* f = fopen(numeFisier, "w");
    if (!f) return;

    NodLista* pl = *cap;
    while (pl) {
        fprintf(f, "Grupa %d:\n", pl->prag);
        NodSublista* ps = pl->sublista;
        while (ps) {
            fprintf(f, "  %d,%s,%.2f,%s,%d\n",
                ps->ag.cod, ps->ag.denumire, ps->ag.profit,
                ps->ag.adresa, ps->ag.nrClienti);

            free(ps->ag.denumire);
            free(ps->ag.adresa);
            NodSublista* tempS = ps;
            ps = ps->next;
            free(tempS);
        }

        NodLista* tempL = pl;
        pl = pl->next;
        free(tempL);
    }

    fclose(f);
    *cap = NULL;
}


int main() {
    Nod* lista = citesteListaDinFisier("agentii.txt");

    printf("Citire din fisier...\n\nLista agentiilor bancare:\n");
    afiseazaLista(lista);

    stergeImpareSiSalveazaPare(&lista, "agentii_pare.txt");

    printf("\nLista dupa stergere (doar coduri pare):\n");
    afiseazaLista(lista);

    NodLista* listaDeListe = transformaInListaDeListe(lista);

    printf("\nLista de liste (grupata dupa nr. clienti):\n");
    afiseazaListaDeListe(listaDeListe);

    salveazaSiElibereazaLista(&lista, "lista_finala.txt");
    salveazaSiElibereazaListaDeListe(&listaDeListe, "lista_de_liste_finala.txt");

    printf("\nToate structurile au fost eliberate si salvate in fisiere.\n");

    return 0;
}

