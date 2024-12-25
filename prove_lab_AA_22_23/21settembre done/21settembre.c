#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define WSL 30

typedef struct {
    char* input_filename;
    int k;
} params;

typedef struct {
    char** stack;
    int top;
    int capacity;
} Stack;

params readinput(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_filename> <k>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    params p;
    p.input_filename = argv[1];
    p.k = atoi(argv[2]);

    if (p.k < 5 || p.k > 15) {
        fprintf(stderr, "k must be in range 5-15\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

void pushword(Stack* stackino, char* word) {
    if (stackino->top == stackino->capacity - 1) {
        fprintf(stderr, "Error, stack overflow\n");
        exit(EXIT_FAILURE);
    }
    stackino->stack[++stackino->top] = strdup(word);
}

char* popword(Stack* stackino) {
    if (stackino->top == -1) {
        fprintf(stderr, "The stack is empty\n");
        exit(EXIT_FAILURE);
    }
    return stackino->stack[stackino->top--];
}

void buildstack(Stack* stackino, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "File cannot be opened\n");
        exit(EXIT_FAILURE);
    }
    char word[WSL];
    while (fscanf(fp, "%s", word) == 1) {
        pushword(stackino, word);
    }
    fclose(fp);
}

void sortword(char* word) {
    size_t len = strlen(word);
    for (size_t i = 0; i < len - 1; i++) {
        for (size_t j = 0; j < len - 1 - i; j++) {
            if (word[j] > word[j + 1]) {
                char tmp = word[j];
                word[j] = word[j + 1];
                word[j + 1] = tmp;
            }
        }
    }
}

void printstack(Stack* stackino, int k) {
    while (stackino->top != -1) {
        char* word = popword(stackino);
        if (strlen(word) >= k) {
            sortword(word);
            printf("%s\n", word);
        }
        free(word);
    }
}

int main(int argc, char** argv) {
    params p = readinput(argc, argv);
    Stack* stackino = malloc(sizeof(Stack));
    stackino->capacity = 100;
    stackino->stack = malloc(stackino->capacity * sizeof(char*));
    stackino->top = -1;

    buildstack(stackino, p.input_filename);
    printstack(stackino, p.k);

    /* Deallocate the memory allocated for the stack */
    for (int i = 0; i <= stackino->top; i++) {
        free(stackino->stack[i]);
    }
    free(stackino->stack);
    free(stackino);

    return 0;
}
