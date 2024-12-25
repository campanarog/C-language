
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define MSL 30

typedef struct{
    char* input;
}params;

void error(const char*err){
    fprintf(stderr,"%s", err);
    exit(EXIT_FAILURE);
}
params readinput(int argc, char**argv){
    if(argc != 3) error("Error :: insert input , integer \n");
    params p;
    p.input = argv[1];
    return p;
}
typedef struct nodo{
    char data[MSL];
    struct nodo* next;
}nodo;

typedef struct{
    nodo *head;
    nodo* tail;
}queue;

void inizializzocoda(queue*q){
    q->head = q->tail = NULL;
}

void enqueueword(queue*q, const char* data){
    nodo*n = (nodo*)malloc(sizeof(nodo));
    if(n== NULL) error("Memory allocation failed\n");
    strncpy(n->data, data, MSL);
    n->next = NULL;
    if(q->head == NULL){
        q->head = q->tail = n;
    }else{
        q->tail->next = n;
        q->tail = n;
    }
}

void dequeueword(queue* q, char*data){
    if(q->head == NULL) error("Queue is empty\n");
    nodo* tmp = q->head;
    q->head = q->head->next;
    if(q->head == NULL){
        q->tail = NULL;
    }
    strncpy(data , tmp->data, MSL);
    free(tmp);
}

void buildqueue(queue*q, const char* input , int *n){
    FILE*fp = fopen(input, "r");
    if(!fp) error("file cannot be opened\n");
    char buffer[MSL];
    *n = 0; //tiene conto di ogni stringa letta.. se non lo metto legge solo l'ultima
    while(fscanf(fp, "%s", buffer) != EOF){
        buffer[strcspn(buffer,"\n")] = '\0';
        enqueueword(q, buffer);
        (*n)++;
    }
    fclose(fp);
}

char** buildstringarray(queue*q, int n){
    char** nuovo_array= (char**)malloc(n*sizeof(char*));
    if(!nuovo_array) error("Memory allocation failed\n");

    for(int i =0; i<n; i++){
        nuovo_array[i]= (char*)malloc(MSL*sizeof(char));
        if(nuovo_array[i]== NULL) error("Memory allocation failed\n");
        dequeueword(q, nuovo_array[i]);
    }
    return nuovo_array;
}

void sortarray(char**array , int dim){
    for(int i =0; i<dim; i++){
        int j = i-1;
        char* tmp = array[i];
        while(j>=0 && strcmp(tmp, array[j])<0){
            array[j+1] = array[j];
            j--;
        }
        array[j+1]= tmp;
    }
}

void printarray(char**array, int n){
    for(int i =0; i<n; i++){
        printf("%s \n", array[i]);
    }
}

int main(int argc, char**argv){
    params p = readinput(argc, argv);
    int n; // tiene traccia dei valori letti..
    queue q;
    inizializzocoda(&q);
    buildqueue(&q, p.input ,&n); // p.input è già un puntatore a char (stringa), gli altri non essendo necessitano &
    char** W = buildstringarray(&q, n);
    sortarray(W, n);
    printarray(W, n);

    for (int i = 0; i < n; i++) {
        free(W[i]);
    }
    free(W);
    return 0;
}






