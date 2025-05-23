#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>

// Structura DispozitivMobil si coada de prioritati

typedef struct DispozitivMobil {
	unsigned int idDispozitiv;
	char* sistemOperare;
	float pret;
	char* defecte;
	unsigned int nrZileInService;
} DispozitivMobil;

typedef struct CoadaPrioritati {
	DispozitivMobil** arr;
	unsigned int size;
} CoadaPrioritati;

typedef struct NodStiva {
	struct NodStiva* next;
	DispozitivMobil* dispozitiv;
} NodStiva;

char* copiazaString(const char* str) {
	if (!str) return NULL;
	char* newStr = (char*)malloc(strlen(str) + 1);
	strcpy(newStr, str);
	return newStr;
}

DispozitivMobil* citesteNextDispozitiv(FILE* fisier) {
	DispozitivMobil* dispozitiv = (DispozitivMobil*)malloc(sizeof(DispozitivMobil));
	char buffer[256];

	fscanf(fisier, "%u", &dispozitiv->idDispozitiv);
	fscanf(fisier, "%s", buffer);
	dispozitiv->sistemOperare = copiazaString(buffer);
	fscanf(fisier, "%f", &dispozitiv->pret);
	fscanf(fisier, "%s", buffer);
	dispozitiv->defecte = copiazaString(buffer);
	fscanf(fisier, "%u", &dispozitiv->nrZileInService);

	return dispozitiv;
}

unsigned int parinteHeap(unsigned int i) {
	return (i - 1) / 2;
}

unsigned int stangaHeap(unsigned int i) {
	return 2 * i + 1;
}

unsigned int dreaptaHeap(unsigned int i) {
	return 2 * i + 2;
}

int conditieHeapify(DispozitivMobil* rad, DispozitivMobil* cand) {
	return rad->nrZileInService < cand->nrZileInService;
}

void swap(DispozitivMobil** arr, unsigned int i, unsigned int j) {
	DispozitivMobil* temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}

void maxHeapify(CoadaPrioritati* coada, unsigned int i) {
	unsigned int st = stangaHeap(i), dr = dreaptaHeap(i), max = i;
	if (st < coada->size && conditieHeapify(coada->arr[max], coada->arr[st])) max = st;
	if (dr < coada->size && conditieHeapify(coada->arr[max], coada->arr[dr])) max = dr;
	if (max != i) {
		swap(coada->arr, i, max);
		maxHeapify(coada, max);
	}
}

CoadaPrioritati* creazaCoadaPrioritatiDinFisier(const char* numeFisier) {
	FILE* fisier = fopen(numeFisier, "r");
	if (!fisier) return NULL;

	unsigned int nr;
	fscanf(fisier, "%u", &nr);
	DispozitivMobil** arr = (DispozitivMobil**)malloc(nr * sizeof(DispozitivMobil*));
	for (unsigned int i = 0; i < nr; ++i) arr[i] = citesteNextDispozitiv(fisier);
	fclose(fisier);

	CoadaPrioritati* coada = (CoadaPrioritati*)malloc(sizeof(CoadaPrioritati));
	coada->arr = arr;
	coada->size = nr;
	for (int i = (int)(nr / 2) - 1; i >= 0; --i) maxHeapify(coada, i);
	return coada;
}

void afisareDispozitiv(const DispozitivMobil* d) {
	if (!d) return;
	printf("\nId: %u\nSistem: %s\nPret: %.2f\nDefecte: %s\nZile: %u\n",
		d->idDispozitiv, d->sistemOperare, d->pret, d->defecte, d->nrZileInService);
}

void afisareCoadaPrioritati(CoadaPrioritati* coada) {
	printf("\nDispozitive in coada: %u\n", coada->size);
	for (unsigned int i = 0; i < coada->size; ++i) afisareDispozitiv(coada->arr[i]);
}

unsigned int obtineNrDispozitiveDupaOs(CoadaPrioritati* coada, const char* os) {
	unsigned int count = 0;
	for (unsigned int i = 0; i < coada->size; ++i)
		if (strcmp(coada->arr[i]->sistemOperare, os) == 0) count++;
	return count;
}

void actualizeazaDefecteDispozitiv(CoadaPrioritati* coada, unsigned int id, const char* noiDefecte) {
	for (unsigned int i = 0; i < coada->size; ++i)
		if (coada->arr[i]->idDispozitiv == id) {
			free(coada->arr[i]->defecte);
			coada->arr[i]->defecte = copiazaString(noiDefecte);
			return;
		}
}

void dezalocareDispozitiv(DispozitivMobil* d) {
	free(d->sistemOperare);
	free(d->defecte);
	free(d);
}

void dezalocareHeap(CoadaPrioritati* coada) {
	for (unsigned int i = 0; i < coada->size; ++i) dezalocareDispozitiv(coada->arr[i]);
	free(coada->arr);
	free(coada);
}

DispozitivMobil* copiazaDispozitiv(DispozitivMobil* d) {
	DispozitivMobil* c = (DispozitivMobil*)malloc(sizeof(DispozitivMobil));
	c->idDispozitiv = d->idDispozitiv;
	c->pret = d->pret;
	c->nrZileInService = d->nrZileInService;
	c->defecte = copiazaString(d->defecte);
	c->sistemOperare = copiazaString(d->sistemOperare);
	return c;
}

DispozitivMobil* popCoadaPrioritati(CoadaPrioritati* coada) {
	DispozitivMobil* d = copiazaDispozitiv(coada->arr[0]);
	swap(coada->arr, 0, coada->size - 1);
	dezalocareDispozitiv(coada->arr[coada->size - 1]);
	coada->size--;
	maxHeapify(coada, 0);
	return d;
}

void testHeapExtrageMax(CoadaPrioritati* coada, unsigned int nr) {
	while (coada->size && nr--) {
		DispozitivMobil* d = popCoadaPrioritati(coada);
		printf("\nDispozitiv extras cu %u zile in service\n", d->nrZileInService);
		dezalocareDispozitiv(d);
	}
}

int main() {
	CoadaPrioritati* coada = creazaCoadaPrioritatiDinFisier("dispozitive.txt");
	afisareCoadaPrioritati(coada);

	printf("\nAndroid: %u\n", obtineNrDispozitiveDupaOs(coada, "Android"));
	printf("iOS: %u\n", obtineNrDispozitiveDupaOs(coada, "iOS"));
	printf("Windows: %u\n", obtineNrDispozitiveDupaOs(coada, "Windows"));

	actualizeazaDefecteDispozitiv(coada, 1, "EcranDistrusComplet");
	actualizeazaDefecteDispozitiv(coada, 3, "Hacked");
	actualizeazaDefecteDispozitiv(coada, 9, "OricumNuExista");

	printf("\nCoada dupa actualizari:\n");
	afisareCoadaPrioritati(coada);
	testHeapExtrageMax(coada, 2);
	printf("\nCoada dupa extragere:\n");
	afisareCoadaPrioritati(coada);

	dezalocareHeap(coada);
	return 0;
}