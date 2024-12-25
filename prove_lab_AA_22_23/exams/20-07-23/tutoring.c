#include <stdio.h>
#include <stdlib.h>

typedef struct params {
    int n;
    int m;
} Params;

Params decodeParameters(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Errore nel numero di parametri.");
        exit(1);
    }

    Params p;
    p.n = atoi(argv[1]);
    p.m = atoi(argv[2]);

    if (p.n < 3 || p.n > 7) {
        fprintf(stderr, "Errore con il valore di N.");
        exit(1);
    }

    if (p.m < 3 || p.m > 7) {
        fprintf(stderr, "Errore con il valore di M.");
        exit(1);
    }

    return p;
}

int getLimit() {
    int h;
    printf("Inserisci un numero compreso tra 10 e 100: ");
    scanf("%d", &h);

    if (h < 10 || h > 100) {
        fprintf(stderr, "Errore con il valore di H.");
        exit(1);
    }

    return h;
}

unsigned int get_random() {
    static unsigned int m_w = 424242;
    static unsigned int m_z = 242424;
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}

int ***getRandomMatrix(int n, int m, int h) {
    int ***A = calloc(n, sizeof(int**));

    for (int i = 0; i < n; i++) {
        A[i] = calloc(m, sizeof(int*));
        for (int j = 0; j < m; j++) {
            A[i][j] = malloc(sizeof(int));
            *A[i][j] = get_random() % (h+1);
        }
    }

    return A;
}

void removeMax(int ***A, int n, int col) {
    int max = -1;
    int idx = -1;

    for (int i = 0; i < n; i++) {
        if (A[i][col] != NULL && max < *A[i][col]) {
            max = *A[i][col];
            idx = i;
        }
    }

    if (idx != -1) {
        free(A[idx][col]);
        A[idx][col] = NULL;
    }
}

void modifyMatrix(int ***A, int n, int m) {
    for (int j = 0; j < m; j++)
        for (int x = 1; x <= 3; x++)
            removeMax(A, n, j);
}

void rowMinNull(int ***A, int n, int m) {
    int min = m;
    int idx = -1;

    for (int i = 0; i < n; i++) {
        int num_null = 0;

        for (int j = 0; j < m; j++) {
            if (A[i][j] == NULL)
                num_null++;
        }

        if (num_null < min) {
            min = num_null;
            idx = i;
        }
    }

    if (idx != -1) {
        printf("Punto D - Riga con minor numero di NULL: \n");
        for (int j = 0; j < m; j++) {
            if (A[idx][j] == NULL) printf("* ");
            else printf("%d ", *A[idx][j]);
        }
    }
    else {
        printf("Punto D: Matrice vuota.");
    }
}

int main(int argc, char **argv) {
    Params p = decodeParameters(argc, argv);
    printf("Punto A - Parametri inseriti: n=%d, m=%d \n", p.n, p.m);

    int h = getLimit();
    int ***A = getRandomMatrix(p.n, p.m, h);

    printf("Punto B - Contenuto di A: \n");
    for (int i = 0; i < p.n; i++) {
        for (int j = 0; j < p.m; j++) {
            printf("%d ", *A[i][j]);
        }
        printf("\n");
    }

    modifyMatrix(A, p.n, p.m);

    printf("Punto C - Contenuto di A dopo la modifica: \n");
    for (int i = 0; i < p.n; i++) {
        for (int j = 0; j < p.m; j++) {
            if (A[i][j] == NULL) printf("* ");
            else printf("%d ", *A[i][j]);
        }
        printf("\n");
    }

    rowMinNull(A, p.n, p.m);

    return 0;
}