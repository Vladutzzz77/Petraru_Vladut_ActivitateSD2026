#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct StructuraMasina Masina;
struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};


//creare structura pentru un nod dintr-o lista simplu inlantuita
typedef struct Nod Nod;

struct Nod {
	Masina masina;
	Nod* next;
};


Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}
void adaugaMasinaInLista(Nod** lista, Masina masinanoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->masina = masinanoua;
	nou->next = NULL;
	Nod* aux = (*lista);
	if ((*lista) != NULL) {
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*lista = nou;
	}

}

void afisareListaMasini(Nod* cap) {
	while (cap != NULL)
	{
		afisareMasina(cap->masina);
		cap = cap->next;
	}
}


void adaugaLaInceputInLista(Nod** lista, Masina masinanoua) {
	//adauga la inceputul listei o noua masina pe care o primim ca parametru
}

void* citireListaMasiniDinFisier(const char* numeFisier) {

	FILE* file = fopen(numeFisier, "r");
	Nod* lista = NULL;
	while (!feof(file))
	{
		adaugaMasinaInLista(&lista, citireMasinaDinFisier(file));
	}
	fclose(file);
	return lista;
}

void dezalocareListaMasini(Nod** lista) {
	Nod* head = *lista;
	Nod* next = NULL;
	while (head != NULL)
	{
		next = head->next;
		free(head->masina.model);
		free(head->masina.numeSofer);
		free(head);
		head = next;
	}
	*lista = NULL;

}

float calculeazaPretMediu(Nod* lista) {
	float suma = 0;
	int k = 0;
	while (lista != NULL)
	{
		suma += lista->masina.pret;
		k++;
		lista = lista->next;
	}
	return suma / k;
}

void stergeMasiniDinSeria(/*/ lista masini / char serieCautata*/) {
	//sterge toate masinile din lista care au seria primita ca parametru.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

float calculeazaPretulMasinilorUnuiSofer(Nod* lista, char* numesofer) {

	float suma = 0;
	while (lista != NULL)
	{
		if (lista->masina.numeSofer == numesofer)
			suma += lista->masina.pret;
	}
	return suma;
}

int main() {
	Nod* lista = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);
	printf("%2f", calculeazaPretMediu(lista));
	return 0;
}