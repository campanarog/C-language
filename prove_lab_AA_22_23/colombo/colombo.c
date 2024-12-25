

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING_LENGTH 20
#define ARRAY_SIZE 200

typedef struct {
    char key[MAX_STRING_LENGTH + 1]; // +1 per il terminatore null
    int length;
} Record;

typedef struct {
    char inputFile[100];
    char outputFile[100];
} FileNames;

FileNames readInput(int argc, char *argv[]) {
    FileNames fn;
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }
    strcpy(fn.inputFile, argv[1]);
    strcpy(fn.outputFile, argv[2]);
    return fn;
}

void buildArray(Record array[], const char *inputFile) {
    FILE *file = fopen(inputFile, "r");
    if (!file) {
        perror("Error opening input file");
        exit(1);
    }

    int i = 0;
    while (i < ARRAY_SIZE && fscanf(file, "%20s", array[i].key) == 1) {
        array[i].length = strlen(array[i].key);
        i++;
    }

    for (; i < ARRAY_SIZE; i++) {
        array[i].length = -1;
    }

    fclose(file);
}

void filter(Record array[]) {
    for (int i = 0; i < ARRAY_SIZE && array[i].length != -1; i++) {
        char temp[MAX_STRING_LENGTH + 1];
        int k = 0;
        for (int j = 0; j < array[i].length; j++) {
            if (!ispunct(array[i].key[j])) {
                temp[k++] = array[i].key[j];
            }
        }
        temp[k] = '\0';
        strcpy(array[i].key, temp);
        array[i].length = k;
    }
}

void createSet(Record array[]) {
    for (int i = 0; i < ARRAY_SIZE && array[i].length != -1; i++) {
        for (int j = i + 1; j < ARRAY_SIZE && array[j].length != -1; j++) {
            if (strcmp(array[i].key, array[j].key) == 0) {
                array[j].length = -1;
            }
        }
    }
}

void printArray(const Record array[]) {
    for (int i = 0; i < ARRAY_SIZE && array[i].length != -1; i++) {
        if (array[i].length > 0) {
            printf("Record %d: %s (length: %d)\n", i, array[i].key, array[i].length);
        }
    }
}

void save(const Record array[], const char *outputFile) {
    FILE *file = fopen(outputFile, "wb");
    if (!file) {
        perror("Error opening output file");
        exit(1);
    }

    for (int i = 0; i < ARRAY_SIZE && array[i].length != -1; i++) {
        fwrite(&array[i], sizeof(Record), 1, file);
    }

    fclose(file);
}

void read(const char *outputFile) {
    FILE *file = fopen(outputFile, "rb");
    if (!file) {
        perror("Error opening output file");
        exit(1);
    }

    Record record;
    while (fread(&record, sizeof(Record), 1, file)) {
        if (record.length > 0) {
            printf("Record: %s (length: %d)\n", record.key, record.length);
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    FileNames files = readInput(argc, argv);

    Record array[ARRAY_SIZE];
    buildArray(array, files.inputFile);
    filter(array);
    createSet(array);

    printf("Filtered and deduplicated array:\n");
    printArray(array);

    save(array, files.outputFile);

    printf("\nReading from binary file:\n");
    read(files.outputFile);

    return 0;
}
