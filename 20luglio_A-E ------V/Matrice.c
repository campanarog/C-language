
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    char *input;
    int n;
    int m;
}params;

typedef struct{
    int rows;
    int cols;
}dimensions;

params readinput(int argc, char**argv){
    if(argc != 4){
        fprintf(stderr,"Usage :: %s Params <n>, <m>", argv[0]);
        exit(EXIT_FAILURE);
    }
    params p;
    p.input = argv[1];
    p.n = atoi(argv[2]);
    p.m = atoi(argv[3]);
    if(p.n < 5 || p.n >10){
        fprintf(stderr,"Params between 5-10\n");
        exit(EXIT_FAILURE);
    }   
    if(p.m < 5 || p.m >10){
        fprintf(stderr,"Params between 5-10\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

dimensions getdimension(char *input){
    FILE *fp = fopen(input,"r");
    if(!fp){
        fprintf(stderr,"File cannot be opened\n");
        exit(EXIT_FAILURE);
    }
    dimensions d;
    d.rows = 0;
    d.cols = 0;
    char buffer[255];
    while(fgets(buffer, 255, fp)){
        d.rows++;
    }
    for(int i=0; i<255; i++){
        if(buffer[i]== ' '){
            d.cols++;
        }
    }
    printf(" ROWS = %d COLS = %d", d.rows , d.cols);
    puts("\n");
    fclose(fp);
    return d;
}

double***allocatematrix(int n , int m){
    double***A = malloc(sizeof(double**)*n);
    if(A== NULL){
        fprintf(stderr,"Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<n; i++){
        A[i]= malloc(sizeof(double*)*m);
        if(A[i]== NULL){
            fprintf(stderr,"Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        for(int j=0; j<m; j++){
            A[i][j]= malloc(sizeof(double));
            if(A[i][j]== NULL){
                fprintf(stderr,"Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    return A;
}
double***fillmatrix(params *p){
    FILE *fp = fopen(p->input, "r");
    if(!fp){
        fprintf(stderr,"File cannot be opened\n");
        exit(EXIT_FAILURE);
    }
    double***A = allocatematrix(p->n, p->m);
    for(int i=0; i<p->n; i++){
        for(int j=0; j<p->m; j++){
            if(fscanf(fp, "%lf", A[i][j]) == EOF){
                fprintf(stderr,"File cannot be opened\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(fp);
    return A;
}

void printmatrix(double***A, int n , int m){
    for(int i=0; i<n; i++){
        for(int j =0; j<m; j++){
            if(A[i][j]){
                printf("%lf", *A[i][j]);
            }
        }
        printf("\n");
    }
}
double***normalize(double***A, int n , int m){
    double***B = allocatematrix(n,m);
    for(int j=0; j<m; j++){
        double MAX_A_j= 0;
        for(int i=0; i<n; i++){
            if(MAX_A_j < *A[i][j]){
                MAX_A_j = *A[i][j];
            }
        }
        for(int i=0; i<n; i++){
            *B[i][j]= (*A[i][j])/ MAX_A_j;
        }
    }
    return B;
}


void sortmatrixcolon(double***B, int n , int m){
    /*insertion sort*/
    for(int j=0; j<m; j++){
        for(int i =0; i<n; i++){
            int min = i;
            for(int k = i+1; k<n; k++){
                if(*B[min][j]> *B[k][j]){
                    min = k;
                }
            }
            double tmp = *B[i][j];
            *B[i][j]= *B[min][j];
            *B[min][j]= tmp;
        }
    }
}


void destroymatrix(double***matrix, int n , int m){
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            free(matrix[i][j]);
            matrix[i][j]= NULL;
        }
        free(matrix[i]);
        matrix[i]= NULL;
    }
    free(matrix);
    matrix = NULL;
}

int main(int argc, char**argv){
    params p = readinput(argc, argv);

    dimensions d = getdimension(p.input);
    
    double***A = fillmatrix(&p);
    printf("**A**\n");
    printmatrix(A, p.n, p.m);


    double***B = normalize(A, p.n, p.m);
    printf("\n** B**\n");
    printmatrix(B, p.n, p.m);

    sortmatrixcolon(B, p.n, p.m);
    printf("\n** B (sorted columns)**\n");
    printmatrix(B, p.n, p.m);


    destroymatrix(A, p.n, p.m);
    destroymatrix(B, p.n, p.m);

}