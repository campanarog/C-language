#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 30

typedef struct {
    char *filename_input;
} Parameters;

Parameters readInput(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename_input> <number_of_strings>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    Parameters params;
    params.filename_input = argv[1];
    return params;
}

typedef struct Node {
    char data[BUFFER_SIZE];
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    unsigned numElements;
} Queue;

void initializeQueue(Queue *q) {
    q->head = q->tail = NULL;
    q->numElements = 0;
}

void enqueueWord(Queue *q, const char *buffer) {
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strncpy(newNode->data, buffer, BUFFER_SIZE);
    newNode->next = NULL;

    if (q->head == NULL) {
        q->head = q->tail = newNode;
    } else {
        q->tail->next = newNode;
        q->tail = newNode;
    }
    q->numElements++;
}

void dequeueWord(Queue *q, char *buffer) {
    if (q->head == NULL) {
        fprintf(stderr, "Queue is empty\n");
        return;
    }

    Node *temp = q->head;
    q->head = q->head->next;
    if (q->head == NULL) {
        q->tail = NULL;
    }
    strncpy(buffer, temp->data, BUFFER_SIZE);
    free(temp);
    q->numElements--;
}

void buildStringArray(Queue *q, char **array, int numStrings) {
    char buffer[BUFFER_SIZE];
    for (int i = 0; i < numStrings; i++) {
        if (q->head == NULL) {
            fprintf(stderr, "Not enough strings in queue\n");
            break;
        }
        dequeueWord(q, buffer);
        array[i] = malloc(BUFFER_SIZE * sizeof(char));
        if (array[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        strncpy(array[i], buffer, BUFFER_SIZE);
    }
}

void sortArray(char **array, int size) {
    for (int i = 1; i < size; i++) {
        int j = i - 1;
        char *tmp = array[i];
        while (j >= 0 && strcmp(tmp, array[j]) < 0) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = tmp;
    }
}

void printArray(char **array, int size) {
    for (int i = 0; i < size; i++) {
        printf("%s\n", array[i]);
    }
}

int main(int argc, char *argv[]) {
    Parameters params = readInput(argc, argv);

    Queue q;
    initializeQueue(&q);

    FILE *fp = fopen(params.filename_input, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file for reading\n");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    int numStrings;
    if (sscanf(argv[2], "%d", &numStrings) != 1 || numStrings <= 0) {
        fprintf(stderr, "Invalid number of strings\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, BUFFER_SIZE, fp) != NULL && q.numElements < numStrings) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Rimuove il carattere newline
        enqueueWord(&q, buffer);
    }
    fclose(fp);

    if (q.numElements < numStrings) {
        fprintf(stderr, "Not enough strings in file\n");
        exit(EXIT_FAILURE);
    }

    char **stringArray = malloc(numStrings * sizeof(char *));
    if (stringArray == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    buildStringArray(&q, stringArray, numStrings);

    printf("\nSorted array:\n");
    sortArray(stringArray, numStrings);
    printArray(stringArray, numStrings);

    // Liberare la memoria
    for (int i = 0; i < numStrings; i++) {
        free(stringArray[i]);
    }
    free(stringArray);

    return 0;
}
