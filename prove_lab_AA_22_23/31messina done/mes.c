
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MSL 30

typedef struct{
    char* input;
    int k;
}params;

typedef struct nodo{
    char data[MSL];
    struct nodo* next;
    struct nodo* prev;
}nodo;

typedef nodo* lista;

params readinput(int argc, char**argv){
    if(argc != 3){
        fprintf(stderr,"Usage %s <input>, <k>", argv[0]);
        exit(EXIT_FAILURE);
    }
    params p;
    p.input = argv[1];
    p.k = atoi(argv[2]);

    if(p.k< 7 || p.k> 13){
        fprintf(stderr,"k must be in range 7-13\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

void insertword(lista*l , char* data){
    nodo *n= malloc(sizeof(nodo));
    if(n == NULL){
        fprintf(stderr,"memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strncpy(n->data, data,MSL);
    if(*l == NULL){
        n->next = n->prev = NULL;
        *l = n;
    }
    else if(strcmp(n->data, (*l)->data) <0){
        n->prev = NULL;
        n->next = *l;
        (*l)->prev = n;
        *l= n;
    }
    else{
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

void buildlist(const char* input, lista *l){
    FILE *fp = fopen(input, "r");
    if(!fp){
        fprintf(stderr,"file cannot be opened\n");
        exit(EXIT_FAILURE);
    }
    char buffer [MSL+1];
    while(fgets(buffer, sizeof(buffer), fp) != NULL){
       buffer[strcspn(buffer, "\n")] = '\0';
       insertword(l, buffer);
    }
    fclose(fp);
}

void printlist(lista l){
    while(l != NULL){
        printf("%s\n", l->data);
        l = l->next;
    }
}

int elab(lista l, int k){
    int pino = 0;
    while( l!= NULL){
        if(strlen(l->data) >= k){
            pino++;
        }
        l = l->next;
    }
    return pino;
} 

void destroylist(lista *l){
    while(*l != NULL){
        nodo *tmp = *l;
        *l = (*l)->next;
        free(tmp);
    }
}

int main(int argc, char**argv){
    params p = readinput(argc, argv);
    lista l = NULL;
    buildlist(p.input, &l);
    int n = elab(l, p.k);
    printf("\nnumero di stringhe aventi lunghezza pari o maggiore di 10 : %d", n);
    printf("\n output \n");
    printlist(l);
    destroylist(&l);
}