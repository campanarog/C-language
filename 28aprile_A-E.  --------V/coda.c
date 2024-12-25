#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MSL 30

typedef struct{
    char*input;
    char a;
    char b;
}params;

params readinput(int argc, char**argv){
    if(argc != 4){
        fprintf(stderr,"Usage ::: %s ::: <input>, <a>, <b>", argv[0]);
        exit(EXIT_FAILURE);
    }
    params p;
    p.input = argv[1];
    p.a = argv[2][0];
    p.b = argv[3][0];
    if(p.a >= p.b ){
        fprintf(stderr, "B cant be greater than A\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

typedef struct nodo{
    struct nodo* next;
    char data[MSL+1];
}nodo;

typedef struct {
    nodo* head;
    nodo* tail;
}queue;

void inizializzocoda(queue*q){
    q->head = q->tail = NULL;
}

void enqueue(queue*q, const char* data){
    nodo* n = malloc(sizeof(nodo));
    if(n == NULL)
    {
        fprintf(stderr,"Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strncpy(n->data, data, MSL+1);
    n->next = NULL;
    if(q->head == NULL && q->tail == NULL){
        q->head = q->tail = n;
    }else{
        q->tail->next = n;
        q->tail = n;
    }
}

char* dequeue(queue*q){
    if(q->head == NULL){
        return NULL;
    }
    nodo *tmp = q->head;
    q->head = q->head->next;
    if(q->head == NULL){
        q->tail = NULL;
    }
    char *data=malloc(sizeof(char) *MSL+1);
    if(data == NULL){
        fprintf(stderr,"Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strncpy(data, tmp->data, MSL+1);
    free(tmp);
    return data;
}

void buildqueue(queue*q, const char *input){
    FILE *fp = fopen(input, "r");
    if(!fp){
        fprintf(stderr,"File cannot be opened\n");
        exit(EXIT_FAILURE);
    }
    char buffer[MSL+1];
    while(fscanf(fp, "%s\n", buffer) != EOF){
        buffer[strcspn(buffer,"\n")] = '\0';
        enqueue(q, buffer);
    }
    fclose(fp);
}

void printqueue(queue*q){
    while(q->head != NULL){
        char *word = dequeue(q);
        if(word != NULL){
            printf("%s\n", word);
            free(word);
        }
    }
}

void elab(queue*q, char a , char b){
    nodo*tmp = q->head;
    while(tmp != NULL){
        for(int i =0; i<MSL; i++){
            if(tmp->data[i] >= 'a' && tmp->data[i] <= 'b'){
                tmp->data [i] = '0';
            }
        }
        tmp = tmp->next;
    }
}


int main(int argc, char**argv){
    params p = readinput( argc, argv);
    queue q;
    inizializzocoda(&q);
    buildqueue(&q, p.input);
    elab(&q, p.a, p.b);
    printqueue(&q);
    return 0;
}