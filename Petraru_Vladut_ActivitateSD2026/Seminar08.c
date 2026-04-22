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

//creare structura pentru Heap
//un vector de elemente, lungimea vectorului si numarul de elemente din vector
struct Heap {
	int lungime;
	int nrMasini;
	Masina* masini;
};
typedef struct Heap Heap;

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

Heap initializareHeap(int lungime) {
	//initializeaza heap-ul cu 0 elemente 
	//dar cu o lungime primita ca parametru
	Heap heap;
	heap.lungime = lungime;
	heap.masini = malloc(lungime * sizeof(Masina));
	heap.nrMasini = 0;
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	//filtreaza heap-ul pentru nodul a carei pozitie o primeste ca parametru
	if (pozitieNod >= 0 && pozitieNod < heap.lungime) {
		int pozitieStanga = 2 * pozitieNod + 1;
		int pozitieDreapta = 2 * pozitieNod + 2;
		int pmax = pozitieNod;
		if (pozitieStanga < heap.nrMasini && heap.masini[pmax].pret < heap.masini[pozitieStanga].pret) {
			pmax = pozitieStanga;
		}
		if (pozitieDreapta < heap.nrMasini && heap.masini[pmax].pret < heap.masini[pozitieDreapta].pret) {
			pmax = pozitieDreapta;
		}
		if (pmax != pozitieNod) {
			Masina aux= heap.masini[pozitieNod];
			heap.masini[pozitieNod] = heap.masini[pmax];
			heap.masini[pmax] = aux;
			if(pmax<(heap.nrMasini-2)/2){
				filtreazaHeap(heap, pmax);
			}
		}
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	//citim toate masinile din fisier si le stocam intr-un heap 
	// pe care trebuie sa il filtram astfel incat sa respecte 
	// principiul de MAX-HEAP sau MIN-HEAP dupa un anumit criteriu
	// sunt citite toate elementele si abia apoi este filtrat vectorul
	FILE* f = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);
	while (!feof(f)) {
			heap.masini[heap.nrMasini++] = citireMasinaDinFisier(f);
	}
	fclose(f);
	for (int i=(heap.nrMasini - 2) / 2; i >= 0; i--){
		filtreazaHeap(heap, i);
	}
	return heap;
}

void afisareHeap(Heap heap) {
	//afiseaza elementele vizibile din heap
	for (int i = 0; i < heap.nrMasini; i++) {
		afisareMasina(heap.masini[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	//afiseaza elementele ascunse din heap
	for (int i = heap.nrMasini; i < heap.lungime; i++) {
		afisareMasina(heap.masini[i]);
	}
}

Masina extrageMasina(Heap* heap) {
	//extrage si returneaza masina de pe prima pozitie
	//elementul extras nu il stergem...doar il ascundem
	if (heap->nrMasini > 0) {
		Masina aux = heap->masini[0];
		heap->masini[0] = heap->masini[heap->nrMasini - 1];
		heap->masini[heap->nrMasini - 1] = aux;
		heap->nrMasini--;
		for (int i = (heap->nrMasini - 2) / 2; i >= 0; i--) {
			filtreazaHeap(*heap, i);
		}
		return heap->masini[heap->nrMasini];
	}
	else {
		Masina m;
		m.id = -1;
		return m;
	}
}


void dezalocareHeap(Heap* heap) {
	//sterge toate elementele din Heap
}

int main() {
	Heap heap = citireHeapDeMasiniDinFisier("masini.txt");
	afisareHeap(heap);
	printf("Masina extrasa:\n");
	Masina m = extrageMasina(&heap);
	afisareMasina(m);
	Masina m2 = extrageMasina(&heap);
	afisareMasina(m2);
	Masina m3 = extrageMasina(&heap);
	afisareMasina(m3);
	printf("Masini ascunse:\n");
	afiseazaHeapAscuns(heap);

	return 0;
}