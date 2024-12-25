#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned int get_random() {
    static unsigned int m_w = 424242;
    static unsigned int m_z = 242424;
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}

void error(char* err) {
    fprintf(stderr, "%s", err);
    exit(EXIT_FAILURE);
}

typedef struct {
    int n;
    char* output;
    char* alfabeto;
} params;

typedef struct nodo {
    char value[255];
    struct nodo* next;
} nodo;

void push(nodo** head, char* value) {
    nodo* n = malloc(sizeof(nodo));
    if (n == NULL) error("Memory allocation failed\n");
    strcpy(n->value, value);
    n->next = *head;
    *head = n;
}

char* pop(nodo** head) {
    if ((*head) == NULL) error("Testa è vuota\n");
    nodo* tmp = *head;
    *head = tmp->next;
    char* out = tmp->value;
    char* copy = malloc((strlen(out) + 1) * sizeof(char));
    strcpy(copy, out);
    free(tmp);
    return copy;
}

params readinput(int argc, char** argv) {
    if (argc != 4) error("Valori da inserire <stringa_alfabeto>,<n>, <output>");
    params p;
    p.alfabeto = argv[1];
    p.n = atoi(argv[2]);
    p.output = argv[3];
    if (strlen(p.alfabeto) < 5 || strlen(p.alfabeto) > 15) error("La stringa di carattere deve essere tra 5 e 15 caratteri\n");
    if (p.n < 5 || p.n > 20) error("Range di N dev'essere 5-20\n");
    return p;
}

int* getValue(int n, int L) {
    int* W = malloc(n * sizeof(int));
    if (W == NULL) error("Memory allocation failed\n");
    int x;
    for (int i = 0; i < n; i++) {
        scanf("%d", &x);
        x = (x < 1) ? 1 : (x > L ? L : x);
        W[i] = x;
    }
    return W;
}

char* samplestring(char* alfabeto, int h) {
    char* array = malloc((h + 1) * sizeof(char));
    int L = strlen(alfabeto);
    for (int i = 0; i < h; i++) {
        array[i] = alfabeto[get_random() % L];
    }
    array[h] = 0;
    return array;
}

char** getstringarray(int* W, char* alfabeto, int n) {
    char** Q = malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++) {
        Q[i] = samplestring(alfabeto, W[i]);
    }
    return Q;
}

nodo* getstack(char** Q, int n) {
    nodo* head = NULL;
    push(&head, Q[0]);
    for (int i = 1; i < n; i++) {
        char* to_insert;
        if (strlen(Q[i]) % 2 != 0) {
            char* top = pop(&head);
            to_insert = malloc(strlen(Q[i]) + strlen(top) + 1);
            strcpy(to_insert, top);
            strcat(to_insert, Q[i]);
            free(top);
        } else {
            to_insert = Q[i];
        }
        push(&head, to_insert);
    }
    return head;
}

void writestacktofile(nodo* head, char* output) {
    FILE* fp = fopen(output, "w");
    if (!fp) error("Errore durante l'apertura del file\n");
    nodo* current = head;
    while (current) {
        fprintf(fp, "%s\n", current->value);
        current = current->next;
    }
    fclose(fp);
}

int main(int argc, char** argv) {
    params p = readinput(argc, argv);
    int* W = getValue(p.n, strlen(p.alfabeto));
    char** Q = getstringarray(W, p.alfabeto, p.n);
    nodo* head = getstack(Q, p.n);
    writestacktofile(head, p.output);

    for (int i = 0; i < p.n; i++) {
        free(Q[i]);
    }
    free(Q);
    free(W);

    return 0;
}
