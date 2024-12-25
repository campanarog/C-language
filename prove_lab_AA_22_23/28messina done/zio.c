#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MSL 30

typedef struct {
    char* input;
    char a;
    char b;
} params;

typedef struct nodo {
    char* data;
    struct nodo* next;
} nodo;

typedef struct {
    nodo* testa;
    nodo* coda;
} queue;

params readinput(int argc, char** argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage %s <input> <a> <b>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    params p;
    p.input = argv[1];  // Corretto
    p.a = argv[2][0];   // Corretto
    p.b = argv[3][0];   // Corretto
    if (p.a > p.b) {    // Corretto
        fprintf(stderr, "a can't be greater than b\n");  // Messaggio corretto
        exit(EXIT_FAILURE);
    }
    return p;
}

void enqueue(queue* q, char* data) {
    nodo* n = malloc(sizeof(nodo));
    if (n == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    n->data = malloc((strlen(data) + 1) * sizeof(char));  // Allocazione della memoria
    if (n->data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strncpy(n->data, data, MSL);
    n->data[MSL] = '\0';  // Assicurare la terminazione con null
    n->next = NULL;
    if (q->coda == NULL) {
        q->testa = n;
    } else {
        q->coda->next = n;
    }
    q->coda = n;
}

char* dequeue(queue* q) {
    if (q->testa == NULL) {
        fprintf(stderr, "Queue is empty\n");
        exit(EXIT_FAILURE);
    }
    nodo* tmp = q->testa;
    char* str = (char*)malloc((strlen(tmp->data) + 1) * sizeof(char));
    if (str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(str, tmp->data);
    q->testa = q->testa->next;
    if (q->testa == NULL) {
        q->coda = NULL;
    }
    free(tmp->data);
    free(tmp);
    return str;
}

void buildequeue(queue* q, const char* input) {
    FILE* fp = fopen(input, "r");
    if (!fp) {
        fprintf(stderr, "File cannot be opened\n");
        exit(EXIT_FAILURE);
    }
    char buffer[MSL + 1];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';  // Rimuovere il carattere di nuova linea
        enqueue(q, buffer);
    }
    fclose(fp);
}

char* elab(char* str, char a, char b) {
    char* ptr = str;
    while (*ptr != '\0') {
        if (*ptr >= a && *ptr <= b) {  // Usa i parametri a e b
            *ptr = '0';
        }
        ptr++;
    }
    return str;
}

void printqueue(queue* q, char a, char b) {
    while (q->testa != NULL) {
        char* str = dequeue(q);
        printf("%s\n", elab(str, a, b));
        free(str);
    }
}

int main(int argc, char** argv) {
    params p = readinput(argc, argv);
    queue q = { NULL, NULL };
    buildequeue(&q, p.input);
    printqueue(&q, p.a, p.b);
    while (q.testa != NULL) {
        dequeue(&q);
    }
    return 0;
}
