#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct Rezervare Rezervare;
struct Rezervare {
    unsigned int codRezervare;
    char* destinatie;
    char* orasPlecare;
    char* dataPlecare;
    unsigned int nrTuristi;
};

typedef struct NodLSC NodLSC;
struct NodLSC {
    Rezervare rezervare;
    NodLSC* next;
};

char* copiazaString(const char* str) {
    char* newStr = (char*)malloc(strlen(str) + 1);
    if (newStr)
        strcpy(newStr, str);
    return newStr;
}

NodLSC* creareNodLSC(Rezervare rezervare, NodLSC* next) {
    NodLSC* nodNou = (NodLSC*)malloc(sizeof(NodLSC));
    if (nodNou) {
        nodNou->rezervare = rezervare;
        nodNou->next = next;
    }
    return nodNou;
}

NodLSC* inserareLaSfarsit(NodLSC* cap, Rezervare rezervare) {
    NodLSC* nodNou = creareNodLSC(rezervare, cap);
    if (!cap) {
        nodNou->next = nodNou;
        return nodNou;
    }

    NodLSC* curent = cap;
    while (curent->next != cap) {
        curent = curent->next;
    }
    curent->next = nodNou;
    return cap;
}

NodLSC* creareListaCircularaDinFisier() {
    NodLSC* capLista = NULL;
    FILE* fisierRezervari = fopen("rezervari_data.txt", "r");
    if (!fisierRezervari) {
        printf("Eroare la deschiderea fisierului.\n");
        return NULL;
    }

    unsigned int nrRezervari;
    fscanf(fisierRezervari, "%u", &nrRezervari);

    char buffer[256];
    for (int i = 0; i < nrRezervari; ++i) {
        Rezervare rezervare;
        fscanf(fisierRezervari, "%u", &rezervare.codRezervare);
        fscanf(fisierRezervari, "%s", buffer);
        rezervare.destinatie = copiazaString(buffer);
        fscanf(fisierRezervari, "%s", buffer);
        rezervare.orasPlecare = copiazaString(buffer);
        fscanf(fisierRezervari, "%s", buffer);
        rezervare.dataPlecare = copiazaString(buffer);
        fscanf(fisierRezervari, "%u", &rezervare.nrTuristi);

        capLista = inserareLaSfarsit(capLista, rezervare);
    }

    fclose(fisierRezervari);
    return capLista;
}

void afisareRezervare(Rezervare rezervare) {
    printf("Cod rezervare: %u\n", rezervare.codRezervare);
    printf("Destinatie: %s\n", rezervare.destinatie);
    printf("Oras plecare: %s\n", rezervare.orasPlecare);
    printf("Data plecare: %s\n", rezervare.dataPlecare);
    printf("Nr turisti: %u\n\n", rezervare.nrTuristi);
}

void afisareListaCirculara(NodLSC* cap) {
    if (!cap) return;
    NodLSC* curent = cap;
    do {
        afisareRezervare(curent->rezervare);
        curent = curent->next;
    } while (curent != cap);
}

Rezervare getRezervareMaxima(NodLSC* cap) {
    Rezervare maxim = cap->rezervare;
    NodLSC* curent = cap->next;
    while (curent != cap) {
        if (curent->rezervare.nrTuristi > maxim.nrTuristi)
            maxim = curent->rezervare;
        curent = curent->next;
    }
    return maxim;
}

void actualizareDataPlecare(NodLSC* cap, char* dataNoua, unsigned int cod) {
    if (!cap) return;
    NodLSC* curent = cap;
    do {
        if (curent->rezervare.codRezervare == cod) {
            free(curent->rezervare.dataPlecare);
            curent->rezervare.dataPlecare = copiazaString(dataNoua);
        }
        curent = curent->next;
    } while (curent != cap);
}

char* getDestinatieMaxima(NodLSC* cap) {
    Rezervare maxRez = getRezervareMaxima(cap);
    return copiazaString(maxRez.destinatie);
}

unsigned int getLungimeLista(NodLSC* cap) {
    if (!cap) return 0;
    unsigned int lungime = 1;
    NodLSC* curent = cap->next;
    while (curent != cap) {
        ++lungime;
        curent = curent->next;
    }
    return lungime;
}

unsigned int* creazaVectorCoduri(NodLSC* cap, unsigned int* lungimeVector) {
    *lungimeVector = getLungimeLista(cap);
    unsigned int* vector = (unsigned int*)malloc(sizeof(unsigned int) * (*lungimeVector));
    if (!vector) return NULL;

    NodLSC* curent = cap;
    for (unsigned int i = 0; i < *lungimeVector; ++i) {
        vector[i] = curent->rezervare.codRezervare;
        curent = curent->next;
    }
    return vector;
}

void afisareVectorCoduri(unsigned int* vector, unsigned int lungime) {
    printf("Afisare coduri:\n");
    for (unsigned int i = 0; i < lungime; ++i) {
        printf("%u ", vector[i]);
    }
    printf("\n");
}

void dezalocareRezervare(Rezervare rezervare) {
    free(rezervare.destinatie);
    free(rezervare.orasPlecare);
    free(rezervare.dataPlecare);
}

void dezalocareNoduriRec(NodLSC* nod, NodLSC* cap) {
    if (nod == cap) return;
    dezalocareNoduriRec(nod->next, cap);
    dezalocareRezervare(nod->rezervare);
    free(nod);
}

void dezalocareListaCirculara(NodLSC* cap) {
    if (!cap) return;
    dezalocareRezervare(cap->rezervare);
    dezalocareNoduriRec(cap->next, cap);
    free(cap);
}

void dezalocareVector(unsigned int* vector) {
    free(vector);
}

int main() {
    NodLSC* capLista = creareListaCircularaDinFisier();
    if (!capLista) return 1;

    afisareListaCirculara(capLista);

    Rezervare rezervareMax = getRezervareMaxima(capLista);
    printf("Rezervare maxima:\n");
    afisareRezervare(rezervareMax);

    actualizareDataPlecare(capLista, "2025-09-15", 4567);
    afisareListaCirculara(capLista);

    char* destinatieMax = getDestinatieMaxima(capLista);
    printf("\nDestinatie maxima: %s\n", destinatieMax);
    free(destinatieMax);

    unsigned int lungime;
    unsigned int* coduri = creazaVectorCoduri(capLista, &lungime);
    if (coduri) {
        afisareVectorCoduri(coduri, lungime);
        dezalocareVector(coduri);
    }

    dezalocareListaCirculara(capLista);
    return 0;
}
