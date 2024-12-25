
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MSL 30

typedef struct {
    char *input;
    int x;
    int y;
} params;

params readinput(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage  ::: %s ::: <input>, <x>, <y> ", argv[0]);
        exit(EXIT_FAILURE);
    }
    params p;
    p.input = argv[1];
    p.x = atoi(argv[2]);
    p.y = atoi(argv[3]);
    if (p.x < 10 || p.x > 60) {
        fprintf(stderr, "x must be in range 10-60\n");
        exit(EXIT_FAILURE);
    }
    if (p.y < 10 || p.y > 60) {
        fprintf(stderr, "y must be in range 10-60\n");
        exit(EXIT_FAILURE);
    }
    if (p.x > p.y) {
        fprintf(stderr, "X can't be greater than y\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

typedef struct record{
    char nome[MSL];
    char cognome[MSL];
    int età;
}record;

typedef struct nodo{
    struct nodo* next;
    record r;
}nodo;

typedef nodo* lista;

void insertrecord(lista *l, record r){
    nodo * n = malloc(sizeof(nodo));
    n->r = r; // se non inizializzata , nodo->r non copierà mai il valore e di conseguenza avremo 0 in output;
    if(n== NULL){
        fprintf(stderr,"Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    if(*l == NULL || strcmp(n->r.cognome , (*l)->r.cognome)<0){
        n->next = *l;
        *l = n;
    }else{
        nodo *tmp = *l;
        while(tmp->next != NULL && strcmp(tmp->next->r.cognome, n->r.cognome)<0){
            tmp = tmp->next;
        }
        n->next = tmp->next;
        tmp->next = n;
    }
}

void buildlist(lista *l , const char* input){
    FILE *fp = fopen(input, "r");
    if(!fp){
        fprintf(stderr,"errore durante l'apertura del file\n");
        exit(EXIT_FAILURE);
    }
    record r;
    while(fscanf(fp, "%s %s %d\n", r.nome, r.cognome, &r.età)!= EOF){
        insertrecord(l,r);
    }
    fclose(fp);
}

void printrecord(record r){
    printf("%s %s %d\n", r.nome , r.cognome, r.età);
}

void printlist(lista l){
    while(l != NULL){
        printrecord(l->r);
        l = l->next;
    }
}

void removebyage(lista *l , int x , int y){
    nodo *tmp = *l;
    nodo* prev = NULL;
    while(tmp != NULL){
        if(tmp->r.età >= x && tmp->r.età <= y){
            if(prev == NULL){
                *l = tmp->next;
            }else {
                prev->next = tmp->next;
            }
            nodo* deleting = tmp;
            tmp = tmp->next;
            free(deleting);
        }else{
            prev = tmp;
            tmp = tmp->next;
        }
    }
}

int main(int argc, char **argv) {
    params p = readinput(argc, argv);
    lista l = NULL;
    
    buildlist(&l, p.input);
    printf("**** Lista originale ****\n");
    printlist(l);
    
    removebyage(&l, p.x, p.y);
    printf("\n**** Lista dopo la rimozione dei record con età tra %d e %d ****\n", p.x, p.y);
    printlist(l);
    
    return 0;
}
