
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MSL 30

typedef struct {
    char* input;
}params;

params readinput(int argc, char**argv){
    if(argc != 3){
        fprintf(stderr,"Usage :: %s  :: <input.txt> , <N>", argv[0]);
        exit(EXIT_FAILURE);
    }
    params p;
    p.input = argv[1];
    return p;
}

typedef struct nodo{
    struct nodo* next;
    char data[MSL];
}nodo;

typedef struct{
    nodo* head;
    nodo* tail;
}queue;

void inizializzocoda(queue*q){
    q->head = q->tail = NULL;
}

void enqueueword(queue*q, const char* data){
    nodo *n = (nodo*)malloc(sizeof(nodo));
    if(n == NULL){
        fprintf(stderr,"Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strncpy(n->data, data , MSL);
    n->data[MSL-1] = '\0';
    n->next = NULL;
    if(q->head == NULL){
        q->head = q->tail = n;
    }else{
        q->tail->next = n;
        q->tail = n;
    }
}
void dequeueword(queue*q, char*data){
    if(q->head == NULL){
        fprintf(stderr,"There's nothing to get\n");
        exit(EXIT_FAILURE);
    }
    nodo *tmp = q->head;
    q->head = q->head->next;
    if(q->head == NULL){
        q->tail = NULL;
    }
    strncpy(data, tmp->data, MSL);
    free(tmp);
}
void buildqueue(queue*q, const char* input, int *n){
    FILE *fp = fopen(input, "r");
    if(!fp){
        fprintf(stderr,"File cannot be opened\n");
        exit(EXIT_FAILURE);
    }
    char buffer[MSL];
    *n = 0;
    while(fscanf(fp, "%s", buffer)!= EOF){
        buffer[strcspn(buffer, "\n")]='\0';
        enqueueword(q, buffer);
        (*n)++;
    }
    fclose(fp);
}

char** buildstringarray(queue *q, int n) {
    // Alloca memoria per un array di n puntatori a char (ossia, n stringhe)
    char** nuovo_array = (char**)malloc(n * sizeof(char*));
    // Verifica che l'allocazione sia avvenuta con successo
    if (nuovo_array == NULL) {
        // Se l'allocazione fallisce, stampa un messaggio di errore e termina il programma
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    // Ciclo per allocare memoria per ciascuna stringa nell'array
    for (int i = 0; i < n; i++) {
        // Alloca memoria per una stringa di lunghezza massima MSL caratteri
        nuovo_array[i] = (char*)malloc(MSL * sizeof(char));
        // Verifica che l'allocazione della singola stringa sia avvenuta con successo
        if (nuovo_array[i] == NULL) {
            // Se l'allocazione fallisce, stampa un messaggio di errore e termina il programma
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }   
        // Estrae una parola dalla coda `q` e la copia in `nuovo_array[i]`
        dequeueword(q, nuovo_array[i]);
    }
    // Restituisce il puntatore all'array di stringhe, ora popolato con le parole estratte dalla coda
    return nuovo_array;
}
void sortarray(char**array, int size){
    for(int i =1; i<size; i++){
        int j = i-1;
        char *tmp = array[i]; // perché lavoriamo su array.. altrmenti no bisogno, cercare per int e imparare
        while(j >=0 && strcmp(tmp, array[j])<0){
            array[j+1] = array[j];
            j--;
        }
        array[j+1] = tmp;
    }
}
void printarray(char**array, int n){
    for(int i =0; i<n; i++){
        printf("%s\n", array[i]);
    }
}
int main(int argc, char**argv){
    params p = readinput(argc, argv);
    int n; // tiene traccia dei valori letti..
    queue q;
    inizializzocoda(&q);
    buildqueue(&q, p.input, &n); // p.input è già un puntatore a char (stringa), gli altri non essendo necessitano &
    char** W = buildstringarray(&q, n);
    sortarray(W, n);
    printarray(W, n);

    for (int i = 0; i < n; i++) {
        free(W[i]);
    }
    free(W);
    return 0;
}

