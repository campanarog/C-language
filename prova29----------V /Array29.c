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

void error(const char *err) {
    fprintf(stderr, "%s", err);
    exit(EXIT_FAILURE);
}

typedef struct {
    int min;
    int max;
    int n;
} params;

params readinput(int argc, char **argv) {
    if (argc != 4) error("Params to insert : n_min, n_max, n\n");
    params p;
    p.min = atoi(argv[1]);
    p.max = atoi(argv[2]);
    p.n = atoi(argv[3]);
    if (p.min < 7 || p.min > 12) error("Min number must be in range 7-12\n");
    if (p.max < 7 || p.max > 12) error("Max number must be in range 7-12\n");
    if (p.n <= 0) error("N must be greater than 0\n");
    return p;
}

char *generatestring(int length) {
    char *array = (char *)malloc((length + 1) * sizeof(char));
    if (array == NULL) error("Memory allocation failed\n");
    char alfabeto[] = "aieou";
    for (int i = 0; i < length; i++) {
        array[i] = alfabeto[get_random() % 5];
    }
    array[length] = 0;
    return array;
}

char** makearray(int min , int max, int n){
    char**A = (char**)malloc(n*sizeof(char*));
    for(int i =0; i<n; i++){
        int x = get_random()%(max-min+1)+min;
        A[i]= generatestring(x);
    }
    return A;
}

void sortarray(char**A, int n){
    for(int i =1; i<n; i++){
        char* tmp = A[i];
        int j =j-1;
        while(j>=0 && strcmp(A[j], tmp)>0){
            A[j+1] = A[j];
            j--;
        }
        A[j+1]= tmp;
    }
}

void printarray(char**A ,int n){
    for(int i =0; i<n; i++){
        printf("%s\n", A[i]);
    }
}

char* concatstring(char**A, int n){
    char* nuova_stringa_da_concatenare= (char*)malloc(n*sizeof(char));
    if(nuova_stringa_da_concatenare == NULL ) error("Memory allocation failed\n");
    for(int i =0; i<n; i++){
        strcat(nuova_stringa_da_concatenare, A[i]);
    }
    return nuova_stringa_da_concatenare;
}

void replaceCharacters(char*B){
    int i =0; 
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
   params p =readinput(argc, argv);
   printf("Punto A - parametri inseriti sono min = %d, max = %d, n= %d\n", p.min, p.max, p.n);
   char**A = makearray(p.min , p.max, p.n);
    printf("\nPUNTO B\n");
    printarray(A, p.n);

    sortarray(A, p.n);
    char* B = concatstring(A, p.n);
    printf("\n Punto C: %s\n", B);
    replaceCharacters(B);
    printf("\n Punto D: %s\n", B);
}