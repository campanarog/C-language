#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 30

struct parametri{
    char *input;
    int k;
};

struct parametri readInput(int argc, char *argv[]){
    struct parametri pt;

    if(argc!=3){
        fprintf(stderr, "Usage <%s> <input.txt> <k>", argv[0]);
        exit(EXIT_FAILURE);
    }

    pt.input=argv[1];
    pt.k=atoi(argv[2]);

    if(pt.k<5||pt.k>15){
        fprintf(stderr, "Errore! Il numero k deve essere compreso tra 5 e 15");
        exit(EXIT_FAILURE);
    }

    return pt;
}

void printParameters(struct parametri pt){
    printf("Valori inseriti: %s e %d", pt.input, pt.k);
}

typedef struct node{
    char data[MAX_SIZE+1];
    struct node *next;
    struct node *prev;
}nodo;

typedef nodo* lista;

void insertWord(lista *l, char *data){
    nodo* newNode=(nodo*)malloc(sizeof(nodo));
    if(newNode==NULL){
        fprintf(stderr, "Errore allocazione memoria");
        exit(EXIT_FAILURE);
    }
    strncpy(newNode->data, data, MAX_SIZE);
    if(*l==NULL){
        *l=newNode;
    }else if (strcmp(newNode->data, (*l)->data)<0){
        newNode->prev=NULL;
        newNode->next=*l;
        (*l)->prev=newNode;
        *l=newNode;
    }else{
        nodo *temp=*l;
        while(temp->next!=NULL && strcmp(temp->next->data, newNode->data)<0){
            temp=temp->next;
        }

        if(temp->next!=NULL){
            newNode->next=temp->next;
            temp->next->prev=newNode;
        }

        newNode->prev=temp;
        temp->next=newNode;
    }
}

void buildList(lista *l, struct parametri pt){
    FILE *fp=fopen(pt.input, "r");
    if(fp==NULL){
        fprintf(stderr, "Errore apertura file");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_SIZE+1];
    while(fgets(buffer, sizeof(buffer), fp)!=NULL){
        insertWord(l, buffer);
    }

    fclose(fp);
}

void printList(lista l){
    while(l!=NULL){
        printf("%s", l->data);
        l=l->next;
    }
}

int elab(lista l, int k){
    int count=0;
    while(l!=NULL){
        if(strlen(l->data)>=k){
            count++;
        }
        l=l->next;
    }
    return count;
}
int main(int argc, char *argv[]){
    struct parametri pt=readInput(argc, argv);
    printParameters(pt);
    lista l=NULL;
    buildList(&l, pt);
    printList(l);
}