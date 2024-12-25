 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING_LENGTH 255

typedef struct Person {
    char nome[MAX_STRING_LENGTH];
    char cognome[MAX_STRING_LENGTH];
    int eta;
    int peso;
    int altezza;
    char sesso;
    struct Person* next;
} Person;

typedef struct {
    char input[MAX_STRING_LENGTH];
    char output[MAX_STRING_LENGTH];
} FileNames;

FileNames readInput(int argc, char *argv[]) {
    FileNames filenames;
    if (argc != 3) {
        fprintf(stderr, "Errore: numero di parametri errato.\n");
        exit(EXIT_FAILURE);
    }

    if (strstr(argv[1], ".txt") == NULL || strstr(argv[2], ".txt") == NULL) {
        fprintf(stderr, "Errore: i file devono avere estensione .txt.\n");
        exit(EXIT_FAILURE);
    }

    strncpy(filenames.input, argv[1], MAX_STRING_LENGTH);
    strncpy(filenames.output, argv[2], MAX_STRING_LENGTH);

    printf("Input file: %s\n", filenames.input);
    printf("Output file: %s\n", filenames.output);

    return filenames;
}

Person* readFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Errore apertura file di input");
        exit(EXIT_FAILURE);
    }

    Person *head = NULL;

    while (!feof(file)) {
        Person *new_person = (Person *)malloc(sizeof(Person));
        if (fscanf(file, "%s %s %d %d %d %c", new_person->nome, new_person->cognome, &new_person->eta,
                   &new_person->peso, &new_person->altezza, &new_person->sesso) != 6) {
            free(new_person);
            break;
        }

        new_person->next = NULL;

        if (head == NULL) {
            head = new_person;
        } else {
            Person *current = head;
            Person *prev = NULL;
            while (current != NULL && current->eta < new_person->eta) {
                prev = current;
                current = current->next;
            }
            if (prev == NULL) { // Inserimento in testa
                new_person->next = head;
                head = new_person;
            } else { // Inserimento dopo prev
                prev->next = new_person;
                new_person->next = current;
            }
        }
    }

    fclose(file);
    return head;
}

double imc(Person *person) {
    return (double)person->peso / ((person->altezza / 100.0) * (person->altezza / 100.0));
}

Person* getMax(Person *head) {
    if (head == NULL) {
        return NULL;
    }

    Person *max_person = head;
    Person *current = head;

    while (current != NULL) {
        if (imc(current) > imc(max_person)) {
            max_person = current;
        }
        current = current->next;
    }

    return max_person;
}

void writeFile(const char *filename, Person *head) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Errore apertura file di output");
        exit(EXIT_FAILURE);
    }

    Person *current = head;
    while (current != NULL) {
        fprintf(file, "%s %s %d %d %d %c\n", current->nome, current->cognome, current->eta,
                current->peso, current->altezza, current->sesso);
        current = current->next;
    }

    fclose(file);
}

void printList(Person *head) {
    Person *current = head;
    while (current != NULL) {
        printf("%s %s %d %d %d %c\n", current->nome, current->cognome, current->eta,
               current->peso, current->altezza, current->sesso);
        current = current->next;
    }
}

Person* removeMaxIMC(Person **head) {
    if (*head == NULL) {
        return NULL;
    }

    Person *max_person = getMax(*head);
    if (max_person == NULL) {
        return NULL;
    }

    if (*head == max_person) {
        *head = (*head)->next;
    } else {
        Person *current = *head;
        while (current->next != max_person) {
            current = current->next;
        }
        current->next = max_person->next;
    }

    max_person->next = NULL;
    return max_person;
}

int main(int argc, char *argv[]) {
    FileNames filenames = readInput(argc, argv);

    Person *A = readFile(filenames.input);
    printf("Lista A dopo inserimento:\n");
    printList(A);

    Person *max_imc_person = getMax(A);
    if (max_imc_person != NULL) {
        printf("Elemento con il massimo IMC:\n");
        printf("%s %s %d %d %d %c\n", max_imc_person->nome, max_imc_person->cognome, max_imc_person->eta,
               max_imc_person->peso, max_imc_person->altezza, max_imc_person->sesso);
    }

    Person *B = NULL;
    for (int i = 0; i < 3; i++) {
        Person *person_to_move = removeMaxIMC(&A);
        if (person_to_move != NULL) {
            person_to_move->next = B;
            B = person_to_move;
        }
    }

    printf("Lista A dopo spostamento:\n");
    printList(A);

    printf("Lista B dopo spostamento:\n");
    printList(B);

    writeFile(filenames.output, B);

    return 0;
}