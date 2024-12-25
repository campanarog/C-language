#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* filename_input;
    int n;
    int m;
} Params;

Params readInput(int argc, char** argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <filename_input> <n> <m>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    Params p;
    p.filename_input = argv[1];
    p.n = atoi(argv[2]);
    p.m = atoi(argv[3]);

    if (p.n < 5 || p.n > 10) {
        fprintf(stderr, "N must be in the range 5-10\n");
        exit(EXIT_FAILURE);
    }
    if (p.m < 5 || p.m > 10) {
        fprintf(stderr, "M must be in the range 5-10\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

double** allocateMatrix(int rows, int cols) {
    double** matrix = (double**)calloc(rows, sizeof(double*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double*)calloc(cols, sizeof(double));
    }
    return matrix;
}

void fillMatrix(const char* filename, double** matrix, int rows, int cols) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open file\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(fp, "%lf", &matrix[i][j]);
        }
    }
    fclose(fp);
}

void printMatrix(double** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%lf ", matrix[i][j]);
        }
        printf("\n");
    }
}

void normalize(double** source, double** dest, int rows, int cols) {
    for (int j = 0; j < cols; j++) {
        double max = source[0][j];
        for (int i = 1; i < rows; i++) {
            if (source[i][j] > max) {
                max = source[i][j];
            }
        }
        for (int i = 0; i < rows; i++) {
            dest[i][j] = source[i][j] / max;
        }
    }
}

void sortMatrixCols(double** matrix, int rows, int cols) {
    for (int j = 0; j < cols; j++) { // Columns
        for (int i = 0; i < rows - 1; i++) { // Rows
            for (int k = 0; k < rows - i - 1; k++) { // Bubble Sort for each column
                if (matrix[k][j] > matrix[k + 1][j]) {
                    double temp = matrix[k][j];
                    matrix[k][j] = matrix[k + 1][j];
                    matrix[k + 1][j] = temp;
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    Params p = readInput(argc, argv);
    int N = p.n;
    int M = p.m;
    
    double **A = allocateMatrix(N, M);
    fillMatrix(p.filename_input, A, N, M);

    printf("** A **\n");
    printMatrix(A, N, M);

    double **B = allocateMatrix(N, M);
    normalize(A, B, N, M);

    printf("** B **\n");
    printMatrix(B, N, M);

    sortMatrixCols(B, N, M);

    printf("** B (sorted columns) **\n");
    printMatrix(B, N, M);

    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
    }
    free(A);
    free(B);

    return 0;
}
