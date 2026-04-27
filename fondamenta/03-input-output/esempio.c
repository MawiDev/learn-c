/*
 * Lezione 03 — Input e Output.
 *
 * Compila:
 *   gcc -std=c17 -Wall -Wextra -Wpedantic esempio.c -o esempio
 */

#include <stdio.h>
#include <string.h>

int main(void) {
    /* === OUTPUT === */
    printf("== Stampe formattate ==\n");
    printf("intero    : %d\n", 42);
    printf("float     : %.3f\n", 3.14159);
    printf("esadec    : 0x%X\n", 255);
    printf("stringa   : %s\n", "ciao");
    printf("larghezza : |%5d|%-5d|%05d|\n", 42, 42, 42);

    puts("\n== puts non richiede \\n ==");

    /* === INPUT === */
    printf("\n== Input ==\n");
    printf("Come ti chiami? ");
    fflush(stdout);

    char nome[64];
    if (fgets(nome, sizeof nome, stdin) == NULL) {
        fprintf(stderr, "Errore di lettura\n");
        return 1;
    }

    /* fgets include il '\n' finale: rimuoviamolo se presente */
    size_t len = strlen(nome);
    if (len > 0 && nome[len - 1] == '\n') {
        nome[len - 1] = '\0';
    }

    printf("Ciao %s!\n", nome);

    /* Leggere un intero con scanf, controllando il valore di ritorno */
    printf("Quanti anni hai? ");
    fflush(stdout);

    int eta;
    if (scanf("%d", &eta) != 1) {
        fprintf(stderr, "Input non valido: mi aspettavo un intero.\n");
        return 1;
    }

    printf("Hai %d anni. Tra 10 anni ne avrai %d.\n", eta, eta + 10);

    /* stderr: separato da stdout */
    fprintf(stderr, "[debug] fine programma\n");

    return 0;
}
