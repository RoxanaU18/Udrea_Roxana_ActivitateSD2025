//Articolul trebuie să aibă logica.Articolul trebuie sa fie un substantiv.Articolul trebuie sa aibă
//cel puțin trei atribute, si cel puțin unul să fie alocat dinamic.
//Realizati o functie cu ajutorul careia sa cititi de la tastatura un articol de tipul construit de
//voi.Funcția returnează obiectul citit.
//Realizati o functie care calculeaza ceva pentru un obiect de tipul implementat de voi. (o
//	medie sau o suma, un maxim, un minim...sau orice altceva)
//	Realizati o functie care modifica pentru un obiect primit ca parametru, un anumit camp.
//	noua valoare este primita ca parametru.
//	Realizați o funcție care afișează un obiect de tipul creat.Afișarea se face la console, și sunt
//	afișate toate informațiile.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Rucsac {
	int id;
	char* producator;
	float pret;

};

//asemenator cu constructorul cu parametrii
struct Rucsac initializare(int id, const char* producator, float pret) {
	struct Rucsac r;
	r.id = id;
	r.producator = (char*)malloc(sizeof(char) * (strlen(producator) + 1));
	strcpy_s(r.producator, strlen(producator) + 1, producator);
	r.pret=pret;
	return r;
}

void afisare(struct Rucsac r) {
	if (r.producator != NULL) {
		printf(" %d. Rucsacul %s costa %5.2f RON\n", r.id, r.producator, r.pret);
	}
	else {
		printf(" %d. (producator necunoscut) costa %5.2f RON\n", r.id, r.pret);
	}


}

void modificaPret(struct Rucsac* r, float noulPret) {
	if (noulPret > 0) {
		r->pret = noulPret;
	}
}



struct Rucsac citireRucsac() {
	struct Rucsac r;
	char buffer[100];

	printf("Introduceti ID-ul rucsacului: ");
	scanf_s("%d", &r.id);
	getchar(); 

	printf("Introduceti producatorul: ");
	fgets(buffer, sizeof(buffer), stdin);
	buffer[strcspn(buffer, "\n")] = 0;

	r.producator = (char*)malloc(strlen(buffer) + 1);
	strcpy_s(r.producator, strlen(buffer) + 1, buffer);

	printf("Introduceti pretul: ");
	scanf_s("%f", &r.pret);

	return r;
}

void afisareRucsac(struct Rucsac r) {
	printf("Rucsac [ID: %d, Producator: %s, Pret: %.2f lei]\n", r.id, r.producator, r.pret);
}

float calculeazaPretMediu(struct Rucsac* rucsaci, int nrRucsaci) {
	if (nrRucsaci <= 0) return 0;

	float suma = 0;
	for (int i = 0; i < nrRucsaci; i++) {
		suma += rucsaci[i].pret;
	}

	return suma / nrRucsaci;
}


void dezalocare(struct Rucsac* r) {
	if (r->producator != NULL) {
		free(r->producator);
		r->producator;
	}
}

int main() {
	struct Rucsac r;
	r = initializare(1, "Adidas", 100.5);
	r = citireRucsac(); // fără "struct Rucsac" în față
	afisare(r);
	modificaPret(&r, 50);
	afisare(r);
	dezalocare(&r);
	struct Rucsac rucsaci[3];
	rucsaci[0] = initializare(1, "Nike", 150);
	rucsaci[1] = initializare(2, "Puma", 100);
	rucsaci[2] = initializare(3, "Adidas", 130);

	for (int i = 0; i < 3; i++) {
		afisare(rucsaci[i]);
	}

	float medie = calculeazaPretMediu(rucsaci, 3);
	printf("Pretul mediu al rucsacilor: %.2f RON\n", medie);

	for (int i = 0; i < 3; i++) {
		dezalocare(&rucsaci[i]);
	}

	return 0;
}

