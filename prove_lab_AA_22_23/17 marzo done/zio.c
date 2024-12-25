#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int a, b;
    double alpha, beta;
} params;

unsigned int get_random() {
    static unsigned int m_w = 123456;
    static unsigned int m_z = 789123;
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}
double randd(){
    return (double)get_random()/UINT_MAX;
}

params readinput(int argc, char**argv){
    if(argc != 5){
        fprintf(stderr,"Usage %s <a> <b> <alpha> <beta>", argv[0]);
        exit(EXIT_FAILURE);
    }
    params p;
    p.a =atoi(argv[1]);
    p.b = atoi(argv[2]);
    p.alpha = atof(argv[3]);
    p.beta = atof(argv[4]);
    if ((p.a < 5 || p.a > 20) || (p.b < 5 || p.b > 20)) {
        fprintf(stderr, "A & B must be in range 5-20\n");
        exit(EXIT_FAILURE);
    }
    if (p.alpha < 10 || p.alpha > 20) {
        fprintf(stderr, "Alpha must be in range 10-20\n");
        exit(EXIT_FAILURE);
    }
    if (p.beta < 1 || p.beta > 5) {
        fprintf(stderr, "Beta must be in range 10-20\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

double **allocatematrix(int n, int m){
    double** matrix= (double**)malloc(n*sizeof(double*));
    if(matrix == NULL){
        fprintf(stderr,"memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for(int i =0; i<n; i++){
        matrix[i]= (double*)malloc(m*sizeof(double));
        if(matrix[i]== NULL){
            fprintf(stderr,"failed to allocate matrix rows\n");
            exit(EXIT_FAILURE);
        }
    }
    return matrix;
}

void fillmatrix(double**matrix, int n , int m , double alpha , double beta){
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            double p = randd();
            if(p > 0.2){
                double value = (randd ()* (2*beta)) + (alpha-beta);
                matrix[i][j]= value;
            }else{
                matrix[i][j]= 0.0;
            }
        }
    }
}

double** traspostomatrix(double**matrix, int n , int m){
    double** trasposto= allocatematrix(m,n);
    for(int i =0; i<n; i++){
        for(int j=0; j<m; j++){
            trasposto[j][i]= matrix[i][j];
        }
    }
    return trasposto;
}

void freematrix(double **matrix, int n){
    for(int i=0; i<n; i++){
        free(matrix[i]);
    }
    free(matrix);
}

void printmatrix(double**matrix, int n , int m){
    for(int i =0; i<n; i++){
        for(int j=0; j<m; j++){
            printf("|%0.2f|", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char**argv){
    params p = readinput(argc, argv );
    double**matrix = allocatematrix(p.a ,p.b);
    fillmatrix(matrix , p.a, p.b, p.alpha, p.beta);    
    printf("-------Original matrix: (6x10)-------\n");
    printmatrix(matrix, p.a, p.b);
    // Transpose matrix
    double** trasposto = traspostomatrix(matrix, p.a, p.b);
    // Print transposed matrix
    printf("------ Transposed matrix: (10x6)------- \n");
    printmatrix(trasposto, p.b, p.a);

    // Free matrices
    freematrix(matrix, p.a); /* a rows*/
    freematrix(trasposto, p.b); /* b rows , the swapped ones*/

}