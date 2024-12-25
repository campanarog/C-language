#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MSL 30

typedef struct {
    char *input;
    float f;
} params;

typedef struct nodo {
    char *data;
    float value;
    struct nodo *next;
    struct nodo *prev;
} nodo;

typedef nodo* lista;

params readinput(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input> <f>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    params p;
    p.input = argv[1];
    p.f = atof(argv[2]);

    if (p.f < 1.0 || p.f > 20.0) {
        fprintf(stderr, "F must be in range 1-20\n");
        exit(EXIT_FAILURE);
    }

    return p;
}

void insertrecord(lista *l, char *data, float value) {
    nodo *n = malloc(sizeof(nodo));
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
    n->data[MSL - 1] = '\0';  // Ensure the string is null-terminated
    n->value = value;
    n->next = n->prev = NULL;

    if (*l == NULL) {
        *l = n;
    } else {
        nodo *current = *l;
        nodo *previous = NULL;
        while (current != NULL && strcmp(current->data, n->data) < 0) {
            previous = current;
            current = current->next;
        }
        n->next = current;
        n->prev = previous;
        if (previous == NULL) {
            *l = n;
        } else {
            previous->next = n;
        }
        if (current != NULL) {
            current->prev = n;
        }
    }
}

void buildlist(const char *input, lista *l) {
    FILE *fp = fopen(input, "r");
    if (!fp) {
        fprintf(stderr, "file cannot be opened\n");
        exit(EXIT_FAILURE);
    }

    char buffer[MSL];
    float value;
    while (fscanf(fp, "%f\n", &value) != EOF && fscanf(fp, "%s\n", buffer) != EOF) {
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
    printf("Numero di record aventi valore pari o maggiore di %.1f: %d\n", f, count);
}

int main(int argc, char **argv) {
    params p = readinput(argc, argv);

    lista l = NULL;
    buildlist(p.input, &l);

    printlist(l);
    elab(l, p.f);
    return 0;
}
