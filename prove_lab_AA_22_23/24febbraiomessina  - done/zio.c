#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


typedef struct {
    int n;
    char a, b;
} params;

unsigned int get_random() {
    static unsigned int m_w = 123456;
    static unsigned int m_z = 789123;
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}

typedef struct nodo {
    char data;
    struct nodo *next;
} nodo;

char getconsonant() {
    char consonanti[] = "bcdfghlmnpqrstvzyx";
    return consonanti[get_random() % 21];
}

char getvowel() {
    char vocali[] = "aeiou";
    return vocali[get_random() % 5];
}

params readinput(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage %s <n>, <a>, <b>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    params p;
    p.n = atoi(argv[1]);
    p.a = argv[2][0];
    p.b = argv[3][0];

    if (p.n < 5 || p.n > 10) {
        fprintf(stderr, "n must be in range 5-10\n");
        exit(EXIT_FAILURE);
    }
    if (p.a < 'a' || p.a > 'z') {
        fprintf(stderr, "a must be in range a-z\n");
        exit(EXIT_FAILURE);
    }
    if (p.b < 'a' || p.b > 'z') {
        fprintf(stderr, "b must be in range a-z\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

void push(nodo **testa, char data) {
    nodo *n = malloc(sizeof(nodo));
    if (n == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(-1);
    }
    n->data = data;
    n->next = *testa;
    *testa = n;
}

void printstack(nodo *testa) {
    printf("** TOP-> ");
    while (testa) {
        printf("%c ", testa->data);
        testa = testa->next;
    }
    printf("<- BOTTOM **\n");
}

void fillstack(nodo **testa, params p) {
    for (int i = 0; i < p.n; i++) {
        char x = '1' + get_random() % 9;
        int pushiamo = x - '0';
        for (int j = 0; j < pushiamo; j++) {
            char c;
            if (x >= '1' && x <= '4') {
                c = getvowel();
            } else {
                c = getconsonant();
            }
            if (c == p.a) {
                c = '*';
            } else if (c == p.b) {
                c = '?';
            }
            push(testa, c);
        }
        push(testa, x);
    }
}

void freestack(nodo *testa) {
    while (testa) {
        nodo *temp = testa;
        testa = testa->next;
        free(temp);
    }
}

int main(int argc, char *argv[]) {
    srand((unsigned int)time(NULL));

    params p = readinput(argc, argv);
    nodo *testa = NULL;

    fillstack(&testa, p);
    printstack(testa);
    freestack(testa);

    return 0;
}
