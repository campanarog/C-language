
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MSL 30

typedef struct{
    char *input; 
    float f;
}params;


params readinput(int argc, char**argv){
    if(argc != 3){
        fprintf(stderr,"Usage ::: %s ::: <input>, <float>", argv[0]);
        exit(EXIT_FAILURE);
    }
    params p;
    p.input = argv[1];
    p.f = atof(argv[2]);
    if(p.f < 1.0 || p.f > 20.0){
        fprintf(stderr,"Float must be in range 1.0-20.0\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

typedef struct nodo{
    struct nodo* next;
    struct nodo* prev;
    float valore;
    char data[MSL+1];
}nodo;

typedef nodo* lista;
void insertrecord(lista *l, char *data, float valore){
    nodo* n = malloc(sizeof(nodo));
    if(n == NULL){
        fprintf(stderr,"Errore durante l'allocazione dinamica\n");
        exit(EXIT_FAILURE);
    }
    strncpy(n->data, data, MSL+1);
    n->valore = valore;
    if(*l == NULL){
        *l = n;
    }else if(strcmp(n->data, (*l)->data)<0){
        n->prev = NULL;
        n->next = *l;
        (*l)->prev = n;
        *l = n;
    }else{
        nodo *tmp = *l;
        while(tmp->next != NULL && strcmp(tmp->next->data, n->data)<0){
            tmp = tmp->next;
        }
        if(tmp->next != NULL){
            n->next = tmp->next;
            tmp->next->prev = n;
        }
        n->prev = tmp;
        tmp->next = n;
    }
}

void buildlist(lista*l , const char *input){
    FILE *fp = fopen(input, "r");
    if(!fp){
        fprintf(stderr,"Error durante l'apertura del file\n");
        exit(EXIT_FAILURE);
    }
    char buffer[MSL+1];
    float valore;
    while(fscanf(fp, "%f %s",&valore, buffer) != EOF){
        insertrecord(l , buffer, valore);
    }
    fclose(fp);
}

void printlist(lista l){
    while(l != NULL){
        printf("%s %.2f\n", l->data, l->valore);
        l = l->next;
    }
}

void elab(lista l , float f){
    int count = 0;
    while (l != NULL){
        if(l->valore >= f){
            count++;
        }
        l = l->next;
    }
    printf("Numero record aventi lunghezza pari o maggiore di %.1f : %d\n", f, count);
}


void destroylist(lista *l){
    while(*l != NULL){
        nodo * tmp = *l;
        *l = (*l)->next;
        free(tmp);
    }
}

int main(int argc, char **argv){
    params p=readinput(argc, argv);
    lista l=NULL;
    buildlist(&l, p.input);
    printlist(l);
    elab(l,p.f);
    return 0;
}