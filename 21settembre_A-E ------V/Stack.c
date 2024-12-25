
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MSL 30

typedef struct{
    char *input;
    int k;
}params;
typedef struct nodo{
    struct nodo*next;
    char data[MSL+1];
}nodo;


params readinput(int argc, char**argv){
    if(argc != 3){
        fprintf(stderr,"Usage %s <input>, <k>", argv[0]);
        exit(EXIT_FAILURE);
    }
    params p;
    p.input = argv[1];
    p.k = atoi(argv[2]);
    if(p.k< 5 || p.k> 15){
        fprintf(stderr,"K must be in range 5-15\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

void pushword(nodo**head, char *data){
    nodo *n = malloc(sizeof(nodo));
    if(n == NULL){
        fprintf(stderr,"Errore durante l'allocazione dinamica\n");
        exit(EXIT_FAILURE);
    }
    n->next = NULL;
    strncpy(n->data, data , MSL+1);
    if(*head != NULL){
        n->next = (*head);
    }
    (*head)= n;
}

void popword(nodo**head){
    if(*head  == NULL){
        fprintf(stderr,"Stack is empty\n");
        exit(EXIT_FAILURE);
    }
    nodo *tmp = *head;
    *head = tmp->next;
    free(tmp);
}

void buildlist(nodo**head, const char* input){
    FILE *fp = fopen(input, "r");
    if(!fp){
        fprintf(stderr,"Errore nella lettura del file\n");
        exit(EXIT_FAILURE);
    }
    char buffer[MSL+1];
    while(fgets(buffer, sizeof(buffer), fp ) != NULL){
        buffer[strcspn(buffer, "\n")] ='\0';
        pushword(head, buffer);
    }
    fclose(fp);
}

void printstack(nodo**head){
    nodo *tmp = *head;
    while(tmp != NULL){
        printf("%s\n", tmp->data);
        tmp = tmp->next;
        popword(head);
    }
}

void sortword(char*data){
    int len = strlen(data);
    for(int i =0; i<len-1; i++){
        for(int j=0; j<len; j++){
            if(data[i] > data[j]){
                char tmp = data[i];
                data[i] = data[j];
                data[j] = tmp;
            }
        }
    }
}

int main(int argc, char**argv){
    params p = readinput(argc, argv);
    nodo *head = NULL;
    // Cicla attraverso la pila e ordina ogni parola
    buildlist(&head, p.input);
    nodo *tmp = head;
    while(tmp != NULL){
        sortword(tmp->data); // Ordina la parola presente e poi passa alla successsiva;
        tmp = tmp->next;
    }
    printstack(&head);
}