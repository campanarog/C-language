#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define SIZE 30
typedef struct{
    char *file_name;
    int k;
}parameters;

parameters readInput(int argc, char **argv){

    if(argc!=3){
        fprintf(stderr,"Usa i parametri:%s <file_name> <k>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    parameters p;
    p.file_name=argv[1];
    p.k=atoi(argv[2]);

    if(p.k<7 || p.k>13){
        fprintf(stderr,"k deve essere compreso nell'intervallo [7,13]\n");
        exit(EXIT_FAILURE);
    }

    return p;
}


typedef struct node{
    char word[SIZE];
    struct node *next;
    struct node *prev;
}node;

typedef node *list;

void insertWord(list *l, char *word){
    
    node *n=malloc(sizeof(node));
    if(n==NULL){
        fprintf(stderr,"Errore nell'allocazione di memoria per il nuovo nodo\n");
        exit(EXIT_FAILURE);
    }
    strncpy(n->word,word,SIZE);
    /*strcpy utilzza 2 argomenti*/

    if(*l==NULL){
        n->next=n->prev=NULL;
        *l=n;
    }
    else if(strcmp(n->word,(*l)->word)<0){
        n->prev=NULL;
        n->next=*l;
        (*l)->prev=n;
        *l=n;
    }
    else{
        node *ptr=*l;
        while(ptr->next!=NULL && strcmp(ptr->next->word,n->word)<0){
            ptr=ptr->next;
        }

        if(ptr->next!=NULL){
            n->next=ptr->next;
            ptr->next->prev=n;
        }
        n->prev=ptr;
        ptr->next=n;
    }
}

void buildList(list *l, char *file_name){

    FILE *f=fopen(file_name,"r");
    if(f==NULL){
        fprintf(stderr,"Errore nell'apertura del file in lettura\n");
        exit(EXIT_FAILURE);
    }
    char word[SIZE];
    while(fscanf(f,"%s\n",word)!=EOF){
        insertWord(l,word);
    }
    fclose(f);

}

void printList(list l){
    while(l!=NULL){
        printf("%s\n",l->word);
        l=l->next;
    }
}

int elab(list l, int k){
    
    int n=0;

    while(l!=NULL){
        if(strlen(l->word)>=k){
           n++; 
        }
        l=l->next;
    }

    return n;
}

void destroyList(list *l){
    
    while(*l!=NULL){
        node *ptr=*l;
        *l=(*l)->next;
        free(ptr);
    }

}

int main(int argc, char **argv){

    parameters p=readInput(argc,argv);
    list head=NULL;
    buildList(&head,p.file_name);
    int n=elab(head,p.k);
    printf("\nNumero di stringhe aventi lunghezza pari o maggiore di 10: %d\n", n);
    printf("\nOutput\n");
    printList(head);
    destroyList(&head);
}