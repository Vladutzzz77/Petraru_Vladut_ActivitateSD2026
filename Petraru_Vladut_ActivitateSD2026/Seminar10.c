#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-un arbore binar de cautare

typedef struct NodArbore NodArbore;
struct NodArbore {
	Masina info;
	NodArbore* stanga;
	NodArbore* dreapta;
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

int calculeazaInaltimeArbore(NodArbore* radacina) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza

	if (radacina) {
		return 1 + max(calculeazaInaltimeArbore(radacina->stanga), calculeazaInaltimeArbore(radacina->dreapta));
	}
	else {
		return 0;
	}

	return 0;
}

//ALTE FUNCTII NECESARE:
// - aici veti adauga noile functii de care aveti nevoie.

char calculGE(NodArbore* nod) {
	//calculeaza gradul de echilibru al unui nod din arbore
	//GE = inaltime(subarbore stang) - inaltime(subarbore drept)
	if (nod) {
		return calculeazaInaltimeArbore(nod->stanga) - calculeazaInaltimeArbore(nod->dreapta);

	}
	else {
		return 0;
	}
}

void rotireStanga(NodArbore** radacina) {
	//rotire stanga a unui nod din arbore
	NodArbore* temp = (*radacina)->dreapta;
	(*radacina)->dreapta = temp->stanga;
	temp->stanga = *radacina;
	*radacina = temp;
}

void rotireDreapta(NodArbore** radacina) {
	//rotire dreapta a unui nod din arbore
	NodArbore* temp = (*radacina)->stanga;
	(*radacina)->stanga = temp->dreapta;
	temp->dreapta = *radacina;
	*radacina = temp;
}

void adaugaMasinaInArboreEchilibrat(NodArbore** radacina, Masina masinaNoua) {
	//adauga o noua masina pe care o primim ca parametru in arbore,
	//astfel incat sa respecte principiile de arbore binar de cautare ECHILIBRAT
	//dupa o anumita cheie pe care o decideti - poate fi ID
	if (radacina) {
		if (masinaNoua.id < (*radacina)->info.id) {
			adaugaMasinaInArboreEchilibrat(&(*radacina)->stanga, masinaNoua);
		}
		else if (masinaNoua.id > (*radacina)->info.id) {
			adaugaMasinaInArboreEchilibrat(&(*radacina)->dreapta, masinaNoua);
		}
		int grad = calculGE(*radacina);
		if (grad == 2) {//dezechilibru stanga
			if (calculGE((*radacina)->stanga) == 1) {//cazul stanga-stanga
				rotireDreapta(radacina);
			}
			else {//cazul stanga-dreapta
				rotireStanga(&(*radacina)->stanga);
				rotireDreapta(radacina);
			}


		}
		else if (grad == -2) {//dezechilibru dreapta
			if (calculGE((*radacina)->dreapta) == -1) {//cazul dreapta-dreapta
				rotireStanga(radacina);
			}
			else {//cazul dreapta-stanga
				rotireDreapta(&(*radacina)->dreapta);
				rotireStanga(radacina);
			}
		}
		else {
			NodArbore* nodNou = malloc(sizeof(NodArbore));
			nodNou->info = masinaNoua;
			nodNou->stanga = NULL;
			nodNou->dreapta = NULL;
			*radacina = nodNou;
		}
	}
}

NodArbore* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	NodArbore* radacina = NULL;
	while(!feof(file)) {
		Masina m1 = citireMasinaDinFisier(file);
		adaugaMasinaInArboreEchilibrat(&radacina, m1);
	}
	fclose(file);
	return radacina;
}

void afisareMasiniDinArbore(NodArbore* radacina) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
	if(radacina) {
		afisareMasiniDinArbore(radacina->stanga);
		afisareMasina(radacina->info);
		afisareMasiniDinArbore(radacina->dreapta);
	}
}

void dezalocareArboreDeMasini(NodArbore** radacina) {
	//sunt dezalocate toate masinile si arborele de elemente
	if (radacina && *radacina) {
		dezalocareArboreDeMasini(&(*radacina)->stanga);
		dezalocareArboreDeMasini(&(*radacina)->dreapta);
		free(*radacina);
		*radacina = NULL;
	}
}

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

Masina getMasinaByID(/*arborele de masini*/int id);

int determinaNumarNoduri(/*arborele de masini*/);

float calculeazaPretTotal(/*arbore de masini*/);

float calculeazaPretulMasinilorUnuiSofer(NodArbore* radacina, const char* numeSofer) {
	float total = 0;
	if (radacina) {
		total += calculeazaPretulMasinilorUnuiSofer(radacina->stanga, numeSofer);
		total += calculeazaPretulMasinilorUnuiSofer(radacina->dreapta, numeSofer);
		if (strcmp(radacina->info.numeSofer, numeSofer) == 0) {
			total += radacina->info.pret;
		}
	}
	return total;
}

int main() {
	NodArbore* arboreMasini = citireArboreDeMasiniDinFisier("masini.txt");
	afisareMasiniDinArbore(arboreMasini);

	dezalocareArboreDeMasini(&arboreMasini);
	return 0;
}