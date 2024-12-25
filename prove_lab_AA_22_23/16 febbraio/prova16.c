
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define SIZE 255
#define N 3
typedef struct{
    char *input;
    char *output;
}parameters;

parameters readInput(int argc, char **argv){

    if(argc!=3){
        fprintf(stderr, "Usage:%s <input> <output>\n", argv[0]);
        exit(1);
    }

    parameters p;
    p.input=argv[1];
    p.output=argv[2];

    if(strcmp(p.input+strlen(p.input)-4,".txt")!=0){
        fprintf(stderr, "Il file %s deve avere estensione \".txt\"\n",p.input);
        exit(1);
    }

    
    if(strcmp(p.output+strlen(p.output)-4,".txt")!=0){
        fprintf(stderr, "Il file %s deve avere estensione \".txt\"\n",p.output);
        exit(1);
    }

    return p;
}

typedef struct{
    char nome[SIZE];
    char cognome[SIZE];
    int età;
    int peso;
    int altezza;
    char sesso;
}persona;

typedef struct node{
    persona p;
    struct node *next;
}node;

typedef node *list;

void insertNodeOrdered(node **l, persona p){

    node *n=malloc(sizeof(node));
    n->p=p;
    n->next=NULL;

    if(*l==NULL){
        *l=n;
    }
    else if(n->p.età<(*l)->p.età){
        n->next=*l;
        *l=n;
    }
    else{
        node *ptr=*l;

        while(ptr->next!=NULL && ptr->next->p.età<n->p.età){
            ptr=ptr->next;
        }

        if(ptr->next!=NULL){
            n->next=ptr->next;
        }
        ptr->next=n;
    }
}

list readFile(char *input){

    FILE *f=fopen(input,"r");
    if(f==NULL){
        fprintf(stderr,"Errore nell'apertura del file in lettura\n");
        exit(1);
    }

    list head=NULL;
    persona p;

    while(fscanf(f,"%s %s %d %d %d %c\n", p.nome, p.cognome, &p.età, &p.peso, &p.altezza, &p.sesso)!=EOF){
        insertNodeOrdered(&head,p);
    }

    fclose(f);
    return head;
}

void printPerson(persona p){

    printf("%10s %10s %5d %5d %5d %5c\n", p.nome, p.cognome, p.età, p.peso, p.altezza, p.sesso);

}

void printList(list l){

    while(l!=NULL){
        printPerson(l->p);
        l=l->next;
    }
}

float imc(persona p){
    float imc=p.peso/pow((p.altezza/100.0),2);
    return imc;
}

void insertNode(list *l, persona p){

    node *n=malloc(sizeof(node));
    n->p=p;
    n->next=NULL;

    if(*l==NULL){
        *l=n;
    }
    else{
        n->next=*l;
        *l=n;
    }
}

void deleteNode(node *to_rmv, list *l){
    //1 CASO:cancello in testa
    if(*l==to_rmv){
        node *ptr=*l;
        *l=(*l)->next;
        free(ptr);
        return ; 
    }
    else{
    //2 CASO: trovo il nodo e lo cancello
    node *ptr=*l;
    node *prev=NULL;

    while(ptr!=NULL && ptr!=to_rmv){
        prev=ptr;
        ptr=ptr->next;
    }

    prev->next=ptr->next;
    free(ptr);
    return ;
    }
}

node * getMax(list A){

    float imc_MAX=0;
    float imc_tmp=0;
    node *imc_max_node=NULL;

    while(A!=NULL){
        imc_tmp=imc(A->p);

        if(imc_MAX<imc_tmp){
            imc_MAX=imc_tmp;
            imc_max_node=A;
        }
        A=A->next;
    }

    return imc_max_node;
}

void writeFile(list B, char *output){

    FILE *f=fopen(output,"w");
    if(f==NULL){
        fprintf(stderr,"Errore nell'apertura del file in scrittura\n");
        exit(1);
    }

    while(B!=NULL){
        fprintf(f,"%s %s %d %d %d %c\n", B->p.nome, B->p.cognome, B->p.età, B->p.peso, B->p.altezza, B->p.sesso);
        B=B->next;
    }

    fclose(f);

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
    printf("=======PUNTO A=======\n");
    printf("input = %s, output = %s\n", p.input, p.output);

    printf("\n=======PUNTO B=======\n");
    list A=readFile(p.input);
    printList(A);

    printf("\n=======PUNTO C=======\n");
    node *imc_max_node=getMax(A);
    printPerson(imc_max_node->p);

    printf("\n=======PUNTO D=======\n");
    list B=NULL;
    for(int i=0; i<N; i++){
        imc_max_node=getMax(A);
        insertNode(&B,imc_max_node->p);
        deleteNode(imc_max_node,&A);    
    }
    printf("A: \n");
    printList(A);
    printf("B: \n");
    printList(B);

    writeFile(B, p.output);

    destroyList(&A);
    destroyList(&B);
    
}