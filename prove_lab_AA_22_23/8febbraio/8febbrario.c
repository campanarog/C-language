#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX_LENGTH 255

typedef struct {
    char *filename_input;
    char *filename_output;
} Params;

Params readinput(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    Params p;
    p.filename_input = argv[1];
    p.filename_output = argv[2];

    if (strlen(p.filename_input) > MAX_LENGTH || strlen(p.filename_output) > MAX_LENGTH) {
        fprintf(stderr, "Error: File names must not exceed %d characters\n", MAX_LENGTH);
        exit(EXIT_FAILURE);
    }
    return p;
}

double **matrix(const char *filename, int n, int m) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "File cannot be opened\n");
        exit(EXIT_FAILURE);
    }
    double **matrix = (double **)malloc(n * sizeof(double *));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        matrix[i] = (double *)malloc(m * sizeof(double));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < m; j++) {
            if (fscanf(fp, "%lf", &matrix[i][j]) != 1) {
                fprintf(stderr, "Error reading from file\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(fp);
    return matrix;
}

void sparsify(double **matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        double rowSum = 0.0;
        int validValuesCount = 0;
        // Calculate the sum of valid values in the row
        for (int j = 0; j < m; j++) {
            if (!isnan(matrix[i][j])) {
                rowSum += matrix[i][j];
                validValuesCount++;
            }
        }
        
        // Calculate the mean of the row
        double rowMean = rowSum / validValuesCount;
        
        // Eliminate values below the mean
        for (int j = 0; j < m; j++) {
            if (!isnan(matrix[i][j]) && matrix[i][j] < rowMean) {
                matrix[i][j] = NAN;
            }
        }
    }
}

double *collect(double **matrix, int n, int m, int *size) {
    *size = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (!isnan(matrix[i][j])) {
                (*size)++;
            }
        }
    }

    double *array = (double *)malloc((*size) * sizeof(double));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    int k = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (!isnan(matrix[i][j])) {
                array[k++] = matrix[i][j];
            }
        }
    }

    return array;
}

void writeToFile(const char *filename, double *array, int size) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%.6lf ", array[i]);
    }

    fclose(file);
}

int main(int argc, char **argv) {
    Params parameters = readinput(argc, argv);

    int n = 6; // Number of rows
    int m = 5; // Number of columns

    double **matrix_data = matrix(parameters.filename_input, n, m);

    sparsify(matrix_data, n, m);

    int size;
    double *array = collect(matrix_data, n, m, &size);

    writeToFile(parameters.filename_output, array, size);

    // Free memory
    for (int i = 0; i < n; i++) {
        free(matrix_data[i]);
    }
    free(matrix_data);
    free(array);

    return 0;
}
