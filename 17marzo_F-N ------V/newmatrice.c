
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

typedef struct{
    int a ,b;
    double alpha,beta;
}params;

void error(const char*err){
    fprintf(stderr,"%s\n", err);
    exit(EXIT_FAILURE);
}
unsigned int get_random() {
    static unsigned int m_w = 123456;
    static unsigned int m_z = 789123;
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}
params readinput(int argc, char**argv){
    if(argc != 5) error("Error :: params to insert :: a, b, alpha, beta\n");
    params p;
    p.a = atoi(argv[1]);
    p.b = atoi(argv[2]);
    p.alpha=atof(argv[3]);
    p.beta =atof(argv[4]);
    if(p.a < 5 ||  p.a > 20) error("A must be in range 5-20");
    if(p.b < 5 ||  p.b> 20) error("B must be in range 5-20");
    if(p.alpha< 10 || p.alpha > 20) error("alpha must be in range 10-20");
    if(p.beta < 1 || p.beta > 5) error("beta must be in range 1-5");
    return p;
}

double***allocatematrix(int a , int b){
    double***matrix = (double***)malloc(sizeof(double**)*a);
    if(matrix== NULL) error("memory allocation failed\n");
    for(int i=0; i<a; i++){
        matrix[i]= (double**)malloc(sizeof(double*)*b);
        if(matrix[i]== NULL) error("memory allocation failed\n");
        for(int j=0; j<b; j++){
            matrix[i][j]= (double*)malloc(sizeof(double));
            if(matrix[i][j]== NULL) error("memory allocation failed\n");
        }
    }
    return matrix;
}

void fillmatrix(double***matrix, params *p){
    double I = (p->alpha+ p->beta) -(p->alpha - p->beta);
    for(int i =0; i<p->a; i++){
        for(int j =0 ; j<p->b; j++){
            double prandom= get_random() / (RAND_MAX* 2.0);
            if(prandom >0.2){
                *matrix[i][j] = get_random() / (RAND_MAX / I+ p->alpha);
            }else{
                free(matrix[i][j]);
                matrix[i][j] = NULL;
            }
        }
    }
}
double*** trasposematrix(double*** matrix, int a, int b) {
    double*** matrixT = allocatematrix(b, a);
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < b; j++) {
            if (matrix[i][j]) {
                *matrixT[j][i] = *matrix[i][j];
            } else {
                free(matrixT[j][i]);
                matrixT[j][i] = NULL;
            }
        }
    }
    return matrixT;
}

void printmatrix(double*** matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (matrix[i][j]) {
                printf("%5.2lf ", *matrix[i][j]);  //distanza + la virgola mobile quanti valori vogliamo vedere
            } else {
                printf("%5c ", 'N');  // la distanza fra il primo e il successivo
            }
        }    
         printf("\n");      
    }
}

void destroymatrix(double***matrix, int a, int b){
    for(int i=0; i<a; i++){
        for(int j=0; j<b; j++){
            if(matrix[i][j]){
                free(matrix[i][j]);
            }
        }
        free(matrix[i]);
    }
    free(matrix);
}

int main(int argc, char**argv){
    params p = readinput(argc, argv);
    double***matrix = allocatematrix(p.a, p.b);
    fillmatrix(matrix, &p);
    printf("*** Matrice m (%d x %d) ***\n", p.a, p.b);
    printmatrix(matrix, p.a, p.b);

    double*** m1 = trasposematrix(matrix, p.a, p.b);
    printf("*** Matrice trasposta m1 (%d x %d) ***\n", p.b, p.a);
    printmatrix(m1, p.b, p.a);

    destroymatrix(matrix, p.a, p.b);
    destroymatrix(m1, p.b, p.a);
}
