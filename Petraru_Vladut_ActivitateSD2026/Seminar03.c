#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

void afisareMasina(Masina masina) {

	//afiseaza toate atributele unei masini

	printf("\n ID Masina: %d", masina.id);
	printf("\n Nr Usi: %d", masina.nrUsi);
	printf("\n Pret: %f", masina.pret);
	printf("\n Model: %s", masina.model);
	printf("\n Numele Soferului: %s", masina.numeSofer);
	printf("\n Serie masina: %c", masina.serie);
	printf("\n-----------------------------");
}

void afisareVectorMasini(Masina* masini, int nrMasini) {

	//afiseaza toate elemente de tip masina din vector
	//prin apelarea functiei afisareMasina()

	for (int i = 0; i < nrMasini; i++) {
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int * nrMasini, Masina masinaNoua) {

	//adauga in vectorul primit o noua masina pe care o primim ca parametru
	//ATENTIE - se modifica numarul de masini din vector;

	(*nrMasini)++;
	Masina* aux = malloc(sizeof(Masina) * (*nrMasini));
	for (int i = 0; i < (*nrMasini) - 1; ++i) {
		aux[i] = (*masini)[i]; //shallow copy
	}
	aux[(*nrMasini) - 1] = masinaNoua; //shallow copy
	free(*masini);
	*masini = aux;
}

Masina citireMasinaFisier(FILE* file) {

	//functia citeste o masina dintr-un strceam deja deschis
	//masina citita este returnata;

	char linie[50];
	fgets(linie, 50, file);

	char delimitator[3] = ",\n";
	Masina masina;

	masina.id = atoi(strtok(linie, delimitator));
	masina.nrUsi = atoi(strtok(NULL, delimitator));
	masina.pret = atof(strtok(NULL, delimitator));

	char* model = strtok(NULL, delimitator);
	masina.model = malloc((strlen(model) + 1) * sizeof(char));
	strcpy(masina.model, model);

	char* numeSofer = strtok(NULL, delimitator);
	masina.numeSofer = malloc((strlen(numeSofer) + 1) * sizeof(char));
	strcpy(masina.numeSofer, numeSofer);
	
	char* serie = strtok(NULL, delimitator);
	masina.serie = serie[0];

	return masina;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {

	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaFisier()
	//numarul de masini este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul
	
	Masina* masini = NULL;
	*nrMasiniCitite = 0;
	FILE* file = fopen(numeFisier, "r");

	while (!feof(file)) {
		adaugaMasinaInVector(&masini, nrMasiniCitite, citireMasinaFisier(file));
	}

	fclose(file);
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {

	//este dezalocat intreg vectorul de masini

	for (int i = 0; i < *nrMasini; i++) {
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}
	free(*vector);
	*vector = NULL;
	*nrMasini = 0;

}

int main() {

	//FILE* file = fopen("masini.txt", "r");
	//Masina masina1 = citireMasinaFisier(file);
	//afisareMasina(masina1);

	Masina* masina;
	int nr = 0;
	masina = citireVectorMasiniFisier("masini.txt", &nr);
	afisareVectorMasini(masina, nr);

	dezalocareVectorMasini(&masina, &nr);

	return 0;
}