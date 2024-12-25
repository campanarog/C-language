
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MSL 30

typedef struct{
    char* input;
    int k;
}params;

params readinput(int argc, char**argv ){
    if(argc != 3){
        fprintf(stderr,"Usage  ::: %s ::: <input>, <k>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    params p;
    p.input = argv[1];
    p.k = atoi(argv[2]);
    if(p.k < 7 || p.k> 13){
        fprintf(stderr,"K must be in range 7-13\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

typedef struct nodo{
    char data[MSL];
    struct nodo* next;
    struct nodo*prev;
}nodo;

typedef nodo* lista;

void insertword(lista*l, char*data){
    nodo*n = malloc(sizeof(nodo));
    if(n == NULL){
       fprintf(stderr,"Memory allocation failed\n");
       exit(EXIT_FAILURE);
    }
    strncpy(n->data, data, MSL);
    if(*l == NULL){
        n->next = n->prev = NULL;
        *l = n;
    }else if(strcmp(n->data, (*l)->data)<0){
        n->prev = NULL;
        n->next = *l;
        (*l)->prev = n;
        *l = n;
    }else{
        nodo* tmp= *l;
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

void buildlist(lista *l, const char* input){
    FILE *fp = fopen(input, "r");
    if(!fp){
        fprintf(stderr,"File cannot be opened\n");
        exit(EXIT_FAILURE);
    }
    char buffer[MSL];
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
    int count =0;
    while(l != NULL){
        if(strlen(l->data) >= k){
            count++;
        }
        l = l->next;
    }
    return count;
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
    buildlist(&l, p.input);
    int n = elab(l, p.k);
    printf("\nNumero di stringhe aventi lunghezza pari o maggiore di 10: %d\n", n);
    printf("\nOutput\n");
    printlist(l);
    destroylist(&l);
}