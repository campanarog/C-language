
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 255
typedef struct{
    char* IN;
    char* OUT;
}params;

params readparams(int argc, char**argv){
    if(argc != 3){
        fprintf(stderr,"Error :: %s <IN>, <OUT>", argv[0]);
        exit(EXIT_FAILURE);
    }
    params p;
    p.IN = argv[1];
    p.OUT = argv[2];
    return p;
}
unsigned int get_random() {
    static unsigned int m_w = 424242;
    static unsigned int m_z = 242424;
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}

typedef struct{
    int n;
    int m;
}dimensions;


dimensions getdimension(char *IN){
    /* Mi serve a dare in output quante righe e colonne sono presenti nel file*/
    FILE *fp = fopen(IN, "r");
    if(!fp){
        fprintf(stderr,"Error\n");
        exit(EXIT_FAILURE);
    }
    dimensions d;
    d.n =0;
    d.m =0;
    char buffer[SIZE];
    while(fgets(buffer,SIZE,fp)){
        d.n++;
    }
    for(int i=0; i<SIZE+1; i++){
        if(buffer[i]== ' '){
            d.m++;
        }
    }
    printf("rows=%d cols=%d\n", d.n, d.m);
    fclose(fp);
    return d;
}

double***creatematrix(char* input, int n , int m){
    FILE *fp = fopen(input,"r");
    if(!fp){
        fprintf(stderr,"Error\n");
        exit(EXIT_FAILURE);
    }
    double***A = malloc(sizeof(double**)*n);
    if(A== NULL){
        fprintf(stderr,"Error\n");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<n;i++){
        A[i]= malloc(sizeof(double*)*m);
            if(A[i]== NULL){
                fprintf(stderr,"Error\n");
                exit(EXIT_FAILURE);
            }
        for(int j=0; j<m; j++){
            A[i][j]= malloc(sizeof(double));
            if(A[i][j]== NULL){
                fprintf(stderr,"Error\n");
                exit(EXIT_FAILURE);
            }
            if(fscanf(fp,"%lf", A[i][j])== EOF){
                fprintf(stderr,"Error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(fp);
    return A;
}

void printmatrix(double***A, int n, int m){
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            if(A[i][j]){
                printf("%lf", *A[i][j]);
            }
        }
        printf("\n");
    }
}

int sparsify(double***A, int n , int m){
    int elementi = 0; // NON NULLI DELLA MATRICE;
    for(int i=0; i<n; i++){
        double media = 0;
        for(int j=0; j<m; j++){
            media += *A[i][j];
        }
        media = media/m;
        for(int j=0; j<m; j++){
            if(media> *A[i][j]){
                free(A[i][j]);
                A[i][j]= NULL;
            }
            if(A[i][j]){
                elementi++;
            }
        }
    }
    return elementi;
}

double* collect(double***A, int n , int m, int elementi){
    double* B = malloc(sizeof(double)*elementi); // creo un array ed inserisco i valori non vuoti , ovvero elementi ...
    if(B== NULL){
        fprintf(stderr,"Error\n");
        exit(EXIT_FAILURE);
    }
    int k =0;
    for(int i =0; i<n; i++){
        for(int j=0; j<m; j++){
            if(A[i][j]){
                B[k]= *A[i][j];
                k++;
            }
        }
   }
   return B;
}

void writetofile(char* output, double* B, int elementi){

    FILE *f=fopen(output,"w");
    if(f==NULL){
        fprintf(stderr,"Errore nell'apertura del file\n");
        exit(EXIT_FAILURE);
    }

 for(int i = 0; i < elementi; i++){
    printf("%lf  ", B[i]); // Aggiungi questa linea per il debug
 }


    fclose(f);
}

void destroymatrix(double***A, int n, int m){
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            free(A[i][j]);
            A[i][j]= NULL;
        }
        free(A[i]);
        A[i]= NULL;
    }
    free(A);
}


int main(int argc, char **argv){

    params p= readparams(argc,argv);
    //printf("IN=%s OUT=%s\n", p.IN, p.OUT);

    //int n=0;
    //int m=0;
    dimensions d=getdimension(p.IN);//,&n,&m);
    //printf("n=%d m=%d\n",d.n, d.m);
    
    double ***A=creatematrix(p.IN,d.n,d.m);
    //printMatrix(A,d.n,d.m);
    
    int elem=sparsify(A,d.n,d.m);
    //printMatrix(A,d.n,d.m);
    //printf("\nelem=%d\n",elem);
    
    double *B=collect(A,d.n,d.m,elem);
    /*for(int k=0; k<elem; k++){
        printf("%lf ",B[k]);
    }*/
    
    writetofile(p.OUT,B,elem);

    destroymatrix(A,d.n,d.m);
    free(B);
    return 0;
}