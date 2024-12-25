#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char* input_filename;
    char* output_filename;
} parametri;

parametri decodeParameters(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage %s, error, required <input> <output> ", argv[0]);
        exit(EXIT_FAILURE);
    }
    parametri p;
    p.input_filename = argv[1];
    p.output_filename = argv[2];
    /* se sono solo stringhe di tipo char da leggere non c'è bisogno conversione */
    if (strcmp(p.input_filename + strlen(p.input_filename) - 4, ".txt") != 0) {
        fprintf(stderr, "error: file must have '.txt extension\n");
        exit(EXIT_FAILURE);
    }
    if (strcmp(p.output_filename + strlen(p.output_filename) - 4, ".txt") != 0) {
        fprintf(stderr, "error: file must have '.txt extension\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

void stampoparametri(parametri p) {
    printf("input_filename = %s\n", p.input_filename);
    printf("output_filename = %s\n", p.output_filename);
    puts("\n");
}

FILE* op(const char* filename, const char* modes) {
    FILE* fp = fopen(filename, modes);
    if (fp == NULL) {
        fprintf(stderr, "cant open file\n");
        exit(EXIT_FAILURE);
    }
    return fp;
}

void readHeader(FILE* fp, int* n, int* m) {
    fscanf(fp, "%d, %d", n, m);
}

double*** readFile(FILE* fp, int n, int m) {
    double*** X = (double***)calloc(n, sizeof(double**));
    for (int i = 0; i < n; i++) {
        X[i] = (double**)calloc(m, sizeof(double*));
        for (int j = 0; j < m; j++) {
            X[i][j] = malloc(sizeof(double));
            fscanf(fp, "%lf", X[i][j]);
        }
    }
    return X;
}

void printmatrix(FILE* fp, double*** X, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (X[i][j]) {
                fprintf(fp, "%7.2f", *X[i][j]);
            } else {
                fprintf(fp, "%7s", "*");
            }
        }
        fprintf(fp, "\n");
    }
}

double* getarray(double*** X, int n, int m) {
    double* Y = (double*)calloc(n, sizeof(double));
    for (int i = 0; i < n; i++) {
        Y[i] = 0;
        int num = 0;
        for (int j = 0; j < m; j++) {
            if (X[i][j]) {
                Y[i] += *X[i][j];
                num++;
            }
        }
        Y[i] /= num;
    }
    return Y;
}

int main(int argc, char** argv) {
    parametri p = decodeParameters(argc, argv);
    printf("------------A PARAMETRI ----------------\n");
    stampoparametri(p);

    int n, m;
    FILE* fp = op(p.input_filename, "r");
    readHeader(fp, &n, &m);
    printf("--------B VALORI n & m---------\n n=%d m = %d", n, m);

    double*** X = readFile(fp, n, m);
    fclose(fp);
    printf("---------B MATRIX--------\n");
    printmatrix(stdout, X, n, m);

    double* Y = getarray(X, n, m);
    printf("======C Array Y======\n");
    for (int i = 0; i < n; i++) {
        printf("%7.2f\n", Y[i]);
    }
    return 0;
}


