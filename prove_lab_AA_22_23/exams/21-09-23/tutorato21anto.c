
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    int min;
    int n;
    int max;
}params;

unsigned int get_random() {
    static unsigned int m_w = 424242;
    static unsigned int m_z = 242424;
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}

params readinput( int argc, char **argv){
    if(argc != 4){
        fprintf(stderr,"Usage %s <filename>,<min>, <max>, <n>", argv[0]);
        exit(EXIT_FAILURE);
        /* se metto %s, argv[0], altrimenti nulla*/
    }
        params p;
        p.min= atoi(argv[1]);
        p.max = atoi(argv[2]);
        p.n = atoi(argv[3]);
        if(p.min < 7 || p.min > 12){
            fprintf(stderr, "I parametri messi devono rispettare un range tra 7 e 12");
            exit(EXIT_FAILURE);
        }
        if(p.max < 7 || p.max > 12){
            fprintf(stderr, "valore max errato");
            exit(EXIT_FAILURE);
        }
        if(p.min >= p.max){
            fprintf(stderr, "relazione min max arrata");
            exit(EXIT_FAILURE);
        }
        if(p.n<= 0){
            fprintf(stderr, "valore di n diverso da 0");
            exit(EXIT_FAILURE);
        }
    return p;
}
char *generateString(int x){
    char vocali[]= "aeiou";
    char *row = malloc((x+1) * sizeof(char));
    /* la stringa va a n-1 , siccome dev'essere la i esima devo fare x+1 */
    for(int i =0 ; i<x; i++){
        row[i]= vocali[get_random()%5];
    }

    return row;
}

char **array(params p) {
    char **A = calloc(p.n, sizeof(char*));
    /* p.n è la dimensione */
    for(int i=0; i<p.n; i++){
        int x = get_random()%(p.max-p.min+1)+p.min;
        A[i] = generateString(x);
    }
    return A;
}

void sortArray(char **A, int n) {
    // scorri tutte le sottosequenze ordinate che
    // iniziano con zero 
    for(int i=1; i<n; i++) {
        // data la sottosequenza che inizia in zero
        // e finisce in i-1, l'obiettivo è di
        // inserire il valore a[i] nella posizione
        // corretta della sottosequenza ordinata
        char* val = A[i]; //conservo il valore di a[i] (lo sovrascriveremo)
        int j = i-1; //parto dall'ultimo elemento della sottosequenza
        while(j>=0 && strcmp(A[j],val)>0){ //finché val è più piccolo dell'elemento corrente
            A[j+1] = A[j]; //sposto l'elemento "in avanti"
            j--;
        }
        A[j+1] = val; //inserisco val nella posizione corretta
    }
}


char* concatString(char **A, params p){
    char *B = malloc(sizeof(char)* (p.n *(p.max+1)));
    /* perché ho n righe e max+1 colonne e creo un array b dove sarà in questo caso un array di 3x3 però sottoforma di un array singolo*/
    for(int i =0; i<p.n; i++){
        strcat(B, A[i]);

    }

    return B;
}
void replaceCharacters(char*B, int len){
    for(int i = 1; i<len; i++){
        if(B[i]=='u' && B[i-1] == 'e'){
            B[i]= '*';
        }
    }
}

int main(int argc, char **argv){
    params p = readinput(argc, argv);
    printf("\n---------------PUNTO A --------------\n");
    printf("min = %d, max=%d, n=%d", p.min, p.max, p.n);
    printf("\n-----------------------------\n");

    char **A= array(p);
    printf("\n---------------PUNTO B--------------\n");
    for(int i =0; i<p.n; i++){
        printf("%s\n", A[i]);
    }
    printf("\n-----------------------------\n");

    sortArray(A, p.n);
    char *B = concatString(A,p);
    printf("\n---------------PUNTO C--------------\n");
    printf("%s\n",B);

    replaceCharacters(B,strlen(B));
    printf("\n---------------PUNTO D--------------\n");
    printf("%s\n",B);
    

    return 0;
}