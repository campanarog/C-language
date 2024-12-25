
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char* input;
    int n;
    int m;
}params;

void error(char*err){
    fprintf(stderr,"%s", err);
    exit(EXIT_FAILURE);
}

params readinput(int argc, char**argv){
    if(argc != 4) error( "Parametri inseriti non corretti.. <input>, <n>, <m>");
    params p;
    p.input = argv[1];
    p.n =atoi(argv[2]);
    p.m = atoi(argv[3]);
    if(p.n< 5 || p.n> 10) error("N dev'essere nel range 5-10\n");
    if(p.m < 5 || p.m >10) error("M dev'essere nel range 5-10\n");
    return p;
}

double***allocatematrix(int n, int m){
    double***matrix = (double***)malloc(sizeof(double**)*n);
    if(matrix == NULL) error("Memory allocation failed\n");
    for(int i =0; i<n; i++){
        matrix[i] = (double**)malloc(sizeof(double*)*m);
        if(matrix[i]== NULL) error("Memory allocation failed\n");
        for(int j=0; j<m; j++){
            matrix[i][j]= (double*)malloc(sizeof(double));
            if(matrix[i][j]== NULL) error("Memory allocation failed\n");
        }
    }
    return matrix;
}

void fillmatrix(params *p, double***matrix){
    FILE *fp = fopen(p->input, "r");
    if(!fp) error( "Errore nell'apertura del file\n");

    for(int i=0; i<p->n; i++){
        for(int j =0; j<p->m; j++){
            if(fscanf(fp, "%lf", matrix[i][j]) == EOF) error("Parsing error\n");
            printf("Read Value %lf\n", *matrix[i][j]);
        }
    }
}

void destroymatrix(double***matrix, int n, int m){
    for(int i =0; i<n; i++){
        for(int j =0; j<m; j++){
            free(matrix[i][j]);
        }
        free(matrix[i]);
    }
    free(matrix);
}

void printmatrix(double***matrix,int n , int m){
    for(int i =0; i<n; i++){
        for(int j=0; j<m; j++){
            printf("%lf\t", *matrix[i][j]);
        }
        puts("\n");
    }
}
void normalize(double*** provenienza, double*** destinazione , int n , int m){
    double max;
    for(int i =0; i<m; i++){
        max = *provenienza[0][i];

        for(int j= 1; j<n; j++){
            if(*provenienza [j][i] > max){
                max = *provenienza[j] [i];
            }
        }
        for(int j =0; j<n; j++){
            *destinazione [j][i] = *provenienza[j][i] / max;
        }
    }
}

void sortmatrixcols(double*** matrix, int n, int m) {
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n - 1; i++) {
            for (int k = i + 1; k < n; k++) {
                if (*matrix[i][j] > *matrix[k][j]) {
                    double temp = *matrix[i][j];
                    *matrix[i][j] = *matrix[k][j];
                    *matrix[k][j] = temp;
                }
            }
        }
    }
}
int main(int argc, char**argv){
    params p = readinput(argc, argv);

    double***A = allocatematrix(p.n,p.m);
    fillmatrix(&p, A);

    double***B = allocatematrix(p.n,p.m);
    normalize(A,B, p.n , p.m);

    printf("\n**--------A--------**\n");
    printmatrix(A, p.n, p.m);

    printf("\n**--------B--------**\n");
    printmatrix(B, p.n, p.m);

    printf("\n** ---------B sorted columns--------**\n");
    sortmatrixcols(B, p.n, p.m);
    printmatrix(B, p.n , p.m);

    destroymatrix(A, p.n, p.m);
    destroymatrix(B, p.n, p.m);
}




  

