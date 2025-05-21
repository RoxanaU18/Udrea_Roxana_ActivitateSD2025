#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//1. Sa se realizeze o implementare de arbore binar de cautare echilibrat cu informatie 
//utila de tip articol(din orice domeniu),care sa includa minim 3 atribute, dintre care 
//minim unul de tip char* .Se vor implementa mecanismele de inserare cu date preluate din 
//fisier text, traversari preordine, inordine, postordine, dezalocare. 

struct filme {
	int idFilm;
	char* denumire;
	float pret;
	float nrBilete;
};

struct nodArb {
	int BF;
	struct filme inf;
	struct nodArb* stanga;
	struct nodArb* dreapta;
};

//crearea nodului
struct nodArb* creareNod(struct nodArb* stanga, struct nodArb* dreapta, struct filme f) {
	struct nodArb* nou = (struct nodArb*)malloc(sizeof(struct nodArb));
	nou->inf.idFilm = f.idFilm;
	nou->inf.denumire = (char*)malloc(strlen(f.denumire) + 1);
	strcpy(nou->inf.denumire, f.denumire);
	nou->inf.pret = f.pret;
	nou->inf.nrBilete = f.nrBilete;
	nou->dreapta = dreapta;
	nou->stanga = stanga;
	nou->BF = 0;
	return nou;
}

//inserare 
struct nodArb* inserare(struct filme f, struct nodArb* rad) {
	if (rad != NULL) {
		if (f.idFilm < rad->inf.idFilm)
			rad->stanga = inserare(f, rad->stanga);
		else if (f.idFilm > rad->inf.idFilm)
			rad->dreapta = inserare(f, rad->dreapta);
		return rad;
	}
	else {
		return creareNod(NULL, NULL, f);
	}
}

//preordine
void traversare_preordine(struct nodArb* rad) {
	if (rad) {
		printf("\nidFilm: %d ,denumire: %s ,pret: %.2f ,nrBilete: %.2f , BF=%d",
			rad->inf.idFilm, rad->inf.denumire, rad->inf.pret, rad->inf.nrBilete, rad->BF);
		traversare_preordine(rad->stanga);
		traversare_preordine(rad->dreapta);
	}
}

//inordine
void traversare_inordine(struct nodArb* root) {
	if (root) {
		traversare_inordine(root->stanga);
		printf("\nidFilm: %d ,denumire: %s ,pret: %.2f ,nrBilete: %.2f , BF=%d",
			root->inf.idFilm, root->inf.denumire, root->inf.pret, root->inf.nrBilete, root->BF);
		traversare_inordine(root->dreapta);
	}
}

//postordine
void traversare_postordine(struct nodArb* root) {
	if (root) {
		traversare_postordine(root->stanga);
		traversare_postordine(root->dreapta);
		printf("\nidFilm: %d ,denumire: %s ,pret: %.2f ,nrBilete: %.2f , BF=%d",
			root->inf.idFilm, root->inf.denumire, root->inf.pret, root->inf.nrBilete, root->BF);
	}
}

//dezalocare
void dezalocare(struct nodArb* root) {
	if (root) {
		dezalocare(root->stanga);
		dezalocare(root->dreapta);
		free(root->inf.denumire);
		free(root);
	}
}

//determinam numarul de niveluri ale arborelui
int maxim(int x, int y) {
	return (x > y) ? x : y;
}

int nr_niveluri(struct nodArb* root) {
	if (root)
		return 1 + maxim(nr_niveluri(root->stanga), nr_niveluri(root->dreapta));
	else
		return 0;
}

//gradul de echilibru al nodului
void calcBF(struct nodArb* root) {
	if (root != NULL) {
		root->BF = nr_niveluri(root->dreapta) - nr_niveluri(root->stanga);
		calcBF(root->stanga);
		calcBF(root->dreapta);
	}
}

// rotatii
struct nodArb* rotatie_dreapta(struct nodArb* root) {
	struct nodArb* nod1 = root->stanga;
	root->stanga = nod1->dreapta;
	nod1->dreapta = root;
	return nod1;
}

struct nodArb* rotatie_stanga(struct nodArb* root) {
	struct nodArb* nod1 = root->dreapta;
	root->dreapta = nod1->stanga;
	nod1->stanga = root;
	return nod1;
}

struct nodArb* rotatie_dreapta_stanga(struct nodArb* root) {
	root->dreapta = rotatie_dreapta(root->dreapta);
	return rotatie_stanga(root);
}

struct nodArb* rotatie_stanga_dreapta(struct nodArb* root) {
	root->stanga = rotatie_stanga(root->stanga);
	return rotatie_dreapta(root);
}

struct nodArb* reechilibrare(struct nodArb* root) {
	calcBF(root);
	if (root->BF <= -2 && root->stanga->BF <= -1)
		root = rotatie_dreapta(root);
	else if (root->BF >= 2 && root->dreapta->BF >= 1)
		root = rotatie_stanga(root);
	else if (root->BF >= 2 && root->dreapta->BF <= -1)
		root = rotatie_dreapta_stanga(root);
	else if (root->BF <= -2 && root->stanga->BF >= 1)
		root = rotatie_stanga_dreapta(root);
	calcBF(root);
	return root;
}

//2. Afisarea pe niveluri
void traversareNivel(struct nodArb* root, int nivelCautat, int nivelCurent) {
	if (root) {
		if (nivelCautat == nivelCurent) {
			printf("\nidFilm: %d ,denumire: %s ,pret: %.2f ,nrBilete: %.2f , BF=%d",
				root->inf.idFilm, root->inf.denumire, root->inf.pret, root->inf.nrBilete, root->BF);
		}
		traversareNivel(root->stanga, nivelCautat, nivelCurent + 1);
		traversareNivel(root->dreapta, nivelCautat, nivelCurent + 1);
	}
}

void traversarePeNivele(struct nodArb* root) {
	int h = nr_niveluri(root);
	for (int i = 0; i < h; i++) {
		printf("\nNivelul %d", i + 1);
		traversareNivel(root, i, 0);
	}
}

//3. Salvare frunze
void detFrunze(struct nodArb* root, struct filme* vector, int* nrF) {
	if (root) {
		if (!root->stanga && !root->dreapta) {
			vector[*nrF].idFilm = root->inf.idFilm;
			vector[*nrF].denumire = (char*)malloc(strlen(root->inf.denumire) + 1);
			strcpy(vector[*nrF].denumire, root->inf.denumire);
			vector[*nrF].pret = root->inf.pret;
			vector[*nrF].nrBilete = root->inf.nrBilete;
			(*nrF)++;
		}
		detFrunze(root->stanga, vector, nrF);
		detFrunze(root->dreapta, vector, nrF);
	}
}

//4. Stergere dupa denumire
struct nodArb* stergeRoot(struct nodArb* root) {
	struct nodArb* aux = root;
	if (aux->stanga) {
		root = aux->stanga;
		if (aux->dreapta) {
			struct nodArb* temp = root;
			while (temp->dreapta)
				temp = temp->dreapta;
			temp->dreapta = aux->dreapta;
		}
	}
	else if (aux->dreapta)
		root = aux->dreapta;
	else
		root = NULL;

	free(aux->inf.denumire);
	free(aux);
	return root;
}

struct nodArb* stergenod(struct nodArb* root, int idFilm) {
	if (!root)
		return NULL;
	if (root->inf.idFilm == idFilm)
		return stergeRoot(root);
	if (idFilm < root->inf.idFilm)
		root->stanga = stergenod(root->stanga, idFilm);
	else
		root->dreapta = stergenod(root->dreapta, idFilm);
	return root;
}

struct nodArb* stergeDenumire(struct nodArb* root, char* denumire) {
	if (root) {
		if (strcmp(root->inf.denumire, denumire) == 0)
			return stergenod(root, root->inf.idFilm);
		root->stanga = stergeDenumire(root->stanga, denumire);
		root->dreapta = stergeDenumire(root->dreapta, denumire);
	}
	return root;
}

// MAIN
int main() {
	struct nodArb* root = NULL;
	FILE* fis = fopen("filme.txt", "r");
	if (!fis) {
		printf("Eroare la deschiderea fisierului!\n");
		return 1;
	}

	int nrFilme;
	char buffer[100];
	struct filme f;
	fscanf(fis, "%d", &nrFilme);

	for (int i = 0; i < nrFilme; i++) {
		fscanf(fis, "%d %s %f %f", &f.idFilm, buffer, &f.pret, &f.nrBilete);
		f.denumire = (char*)malloc(strlen(buffer) + 1);
		strcpy(f.denumire, buffer);
		root = inserare(f, root);
		root = reechilibrare(root);
		free(f.denumire);
	}
	fclose(fis);

	printf("\n-------preordine-----");
	traversare_preordine(root);
	printf("\n\n-------inordine-----");
	traversare_inordine(root);
	printf("\n\n-------postordine-----");
	traversare_postordine(root);

	traversarePeNivele(root);

	// afisare frunze
	struct filme* vector = (struct filme*)malloc(nrFilme * sizeof(struct filme));
	int nr = 0;
	detFrunze(root, vector, &nr);
	printf("\n\n--- Frunze ---");
	for (int i = 0; i < nr; i++) {
		printf("\nFrunza: %d, %s, %.2f", vector[i].idFilm, vector[i].denumire, vector[i].pret);
		free(vector[i].denumire);
	}
	free(vector);

	// stergere dupa denumire
	char denum[100];
	printf("\n\nIntroduceti filmul de sters: ");
	scanf("%s", denum);
	root = stergeDenumire(root, denum);
	root = reechilibrare(root);

	printf("\n\n--- Arbore dupa stergere ---");
	traversare_preordine(root);

	dezalocare(root);
	return 0;
}
