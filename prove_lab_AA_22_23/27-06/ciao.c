
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char* filename;
    int n;
} params;

unsigned int get_random() {
    static unsigned int m_w = 123456;
    static unsigned int m_z = 789123;
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}


params decodeParameters(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Utilizzo: %s <filename> <n>\n", argv[0]);
        exit(-1);
    }
    params p;
    p.filename = argv[1];
    p.n = atoi(argv[2]);
    if (p.n < 3 || p.n > 5) {
        fprintf(stderr, "Errore: n deve essere compreso tra 3 e 5\n");
        exit(-1);
    }

    char* estensione = strrchr(p.filename, '.');
    if (estensione != NULL && strcmp(estensione, ".bin") == 0) {
        printf("Il file ha estensione .bin\n");
    } else {
        printf("Il file non ha estensione .bin\n");
    }
    return p;
}

typedef struct {
    int numeroconto;
    char nome[255];
    char cognome[255];
    float saldo;
} id;

/* char nome , tipo di tipo int, char  nome[255], char di tipo int* */

typedef struct nodo {
    struct nodo* next;
    id data;
} nodo;

/* lista? quindi devo creare nodo*/
nodo* creonodo(id data) {
    nodo* nodino = malloc(sizeof(nodo));
    if (nodino == NULL) {
        fprintf(stderr, "Memoria non allocata correttamente\n");
        exit(-1);
    }
    nodino->data = data;
    nodino->next = NULL;
    return nodino;
}
/* nodo? quindi devo inserirlo da qualche parte*/

void inseriscinodino(nodo** testa, id record) {
    nodo* nodino = creonodo(record);
    if (*testa == NULL) {
        *testa = nodino;
    } else {
        nodo* tmp = *testa;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = nodino;
    }
}

id readrecord(){
    id r;
    scanf("%d, %s, %s, %f", &r.numeroconto, r.nome , r.cognome, &r.saldo);
    return r;
}
/*Quindi, in breve, usi l'operatore "&" per i tipi di dati primitivi come int e float,
ma non per le stringhe poiché in C le stringhe sono rappresentate come array di caratteri 
 e il nome dell'array già si comporta come un puntatore all'indirizzo di memoria del primo elemento dell'array.*/
/* & chiamata anche ampersand*/

void loadrecord(nodo** testa){
    bool end = false;
    while(!end){
        id r = readrecord();
        if(feof(stdin)){
            end = true;
            break;
        }
        inseriscinodino(testa , r);
    }
     /* con questo leggo i record da riga da comando e li conserva nella lista L*/
}

void printrecord(id r){
    printf("%d, %s, %s, %.2f\n", r.numeroconto, r.nome, r.cognome, r.saldo);
}
int listlen(nodo* testa) {
    nodo* current = testa;
    int i = 0;

    while (current != NULL) { 
        i++;
        current = current->next;
    }

    return i;
}

void selectionsort(id *X , int n){
    double m = 0;
    for(int i=0; i<n; i++){
        m += X[i].saldo;
    }
    m/=n;

    for(int pass =0; pass<n-1; pass++){
        int idx_min = pass;
        for(int i= pass+1; i<n; i++){
            if((X[i].saldo-m)*(X[i].saldo-m)< (X[idx_min].saldo-m)*(X[idx_min].saldo-m)){
                idx_min= i;
            }
            id tmp = X[pass];
            X[pass]= X[idx_min];
            X[idx_min]= tmp;
        }
    }
}

void saveToFile(id X[], char filename[], int n) {
    FILE *cfPtr = NULL;

    if ((cfPtr = fopen(filename, "wb")) == NULL) {
        fprintf(stderr, "Impossibile aprire il file.");
        exit(-1);
    }

    for(int i = 0; i<n; i++){
        fwrite(&X[i], sizeof(id), 1, cfPtr);
    }
    fclose(cfPtr);
}/* "WB" è mod. scrittura binaria , write binary */
/* fwrite si usa quando la modalità è in WB*/

void printArray(id *X, int len) {
    for(int i=0; i<len; i++) {
        printrecord(X[i]);
    }
}


id* getarray(nodo* testa){
    id *X = calloc(listlen(testa), sizeof(id));
    nodo *ptr = testa;
    int i =0;
    while(ptr){
        X[i++]= ptr->data;
        ptr = ptr->next;
    }

    return X;
}


int* getRandomOrder(int n){
    int* ordinovabene = calloc(n,sizeof(int));

    for(int i=0; i<n; i++) ordinovabene[i]=i;

    for(int i=0; i<n; i++){
        int j = get_random()%n;
        int tmp= ordinovabene[i];
        ordinovabene[i]= ordinovabene[j];
        ordinovabene[j]= tmp;
    }
    return ordinovabene;
} 
void showFileContent(char filename[], int n){
    FILE *fp = NULL;
    int* ordinovabene= getRandomOrder(n);
    if((fp = fopen(filename, "rb"))== NULL){
        fprintf(stderr,"Impossibile aprire il file\n");
        exit(-1);
    }

    for(int i =0; i<n; i++){
        fseek(fp, ordinovabene[i]*sizeof(id), SEEK_SET);
        id r;
        size_t result = fread(&r, sizeof(id),1, fp);
        if(result!=0){
            printrecord(r);
        }
    }
    fclose(fp);
}
int main(int argc, char** argv) {
    params p = decodeParameters(argc, argv);
    printf("-------PUNTO A------\n");
    printf("filename = %s\n", p.filename);
    printf("n = %d\n", p.n);

    nodo* L = NULL;
    loadrecord(&L);

    printf("-------PUNTO B------\n");
    printf("Contenuto di L:\n");
    nodo* current = L;
    while (current != NULL) {
        printrecord(current->data);
        current = current->next;
    }

    id* X = getarray(L);

    // Deallocazione della memoria assegnata a X, se necessario

    selectionsort(X, listlen(L));
    printf("\nPUNTO C - Contenuto di X dopo l'ordinamento:\n");
    printArray(X, listlen(L));

    saveToFile(X, p.filename, listlen(L));
    printf("\nPUNTO D - Contenuto del file binario in ordine casuale:\n");
    showFileContent(p.filename, p.n);

    return 0;
}
