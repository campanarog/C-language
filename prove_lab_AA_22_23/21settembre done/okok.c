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
    struct node* next;
}nodo;

void pushWord(nodo** head, char *data){
    nodo* newNode=(nodo*)malloc(sizeof(nodo));
    newNode->next=NULL;
    strncpy(newNode->data, data, MAX_SIZE+1);
    if(*head!=NULL){
        newNode->next=(*head);
    }
    (*head)=newNode;
}

void popWord(nodo** head){
    nodo* temp=*head;
    if(*head==NULL){
        printf("Lista vuota");
        return;
    }
    *head=temp->next;
    free(temp);
}

void buildList(nodo** head, struct parametri pt){
    FILE *fp=fopen(pt.input, "r");
    if(fp==NULL){
        fprintf(stderr, "errore apertura file");
        exit(EXIT_FAILURE);
    }
    char buffer[MAX_SIZE+1];
    while(fgets(buffer, sizeof(buffer), fp)!=NULL){
        pushWord(head, buffer);
    }

    fclose(fp);
}

void printList(nodo** head){
    nodo* temp=*head;
    while(temp!=NULL){
        printf("%s", temp->data);
        temp=temp->next;
        popWord(head);
    }
}

int main(int argc, char *argv[]){
    struct parametri pt=readInput(argc, argv);
    nodo* head=NULL;
    buildList(&head, pt);
    printList(&head);
}