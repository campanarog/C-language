#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 30

struct parametri{
    char *input;
    int k;
};

struct parametri readInput(int argc, char **argv){
    struct parametri pt;

    if(argc!=3){
        fprintf(stderr, "Usage %s <input.txt> <k>", argv[0]);
        exit(EXIT_FAILURE);
    }

    pt.input=argv[1];
    pt.k=atoi(argv[2]);

    if(pt.k<5 || pt.k>15){
        fprintf(stderr, "Errore! Il numero k deve essere compreso tra 5 e 15");
        exit(EXIT_FAILURE);
    }

    return pt;
}

typedef struct node{
    char data[MAX_SIZE+1];
    struct node *next;
    struct node *tail;
}nodo;

void enqueue(nodo** head, nodo **tail, char *data){
    nodo* newNode=malloc(sizeof(nodo));
    newNode->next=NULL;
    strncpy(newNode->data, data, MAX_SIZE+1);
    if(*head==NULL){
        *head=newNode;
        *tail=newNode;
    }else{
        (*tail)->next=newNode;
        *tail=newNode;
    }
}

void dequeue(nodo** head){
    if(*head==NULL){
        printf("Lista vuota");
        return;
    }
    nodo* temp=*head;
    *head=temp->next;
    free(temp);
}

void buildList(nodo** head, nodo** tail, struct parametri pt){
    FILE *fp=fopen(pt.input, "r");
    if(fp==NULL){
        fprintf(stderr, "Errore apertura file");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_SIZE+1];
    while(fgets(buffer, sizeof(buffer), fp)!=NULL){
        enqueue(head, tail, buffer);
    }
    fclose(fp);
}

void printList(nodo** head){
    nodo* temp=*head;
    while(temp!=NULL){
        printf("%s", temp->data);
        temp=temp->next;
        dequeue(head);
    }
}

int main(int argc, char *argv[]){
    struct parametri pt=readInput(argc, argv);
    nodo* head=NULL;
    nodo* tail=NULL;
    buildList(&head, &tail, pt);
    printList(&head);
}