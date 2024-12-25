#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MSL 255

typedef struct {
    char *input;
    char *output;
} params;

void error(const char *err) {
    fprintf(stderr, "%s", err);
    exit(EXIT_FAILURE);
}

typedef struct nodo {
    struct nodo *next;
    struct nodo *prev;
    char nome[MSL + 1];
    char cognome[MSL + 1];
    int numero_conto;
    int anno_apertura;
    float saldo;
} nodo;

typedef nodo *lista;

params readInput(int argc, char **argv) {
    if (argc != 3) error("Error:: params to insert input, output\n");
    params p;
    p.input = argv[1];
    p.output = argv[2];
    if (strcmp(argv[1] + strlen(argv[1]) - 4, ".txt") != 0) error("Il parametro input deve contenere l'estensione .txt");
    if (strcmp(argv[2] + strlen(argv[2]) - 4, ".txt") != 0) error("Il parametro output deve contenere l'estensione .txt");
    return p;
}

void insertNodo(lista *l, char *nome, char *cognome, int numero_conto, int anno_apertura, float saldo) {
    nodo *n = malloc(sizeof(nodo));
    if (n == NULL) error("Memory allocation failed\n");
    strncpy(n->nome, nome, MSL + 1);
    strncpy(n->cognome, cognome, MSL + 1);
    n->numero_conto = numero_conto;
    n->anno_apertura = anno_apertura;
    n->saldo = saldo;
    n->next = NULL;
    n->prev = NULL;

    if (*l == NULL) {
        *l = n;
    } else if (n->anno_apertura < (*l)->anno_apertura) {
        n->next = *l;
        (*l)->prev = n;
        *l = n;
    } else {
        nodo *tmp = *l;
        while (tmp->next != NULL && tmp->next->anno_apertura < n->anno_apertura) {
            tmp = tmp->next;
        }
        n->next = tmp->next;
        if (tmp->next != NULL) {
            tmp->next->prev = n;
        }
        tmp->next = n;
        n->prev = tmp;
    }
}

nodo **readFile(char *input) {
    nodo **head = malloc(sizeof(nodo *));
    *head = NULL;
    FILE *fp = fopen(input, "r");
    if (!fp) error("File cannot be opened\n");

    char nome[MSL + 1], cognome[MSL + 1];
    int numero_conto, anno_apertura;
    float saldo;
    
    while (fscanf(fp, "%s %s %d %d %f", nome, cognome, &numero_conto, &anno_apertura, &saldo) != EOF) {
        insertNodo(head, nome, cognome, numero_conto, anno_apertura, saldo);
    }

    fclose(fp);
    return head;
}

float getMaxSaldo(lista l) {
    float maxSaldo = -1;
    while (l != NULL) {
        if (l->saldo > maxSaldo) {
            maxSaldo = l->saldo;
        }
        l = l->next;
    }
    return maxSaldo;
}

void removeAccount(lista *l, nodo *n) {
    if (n->prev != NULL) {
        n->prev->next = n->next;
    } else {
        *l = n->next;
    }
    if (n->next != NULL) {
        n->next->prev = n->prev;
    }
    free(n);
}

void calculateXAndRemove(lista *l, float maxSaldo) {
    nodo *current = *l;
    float maxX = -1;
    nodo *maxNode = NULL;

    while (current != NULL) {
        float x = fminf(2023 - current->anno_apertura, 17 * current->saldo / maxSaldo);
        if (x > maxX) {
            maxX = x;
            maxNode = current;
        }
        current = current->next;
    }

    if (maxNode != NULL) {
        removeAccount(l, maxNode);
    }
}

void writeFile(char *output, lista l) {
    FILE *fp = fopen(output, "w");
    if (!fp) error("File cannot be opened for writing\n");

    nodo *current = l;
    while (current != NULL) {
        fprintf(fp, "%s %s %d %d %.2f\n", current->nome, current->cognome, current->numero_conto, current->anno_apertura, current->saldo);
        current = current->next;
    }

    fclose(fp);
}

int main(int argc, char **argv) {
    params p = readInput(argc, argv);

    lista l = NULL;
    nodo **head = readFile(p.input);
    l = *head;

    float maxSaldo = getMaxSaldo(l);

    // Calcoliamo quante volte rimuovere il cliente
    int n = 0;
    nodo *current = l;
    while (current != NULL) {
        n++;
        current = current->next;
    }

    int h = (int)sqrt(n); // Come indicato nel punto D

    for (int i = 0; i < h; i++) {
        calculateXAndRemove(&l, maxSaldo);
        maxSaldo = getMaxSaldo(l); // Ricalcoliamo il saldo massimo
    }

    writeFile(p.output, l);
    return 0;
}
