#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MSL 30

typedef struct {
    char* input;
    int k;
} params;

typedef struct nodo {
    char* data;
    struct nodo* next;
} nodo;

params readinput(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage %s <input> <k>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    params p;
    p.input = argv[1];
    p.k = atoi(argv[2]);
    if (p.k < 5 || p.k > 15) {
        fprintf(stderr, "k must be in range 5-15\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

void pushword(nodo** top, char* data) {
    nodo* n = (nodo*)malloc(sizeof(nodo));
    if (n == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    n->data = strdup(data);
    if (n->data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    n->next = *top;
    *top = n;
}

char* popword(nodo** top) {
    if (*top == NULL) {
        return NULL;
    }
    nodo* tmp = *top;
    char* data = strdup(tmp->data);  // Duplicate the data before freeing the node
    if (data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    *top = (*top)->next;
    free(tmp->data);
    free(tmp);
    return data;
}

void buildstack(nodo** top, const char* input) {
    FILE* fp = fopen(input, "r");
    if (!fp) {
        fprintf(stderr, "File cannot be opened\n");
        exit(EXIT_FAILURE);
    }
    char buffer[MSL];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline character
        pushword(top, buffer);
    }
    fclose(fp);
}

void printstack(nodo** top) {
    while (*top != NULL) {
        char* ptr = popword(top);
        printf("%s\n", ptr);
        free(ptr);
    }
}

int main(int argc, char** argv) {
    params p = readinput(argc, argv);
    nodo* top = NULL;
    buildstack(&top, p.input);
    printstack(&top);
    return 0;
}