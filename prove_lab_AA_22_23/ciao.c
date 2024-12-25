
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

/* struttura per passare i parametri dell'input*/
typedef struct {
 int N;
 char a;
 char b;

}InputParams;

struct node  {
    char data;
    struct node* next;
};

typedef struct node node;
 /* funzione per generare numeri pseudocasuali*/
unsigned int get_random(){
     static unsigned int m_w = 123456;
    static unsigned int m_z = 789123;
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}

/* funzion per la generazione di un carattere vocale pseudocasuale
const char wovels e metto le vocali.. return wovels+ get random per 5*/
char genwovel(){
    const char wovels []= { 'a', 'e', 'i', 'o', 'u'};
    return wovels[get_random()%5];
}

char genConsonant(){
    const char consonants[]={'b','c','d','f','g','h','i','l','m','n','p','q','r','s','t','v','z'};
    return consonants[get_random()%21];
}

/* funzione per inserire un elemento sullo stack*/
void push(node** top, char data){
    node* newnode = (node*)malloc(sizeof(node));
    newnode-> data = data;
    newnode->next = *top;
    *top = newnode;
}

/* funzione per stampare lo stack*/

void printstack(node* top){
    printf("**TOP ------>");
    while(top != NULL){
        printf("%c", top->data);
        top= top->next;
    }
    printf("<-----BOTTOM**\n");
}

/* funzione per leggere input*/
InputParams readInput(char* argv[]){
     InputParams params;
    /* controllo degli argomenti */
    if(argv[1]== NULL || argv[2]== NULL || argv[3]== NULL){
        fprintf(stderr, "Error: type in all required params\n");
        exit(EXIT_FAILURE);
    }
    /* conversione degli argomenti (insertiti dall'utente)*/
    /* prima nella struct avevo messo int n char a e char b, quindi devo convertire questo*/
    params.N = atoi(argv[1]);
    /* punta ad una stringa(che rimane tale.. )
    invece argv [2][0] punta alla stessa stringa ma potrebbe a piu  caselle, quindi è necessario terminarlo \0 con [0];  */
    params.a = argv[2][0];
    params.b = argv[3][0];
    /*Quando si tratta di leggere un singolo carattere da una stringa, è comune utilizzare l'indice [0] per ottenere il primo elemento.
     Questo perché le stringhe in C sono terminate con il carattere nullo '\0', e indicizzare [0] restituisce il primo carattere prima di questa terminazione.*/

    /* adesso il controllo dei parametri, ovvero lo voglio flitrare*/

    if(params.N < 5 || params.N > 10 || (params.a < 'a' || params.b >'z') || (params.b < 'a' || params.b > 'z')){
        fprintf(stderr,"Error : non valid params\n");
        exit(EXIT_FAILURE);
    }
    return params;
}

/* funzione per implementare la procedura descritta nel punto B
essendo che devo fare dei push a scaglioni debbo fare :*/
void fillstack(node** top,  InputParams params){
    for(int i =0 ; i<params.N; ++i){
        char x = '1' + get_random()%9;
        char c;

        for(int j = 0; j< x-'0'; ++j){

            if(x >= '1' && x <= '4'){
                c= genwovel();
            }else {
                c = genConsonant();
            }
            if(c == params.a){
                c = '*';

            }else if( c== params.b){
                c = '?';
            }
            push(top , c);

        }
        push(top,c);
    }

}

int main(int argc, char* argv[]){
    /* leggi input*/
    InputParams params = readInput(argv);
    /* inizializza lo stack come lista concatenata*/
    node* stackTop = NULL; 
    /* esegui la procedura descritta nel punto B*/
    fillstack(&stackTop, params);
    /* print lo stack*/
    printstack(stackTop);

    /* libera la memoria occupata dalla lista concatenata (stack)*/
    while(stackTop != NULL){
        node* temp = stackTop;
        stackTop = stackTop->next;
        free(temp);
    }
    return 0;
}