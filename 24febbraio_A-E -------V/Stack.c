
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct{
    int n;
    char a;
    char b;
}params;

unsigned int get_random() {
static unsigned int m_w = 123456;
static unsigned int m_z = 789123;
m_z = 36969 * (m_z & 65535) + (m_z >> 16);
m_w = 18000 * (m_w & 65535) + (m_w >> 16);
return (m_z << 16) + m_w;
}

params readinput(int argc, char**argv){
    if(argc != 4){
        fprintf(stderr,"ERROR ::: %s ::: <n>, <a>, <b>", argv[0]);
        exit(EXIT_FAILURE);
    }
    params p;
    p.n = atoi(argv[1]);
    p.a= argv[2][0];
    p.b = argv[3][0];
    if(p.n < 5 || p.n > 10){
        fprintf(stderr,"Error N must be in range 5-10\n");
        exit(EXIT_FAILURE);
    }
    if(p.a < 'a' || p.a > 'z'){
        fprintf(stderr,"Error A must be in range a-z\n");
        exit(EXIT_FAILURE);
    }
    if(p.b < 'a' || p.b > 'z'){
        fprintf(stderr,"Error B must be in range a-z \n");
        exit(EXIT_FAILURE);
    }
    return p;
}

typedef struct nodo{
    struct nodo*next;
    char data;
}nodo;

char getVocali(){
    char vocali[] = "aeiou";
    return vocali[rand()%5];
}

char getConsonanti(){
    char consonanti[] = "qwrtypsdfghjklzxcvbnm";
    return consonanti[rand()%21];
}

void push(nodo **head, char data){
    nodo* newNode=(nodo*)malloc(sizeof(nodo));
    if(newNode==NULL){
        fprintf(stderr, "Errore allocazione in memoria");
        exit(EXIT_FAILURE);
    }
    newNode->next=NULL;
    newNode->data=data;
    
    newNode->next=*head;
    *head=newNode;
}

void fillstack(nodo**head, params p){
    for(int i =0; i<p.n; i++){
        char x = '1'+ get_random()%9;
        int pushing = x -'0';
        for(int j =0; j<pushing ; j++){
            char c;
            if(x >= '1' && x <= '4'){
                c = getVocali();
            }else{
                c = getConsonanti();
            }
            if(c == p.a){
                c = '*';
            }else if(c == p.b){
                c = '?';
            }
            push(head, c); //Guardiamo push letteralmente come, "Ho tutto ? posso darlo in output e metterlo sullo stack?"
        }
        push(head, x);
    }
}
// in questo caso , c per il carattere... e x per generare c da 1-9.

void printstack(nodo *head){
    printf(" **** TOP ------>");
    while(head != NULL){
        printf("%c ", head->data);
        head = head->next;
    }
    printf("<------ BOTTOM ***\n");
}

int main(int argc, char**argv){
    params p = readinput(argc, argv);
    nodo *head = NULL;
    fillstack(&head, p);
    printstack(head);
    return 0;
}