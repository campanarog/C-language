

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct{
    char* filename_input;
    char* filename_output;
}params;

params decodeParameters(int argc, char**argv){
    if(argc!= 3){
        fprintf(stderr,"Usage %s <filename_input>, <filename_output>", argv[0]);
        exit(-1);
    }
    params p;
    p.filename_input = argv[1];
    p.filename_output= argv[2];

    if(strcmp(p.filename_input+ strlen(p.filename_input)-4 ,".txt")!=0){
        fprintf(stderr, "error file must have .txt extension\n");
        exit(-1);
    }

    if(strcmp(p.filename_output+ strlen(p.filename_output)-4 ,".txt")!=0){
        fprintf(stderr, "error file must have .txt extension\n");
        exit(-1);
    }
     /* 
     oppure :::
    char* estensione = strrchr(p.filename, '.');
    if (estensione != NULL && strcmp(estensione, ".bin") == 0) {
        printf("Il file ha estensione .bin\n");
    } else {
        printf("Il file non ha estensione .bin\n");
    }
    return p;*/

    return p;
}

void printParameters(params p){
    printf("input_filename = %s\n", p.filename_input);
    printf("input_output = %s\n", p.filename_output);
    puts("\n");
}

/* devo aprire un file per leggerlo quindi*/
FILE *openfile(const char *filename, const char* modes){
    FILE *fp =fopen(filename, modes);
    if(fp==NULL){
        fprintf(stderr,"Error , file %s cannot be open \n", filename);
        exit(-1);
    }
    return fp;
}

void readheader(FILE *fp, int* n , int* m){
    fscanf(fp, "%d, %d", n, m);
    /* in questo caso essendo che voglio leggere vado proprio a puntare 
    n o m che sono gli elementi che devo leggere ,e ovviamente readheader cosa?
    il file, quindi FILE *fp che punta a file per consentire all'esecutore di leggere il file in input*/
}

double*** readfile(FILE *fp, int n, int m){
    double*** X = (double***)calloc(n, sizeof(double**));
    if (X == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(-1);
    }
    
    for(int i = 0; i < n; i++) {
        X[i] = (double**)malloc(m * sizeof(double*));
        if (X[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(-1);
        }
        
        for(int j = 0; j < m; j++) {
            X[i][j] = (double*)malloc(sizeof(double));
            if (X[i][j] == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(-1);
            }
            if (fscanf(fp, "%lf", X[i][j]) != 1) {
                fprintf(stderr, "Error reading file\n");
                exit(-1);
            }
            printf("Read value: %lf\n", *X[i][j]);
        }
    }
    
    return X;
}


void printmatrix(FILE *fp, double ***X, int n , int m){
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            if(X[i][j]){
                fprintf(fp,"%7.2f", *X[i][j]);
            }else{
                fprintf(fp, "%7s", "*");
            }
        }
        fprintf(fp, "\n");
    }
}

double *getArray(double***X, int n, int m){
    /* imparare a memoria perché questo serve a fare la media*/
    double* Y= (double*)calloc(n,sizeof(double));
    for(int i=0; i<n; i++){/*righe*/
    Y[i]=0;
    int num =0;
    /* è una variabile che tiene traccia del numero di valori non nulli trovati nella 
    riga i della matrice X. Questo è utile per calcolare la media corretta, dividendo 
    la somma dei valori per il numero totale di valori non nulli, come è fatto successivamente nel codice.*/
    for(int j=0; j<m; j++){ /*colonne*/
        if(X[i][j]){
            Y[i]+=*X[i][j];
            num++;
         }
    } Y[i]/=num; /* calcola la media dei valori non nulli*/
        
    }
    return Y;
}

/* se c'è print matrix c'è and print array*/
void printarray(double *Array, int n){
    for(int i=0; i<n; i++){
        printf("%0.2f", Array[i]);

    }
    puts("\n");
}

void selectionSort(double *Y, int n){
    for(int passi = 0; passi<n-1; passi++){
        int idx_min = passi;
        for(int i=passi+1; i<n; i++){
            if(Y[i]<Y[idx_min]){
                idx_min=i;
         }
        }
        float tmp = Y[passi];
        Y[passi]= Y[idx_min];
        Y[idx_min]= tmp;
    }
}

void processX(double***X, double *Y, int n, int m){
    for(int i=0; i<n;i++){
        for(int j=0; j<m; j++){
            if(X[i][j] && *X[i][j]>Y[i]){
                X[i][j]= NULL;
            }
     }
    }
}

void writeToFile(const char* filename, double***X, int n, int m){
    FILE *fp =fopen("filename", "w");
    printmatrix(fp, X,n,m);
    fclose(fp);
}


int main(int argc, char** argv) {
    params p = decodeParameters(argc, argv);
    printf("-----Parametri----\n");
    printParameters(p);

    int n, m;
    FILE* fp = openfile(p.filename_input, "r");
    readheader(fp, &n, &m);
    fclose(fp);

    printf("----Valori n & m----\n n=%d, m=%d\n\n", n, m);

    double*** X = readfile(fp, n, m);
    printf("======Matrice X======\n");
    printmatrix(stdout, X, n, m);

    double* Y = getArray(X, n, m);
    printf("======Array Y======\n");
    printarray(Y, n);

    selectionSort(Y, n);
    printf("======Array Y Ordinato======\n");
    printarray(Y, n);

    processX(X, Y, n, m);
    printf("======Matrice X modificata======\n");
    printmatrix(stdout, X, n, m);

    writeToFile(p.filename_output, X, n, m);

    return 0;
}
