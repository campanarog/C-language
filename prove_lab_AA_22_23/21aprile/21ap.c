
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MSL 30

typedef struct{
    char* input;
    int k;
}params;

typedef struct nodo{
   char* data;
   struct nodo*next;
}nodo;


params readinput(int argc, char**argv){
    if(argc != 3){
        fprintf(stderr,"Usage %s <input>, <k>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    params p;
    p.input = argv[1];
    p.k= atoi(argv[2]);
    if(p.k<5 || p.k> 15){
        fprintf(stderr,"k must be in range 5-15\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

void pushword(nodo** testa, char* data){
    nodo* n= malloc(sizeof(nodo));
    if(n == NULL){
        fprintf(stderr,"memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    n->data = strdup(data);
    if(n->data == NULL){
        fprintf(stderr,"memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    n->next= *testa;
    *testa = n;
}

char* popword(nodo**testa){
    if(*testa == NULL){
        return NULL;
    }
    nodo* ptr = *testa;
    char* data = strdup(ptr->data);
    if(data == NULL){
        fprintf(stderr,"errore durante l'allocazione di memoria\n");
        exit(1);
    }
    *testa = (*testa)->next;
    free(ptr->data);
    free(ptr);
    return data;
}

int numstringhe( const char*input){
    FILE *fp = fopen(input, "r");
    if(!fp){
        fprintf(stderr,"memory\n");
        exit(EXIT_FAILURE);
    }
    int n =0;
    char buffer[MSL];
    while(fscanf(fp, "%s", buffer) != EOF){
        n++;
    }
    printf("le stringhe di lette sono : %d",n );
    fclose(fp);
    return n;
}

void buildstack(const char* input,nodo**testa){
    FILE *fp = fopen(input,"r");
    if(!fp){
        fprintf(stderr,"file cannot be opened\n");
        exit(EXIT_FAILURE);
    }
    char buffer[MSL+1];
    int stringhez =numstringhe(input);
        for(int i =0; i<stringhez; i++){
            char buffer[MSL];
            if(fscanf(fp, "%s", buffer) == EOF){
                fprintf(stderr,"Errore durante il parsing del file\n");
                exit(-1);
            }
            pushword(testa, buffer);
        }
        fclose(fp);
}

void printstack(nodo**testa){
    while(*testa != NULL){
        char *ptr = popword(testa);
        printf("%s\n", ptr);
        free(ptr);
    }
}


int main(int argc, char**argv){
    params p = readinput(argc, argv);
    nodo* testa = NULL;
    buildstack(p.input, &testa);
    printstack(&testa);
    numstringhe(p.input);
    return 0;
}