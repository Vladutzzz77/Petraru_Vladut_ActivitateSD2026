#include<stdio.h>
#include<malloc.h>

struct Film {
    int id;
    int durata;
    const char* denumire;
    float buget;
    char varstaMinima;
};
struct Film initializare(int _id, int _durata, const char* _denumire, float _buget, char _varstaMinima) {
    struct Film s;
    s.id = _id;
    s.durata = _durata;
    s.denumire = malloc((strlen(_denumire) + 1) * sizeof(char));
    strcpy(s.denumire, _denumire);
    s.buget = _buget;
    s.varstaMinima = _varstaMinima;

    return s;
}

struct Film copyFilm(struct Film s) {
    return initializare(s.id, s.durata, s.denumire, s.buget, s.varstaMinima);
}

void afisare(struct Film s) {
    printf("Id: %d\n", s.id);
    printf("Nume: %s\n", s.denumire);
    printf("Durata in minute: %d\n", s.durata);
    printf("Buget: %f\n", s.buget);
    printf("Varsta minima pentru vizionare:%c \n", s.varstaMinima);
    printf("\n");
}

void afisareVector(struct Film* vector, int nrElemente) {
    for (int i = 0; i < nrElemente; i++)
        afisare(vector[i]);
}

struct Film* copiazaPrimeleNElemente(struct Film* vector, int nrElemente, int nrElementeCopiate) {
    struct Film* vectorNou = malloc(nrElementeCopiate * sizeof(struct Film));
    for (int i = 0; i < nrElementeCopiate; ++i)
        vectorNou[i] = vector[i];
    return vectorNou;
}

void dezalocare(struct Film** vector, int* nrElemente) {
    for (int i = 0; i < nrElemente; ++i)
        free((*vector)[i].denumire);
    free(vector);
    *vector = NULL;
    *nrElemente = 0;
}

void copiazaAnumiteElemente(struct Film* vector, char nrElemente, float bugetMaxim, struct Film** vectorNou, int* dimensiune) {
    *dimensiune = 0;
    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].buget < bugetMaxim)
            (*dimensiune)++;

    }

    vectorNou = malloc((*dimensiune) * sizeof(struct Film));

    int k = 0;
    for (int i = 0; i < (*dimensiune); i++)
        if (vector[k].buget < bugetMaxim) {
            (*vectorNou)[k] = copyFilm(vector[i]);
            k++;
        }

}

struct Film getPrimulElementConditionat(struct Film* vector, int nrElemente, const char* numeCautat) {

    struct Film s;
    s.id = 1;
    s.denumire = NULL;
    for (int i = 0; i < nrElemente; i++) {
        if (strcmp(vector[i].denumire, numeCautat))
            return vector[i];
    }
}



int main() {

    struct Film s = initializare(1, 20, "Denumire", 300, 13);

    int nr_filme = 3;
    struct Film* filme = malloc(nr_filme * sizeof(struct Film));
    filme[0] = s;
    *(filme + 1) = initializare(2, 360, "James bont", 14, 12);
    filme[2] = initializare(3, 30, "James bont 2", 154.8, 12);


    afisareVector(filme, nr_filme);

    int nrFilmeCopiate = 2;
    struct Film* vectorNou;
    vectorNou = copiazaPrimeleNElemente(filme, nr_filme, nrFilmeCopiate);
    printf("\nElemente copiate: \n");
    afisareVector(vectorNou, nrFilmeCopiate);

    dezalocare(&vectorNou, &nrFilmeCopiate);
    printf("\nElemente copiate:");
    afisareVector(vectorNou, nrFilmeCopiate);
    struct Film* vectorFilmeIeftine;
    float prag = 50;
    int dimIeftin;
    copiazaAnumiteElemente(filme, nr_filme, prag, vectorFilmeIeftine, &dimIeftin);
    afisareVector(vectorFilmeIeftine, dimIeftin);

    afisare(getPrimulElementConditionat(filme, nr_filme, "Dune"));
    dezalocare(&filme, &nr_filme);
    return 0;
}