////1. Definiti structura Factura ce contine : id factura(int), data emitere(char*), denumire furnizor(char*), suma de plata(float), numar zile scadenta(unsigned int).
////Creati o lista dubla cu cel putin 5 facturi ale caror date sunt preluate dintr - un fisier text.
////Crearea structurii se realizeaza prin apel repetat al unei functii de inserare, 
////iar verificarea crearii cu succes se face prin traversarea listei in ambele sensuri. 
////
////2. Implementati functia care determina numarul total de zile pana la scadenta aferente facturilor emise catre un anumit beneficiar, 
////specificat ca parametru de intrare al functiei.
////Functia implementata se apeleaza in functia main(), iar rezultatul apelului se afiseaza in consola de executie a aplicatiei.
////
////3. Implementati functia care modifica denumirea furnizorului aferent unei facturi specificata prin id factura in lista de parametri ai functiei.
////Noul furnizor este specificat, de asemenea, in lista de parametri ai functiei.Functia implementata se apeleaza in functia main(), 
////iar rezultatul apelului se afiseaza in consola de executie a aplicatiei.
////
////4. Implementati functia care sterge facturile din lista dubla pentru care suma de plata este mai mica decat un prag specificat ca parametru.
////Functia implementata se apeleaza in functia main(), iar rezultatul apelului se afiseaza in consola de executie a aplicatiei.
////
////5. Implementati functia care returneaza un vector cu facturile emise dupa o anumita data calendaristica precizata ca parametru de intrare al functiei.
////Valorile sunt preluate din lista dubla creata mai sus si nu sunt partajate zone de memorie heap intre cele doua structuri(lista dubla si vector).
////Functia implementata se apeleaza in functia main(), iar rezultatul apelului(vector de facturi) se afiseaza in consola de executie a aplicatiei.
////
////#define _CRT_SECURE_NO_WARNINGS
////#include <stdio.h>
////#include <stdlib.h>
////#include <string.h>
////
//// 1. Structura Factura si lista dubla
////
////typedef struct Factura {
////    int id;
////    char* data_emitere;
////    char* furnizor;
////    float suma;
////    unsigned int zile_scadenta;
////} Factura;
////
////typedef struct Nod {
////    Factura info;
////    struct Nod* next;
////    struct Nod* prev;
////} Nod;
////
////Nod* inserareSfarsit(Nod* cap, Factura f) {
////    Nod* nou = (Nod*)malloc(sizeof(Nod));
////    nou->info = f;
////    nou->next = NULL;
////    nou->prev = NULL;
////    if (cap == NULL)
////        return nou;
////    Nod* p = cap;
////    while (p->next)
////        p = p->next;
////    p->next = nou;
////    nou->prev = p;
////    return cap;
////}
////
////void afisareLista(Nod* cap) {
////    Nod* p = cap;
////    while (p) {
////        printf("Factura: %d, Data: %s, Furnizor: %s, Suma: %.2f, Zile: %d\n",
////            p->info.id, p->info.data_emitere, p->info.furnizor, p->info.suma, p->info.zile_scadenta);
////        p = p->next;
////    }
////}
////
////Factura citesteFacturaDinFisier(FILE* f) {
////    char buffer[100];
////    char* token;
////    Factura fact;
////
////    fgets(buffer, 100, f);
////    token = strtok(buffer, ",\n");
////    fact.id = atoi(token);
////
////    token = strtok(NULL, ",\n");
////    fact.data_emitere = _strdup(token);
////
////    token = strtok(NULL, ",\n");
////    fact.furnizor = _strdup(token);
////
////    token = strtok(NULL, ",\n");
////    fact.suma = atof(token);
////
////    token = strtok(NULL, ",\n");
////    fact.zile_scadenta = atoi(token);
////
////    return fact;
////}
////
////void eliberareLista(Nod* cap) {
////    while (cap) {
////        Nod* temp = cap;
////        cap = cap->next;
////        free(temp->info.data_emitere);
////        free(temp->info.furnizor);
////        free(temp);
////    }
////}
////
//// 2. Numar total de zile pentru un anumit furnizor
////int totalZileScadenta(Nod* cap, const char* furnizor) {
////    int total = 0;
////    while (cap) {
////        if (strcmp(cap->info.furnizor, furnizor) == 0)
////            total += cap->info.zile_scadenta;
////        cap = cap->next;
////    }
////    return total;
////}
////
//// 3. Modificare furnizor dupa ID
////void modificaFurnizor(Nod* cap, int id, const char* nou_furnizor) {
////    while (cap) {
////        if (cap->info.id == id) {
////            free(cap->info.furnizor);
////            cap->info.furnizor = _strdup(nou_furnizor);
////        }
////        cap = cap->next;
////    }
////}
////
//// 4. Sterge facturi cu suma < prag
////Nod* stergeFacturiSumaMaiMica(Nod* cap, float prag) {
////    Nod* p = cap;
////    while (p) {
////        if (p->info.suma < prag) {
////            Nod* de_sters = p;
////            if (p->prev) p->prev->next = p->next;
////            else cap = p->next;
////            if (p->next) p->next->prev = p->prev;
////            p = p->next;
////            free(de_sters->info.data_emitere);
////            free(de_sters->info.furnizor);
////            free(de_sters);
////        }
////        else {
////            p = p->next;
////        }
////    }
////    return cap;
////}
////
//// 5. Returneaza vector de facturi dupa o anumita data (simplificat: strcmp pe stringuri)
////Factura* facturiDupaData(Nod* cap, const char* data, int* nr) {
////    *nr = 0;
////    Nod* p = cap;
////    while (p) {
////        if (strcmp(p->info.data_emitere, data) > 0)
////            (*nr)++;
////        p = p->next;
////    }
////    if (*nr == 0) return NULL;
////
////    Factura* vect = (Factura*)malloc(sizeof(Factura) * (*nr));
////    int i = 0;
////    p = cap;
////    while (p) {
////        if (strcmp(p->info.data_emitere, data) > 0) {
////            vect[i].id = p->info.id;
////            vect[i].data_emitere = _strdup(p->info.data_emitere);
////            vect[i].furnizor = _strdup(p->info.furnizor);
////            vect[i].suma = p->info.suma;
////            vect[i].zile_scadenta = p->info.zile_scadenta;
////            i++;
////        }
////        p = p->next;
////    }
////    return vect;
////}
////
//// main
////int main() {
////    Nod* lista = NULL;
////    FILE* f = fopen("facturi.txt", "r");
////    if (f) {
////        for (int i = 0; i < 5; i++) {
////            Factura fact = citesteFacturaDinFisier(f);
////            lista = inserareSfarsit(lista, fact);
////        }
////        fclose(f);
////    }
////
////    printf("--- Lista initiala ---\n");
////    afisareLista(lista);
////
////     2. total zile scadenta
////    printf("\nTotal zile pentru furnizor 'Electrica': %d\n", totalZileScadenta(lista, "Electrica"));
////
////     3. modificare furnizor
////    modificaFurnizor(lista, 2, "FurnizorNou");
////    printf("\nDupa modificare furnizor:\n");
////    afisareLista(lista);
////
////     4. stergere
////    lista = stergeFacturiSumaMaiMica(lista, 300.0f);
////    printf("\nDupa stergere facturi < 300:\n");
////    afisareLista(lista);
////
////     5. vector dupa data
////    int nr = 0;
////    Factura* vect = facturiDupaData(lista, "2024-01-01", &nr);
////    printf("\nFacturi dupa data 2024-01-01:\n");
////    for (int i = 0; i < nr; i++) {
////        printf("Factura: %d, Data: %s, Furnizor: %s, Suma: %.2f, Zile: %d\n",
////            vect[i].id, vect[i].data_emitere, vect[i].furnizor, vect[i].suma, vect[i].zile_scadenta);
////        free(vect[i].data_emitere);
////        free(vect[i].furnizor);
////    }
////    free(vect);
////
////    eliberareLista(lista);
////    return 0;
////}