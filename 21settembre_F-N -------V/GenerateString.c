#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned int get_random() {
    static unsigned int m_w = 424242;
    static unsigned int m_z = 242424;
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}

typedef struct{
    int min;
    int max;
    int n;
}params;

params decodeparams(int argc, char**argv){
    if(argc != 4){
        fprintf(stderr,"Usage %s <min>, <max>, <n>", argv[0]);
        exit(EXIT_FAILURE);
    }
    params p;
    p.min = atoi(argv[1]);
    p.max = atoi(argv[2]);
    p.n = atoi(argv[3]);

    if(p.min < 7 || p.min> 12){
        fprintf(stderr,"MIN must be in range 7-12\n");
        exit(EXIT_FAILURE);
    }
    if(p.max < 7 || p.max> 12){
        fprintf(stderr,"MAX must be in range 7-12\n");
        exit(EXIT_FAILURE);
    }
    /*if(p.max > p.min){
        fprintf(stderr,"Max cant be greater\n");
        exit(EXIT_FAILURE);
    }*/
    if(p.n <= 0){
        fprintf(stderr," N cant be 0\n");
    }
    return p;
}

// TO KNOW
// IN C, the explicit cast to (char*) is not necessary because malloc returns a void*, which is automatically promoted to other pointer types.
//However, in C++, an explicit cast is required because C++ is stricter about type safety and does not implicitly convert void* to other pointer types.

char* generateString(int min, int max) {
    const char vowels[] = "aeiou";
    int length = rand() % (max - min + 1) + min;
    char *str = malloc((length + 1) * sizeof(char)); // +1 per il carattere di terminazione '\0'

    for (int i = 0; i < length; i++) {
        str[i] = vowels[rand() % 5]; // Scegli casualmente una vocale
    }

    str[length] = '\0'; // Termina la stringa
    return str;
}

char** makeArray(int n, int min, int max) {
    char **A = malloc(n * sizeof(char*)); // Array di stringhe
    for (int i = 0; i < n; i++) {
        A[i] = generateString(min, max); // Genera una stringa con lunghezza casuale tra min e max
    }
    return A;
}

void sortarray(char** A, int n){
    for(int i =1; i<n; i++){
        char *tmp = A[i];
        int j= i-1;
        while(j>= 0 && strcmp(A[j], tmp)>0){
            A[j+1] = A[j];
            j--;
        }
        A[j+1] = tmp;
    }
}
void printarray(char**A, int n){
    for(int i=0; i<n; i++){
        printf("%s\n", A[i]);
    }
}

char* concatstring(char**A, int n){
    // nella funzione ovviamente metto la prima stringa che ho
    char* array_nuovo = malloc(n*15*sizeof(char));
    for(int i=0; i<n; i++){
        strcat(array_nuovo, A[i]);
    }
    return array_nuovo;
}

void replaceCharacters(char *B){
    int i=0; 
    char last = ' ';
    while(B[i]){
        if(B[i] == 'u' && last == 'e'){
            B[i] = '*';
        }
        last = B[i];
        i++;
    }
}

int main(int argc, char**argv){
    params p= decodeparams(argc, argv);
    printf("Punto A -----> min = %d, max = %d , n = %d", p.min , p.max, p.n);
    char** A = makeArray(p.n , p.min, p.max); 
    printf("\nPUNTO B\n");
    printarray(A, p.n);
   sortarray(A, p.n);
    char *B = concatstring(A, p.n);
    printf("\n Punto C: %s\n", B);
    replaceCharacters(B);
    printf("\n Punto D: %s\n", B);

}