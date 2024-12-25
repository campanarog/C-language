
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MSL 30

typedef struct{
    char* input;
    float f;
}params;

typedef struct nodo{
    char data[MSL];
    float V;
    struct nodo *next;
    struct nodo* prev;
}nodo;

typedef nodo* lista;

params readinput(int argc, char**argv){
    if(argc != 3){
        fprintf(stderr,"Usage %s <input>, <f>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    params p;
    p.input = argv[1];
    p.f = atof(argv[2]);
    if(p.f< 1.0 || p.f> 20.0){
        fprintf(stderr,"F must be in range 1-20.0\n ");
        exit(EXIT_FAILURE);
    }
    return p;
}

void insertrecord(lista*l , char*data, float V){
    nodo *n = malloc(sizeof(nodo));
    if(n == NULL){
        fprintf(stderr,"memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strncpy(n->data , data , MSL);
    n->V = V;
    if(*l == NULL){
        n->next = NULL;
        n->prev = NULL;
        *l = n;
    }else if(strcmp(n->data, (*l)->data)<0){
        n->next = *l;
        n->prev = NULL;
        *l = n;
    }else{
        nodo* tmp = *l;
        nodo *prev = NULL;
        while(tmp->next != NULL && strcmp(tmp->next->data, n->data)<0){
            tmp = tmp->next;
        }
        if(tmp != NULL){
            n->next = tmp->next;
            tmp->next->prev= n;
        }
        n->prev = tmp;
        tmp->next = n;
    }
}

void buildlist(lista*l ,const char* input){
    FILE *fp = fopen(input, "r");
    if(!fp){
        fprintf(stderr,"File cannot be opened\n");
        exit(EXIT_FAILURE);
    }
    char buffer[MSL+1];
    float V;
    while(fscanf(fp, "%f \n %s \n", &V, buffer) != EOF){
        insertrecord(l, buffer, V);
    }

}

void printlist(lista l){
    while(l!= NULL){
        printf("%s %.1f\n", l->data, l->V);
        l= l->next;
    }
}

void elab(lista l, float f){
    int conteggio = 0;
    while(l != NULL){
        if((l->V)>=f){
            conteggio++;
        }
        l = l->next;
    }
    printf("Numero di record aventi lunghezza pari o maggiore di %.1f: %d\n", f,conteggio);
}

int main(int argc, char**argv){
    params p = readinput(argc, argv);
    lista l = NULL;
    buildlist(&l , p.input);
    printlist(l);
    elab(l, p.f);
    return 0;
}




