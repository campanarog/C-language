#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MSL 30

typedef struct{
    char* input;
    float f;
} params;

typedef struct nodo{
    char *data;
    float value;
    struct nodo* next;
    struct nodo* prev;
} nodo;

typedef nodo* lista;

params readinput(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage %s <input> <f>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    params p;
    p.input = argv[1];
    p.f = atof(argv[2]);
    if (p.f < 1.0 || p.f > 20.0) {
        fprintf(stderr, "f must be in range 1-20\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

void insertrecord(lista* l, const char* data, float value) {
    nodo* n = malloc(sizeof(nodo));
    if (n == NULL) {
        fprintf(stderr, "memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
       n->data = malloc(MSL * sizeof(char));
    if (n->data == NULL) {
        fprintf(stderr, "memory allocation failed\n");
        free(n);
        exit(EXIT_FAILURE);
    }
    strncpy(n->data, data, MSL);
    n->data[MSL - 1] = '\0';
    n->value = value;
    n->next = n->prev = NULL;

    if (*l == NULL) {
        *l = n;
    } else {
        nodo* tmp = *l;
        nodo* prev = NULL;
        while (tmp != NULL && strcmp(tmp->data, n->data) < 0) {
            prev = tmp;
            tmp = tmp->next;
        }
        n->next = tmp;
        n->prev = prev;
        if (prev == NULL) {
            *l = n;
        } else {
            prev->next = n;
        }
        if (tmp != NULL) {
            tmp->prev = n;
        }
    }
}

void buildlist(lista* l, const char* input) {
    FILE* fp = fopen(input, "r");
    if (!fp) {
        fprintf(stderr, "file cannot be opened\n");
        exit(EXIT_FAILURE);
    }
    char buffer[MSL];
    float value;
    while (fscanf(fp, "%s %f\n", buffer, &value) != EOF) {
        insertrecord(l, buffer, value);
    }
    fclose(fp);
}

void printlist(lista l) {
    while (l != NULL) {
        printf("%s %.1f\n", l->data, l->value);
        l = l->next;
    }
}

void elab(lista l, float f) {
    int count = 0;
    while (l != NULL) {
        if (l->value >= f) {
            count++;
        }
        l = l->next;
    }
    printf("Numero di record aventi pari o maggiori di %.1f: %d\n", f, count);
}

int main(int argc, char** argv) {
    params p = readinput(argc, argv);
    lista l = NULL;
    buildlist(&l, p.input);
    printlist(l);
    elab(l, p.f);
    return 0;
}
