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
typedef struct Nod Nod;
struct Nod {
    struct Nod* nodStanga;
    struct Nod* nodDreapta;
    Masina info;
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

//INSERARE
void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {
    //adauga o noua masina pe care o primim ca parametru in arbore,
    //astfel incat sa respecte princiippile de arbore binar de cautare
    //dupa o anumita cheie pe care o decideti - poate fi ID

    if ((*radacina) == NULL) {
        Nod* nodNou = (Nod*)malloc(sizeof(Nod));
        nodNou->info = masinaNoua;
        nodNou->nodDreapta = NULL;
        nodNou->nodStanga = NULL;
        (*radacina) = nodNou;
        return;
    }
    if (masinaNoua.id < (*radacina)->info.id) {
        adaugaMasinaInArbore(&(*radacina)->nodStanga, masinaNoua); //adresa nodului din stanga

    }
    else //daca sunt egale pun in dreapta
    {
        adaugaMasinaInArbore(&(*radacina)->nodDreapta, masinaNoua);
    }
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) {
    //functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
    //prin apelul repetat al functiei citireMasinaDinFisier()
    //ATENTIE - la final inchidem fisierul/stream-ul

    Nod* radacina = NULL;
    FILE* f = fopen(numeFisier, "r");

    while (!feof(f)) {
        adaugaMasinaInArbore(&radacina, citireMasinaDinFisier(f));
    }

    fclose(f);

    return radacina;
}

void afisareMasiniDinArboreSRD(Nod* radacina) {
    //afiseaza toate elemente de tip masina din arborele creat
    //prin apelarea functiei afisareMasina()
    //parcurgerea arborelui poate fi realizata in TREI moduri
    //folositi toate cele TREI moduri de parcurgere

    if (radacina != NULL) {
        //inordine SRD stanga redacina dreapta
        afisareMasiniDinArboreSRD(radacina->nodStanga);
        afisareMasina(radacina->info);
        afisareMasiniDinArboreSRD(radacina->nodDreapta);
    }
}

void afisareMasiniArborePostOrdine(Nod* radacina)
{
    if (radacina != NULL) {
        afisareMasiniArborePostOrdine(radacina->nodStanga);
        afisareMasiniArborePostOrdine(radacina->nodDreapta);
        afisareMasina(radacina->info);

    }
}

void dezalocareArboreDeMasini(Nod** radacina) {
    //sunt dezalocate toate masinile si arborele de elemente

    if ((*radacina) != NULL) {
        //postordine pt dezalocare stanga dreapta radacina
        dezalocareArboreDeMasini(&(*radacina)->nodStanga);
        dezalocareArboreDeMasini(&(*radacina)->nodDreapta);


        free((*radacina)->info.model);
        free((*radacina)->info.numeSofer);
        free(*radacina);
    }
}

Masina getMasinaByID(Nod* radacina, int idCautat) {
    Masina masinaRezultat;

    if (radacina != NULL)
    {
        if (radacina->info.id == idCautat)
        {
            masinaRezultat = radacina->info;
            masinaRezultat.model = malloc(strlen(radacina->info.model) + 1);
            strcpy(masinaRezultat.model, radacina->info.model);

            masinaRezultat.numeSofer = malloc(strlen(radacina->info.numeSofer) + 1);
            strcpy(masinaRezultat.numeSofer, radacina->info.numeSofer);
        }
        else
        {
            if (radacina->info.id <= idCautat)
            {
                masinaRezultat = getMasinaByID(radacina->nodDreapta, idCautat);
            }
            else
            {
                masinaRezultat = getMasinaByID(radacina->nodStanga, idCautat);
            }
        }
    }
    else
    {
        masinaRezultat.id = -1;
    }

    return masinaRezultat;

}

int determinaNumarNoduri(Nod* radacina) {
    //calculeaza numarul total de noduri din arborele binar de cautare
    if (radacina != NULL) {
        int nrNoduriStanga = determinaNumarNoduri(radacina->nodStanga);
        int nrNoduriDreapta = determinaNumarNoduri(radacina->nodDreapta);

        return 1 + nrNoduriStanga + nrNoduriDreapta;
    }

    return 0;
}
int calculareMaxim(int param1, int param2) {
    if (param1 > param2)
        return param1;
    else
        return param2;
}
int calculeazaInaltimeArbore(Nod* radacina) {
    if (radacina == NULL) return -1;
    else {
        return 1 + calculareMaxim(calculeazaInaltimeArbore(radacina->nodStanga), calculeazaInaltimeArbore(radacina->nodDreapta));
    }
    return 0;
}

float calculeazaPretTotal(Nod* radacina) {
    //calculeaza pretul tuturor masinilor din arbore.

    if (radacina == NULL) return 0;
    else {
        float totalStanga = calculeazaPretTotal(radacina->nodStanga);
        float totalDreapta = calculeazaPretTotal(radacina->nodDreapta);
        return radacina->info.pret + totalStanga + totalDreapta;
    }

}

float calculeazaPretulMasinilorUnuiSofer(/*arbore de masini*/ const char* numeSofer) {
    //calculeaza pretul tuturor masinilor unui sofer.
    return 0;
}

int main() {
    Nod* arbore = citireArboreDeMasiniDinFisier("masini_arbore.txt");
    afisareMasiniDinArboreSRD(arbore);

    printf("\nAfirare by ID:\n");
    afisareMasina(getMasinaByID(arbore, 5));
    printf("\nNumar Noduri din arbore: %d\n", determinaNumarNoduri(arbore));
    printf("\nInaltime arbore (muchii): %d\n", calculeazaInaltimeArbore(arbore));
    printf("\nPret total: %d\n", calculeazaPretTotal(arbore));
    return 0;
}