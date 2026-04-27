/*
 * Lezione 06 — Cicli.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

static bool e_primo(int n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (int d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

int main(void) {
    /* for classico: Fibonacci */
    printf("Fibonacci:");
    long long a = 0, b = 1;
    for (int i = 0; i < 15; ++i) {
        printf(" %lld", a);
        long long t = a + b;
        a = b;
        b = t;
    }
    putchar('\n');

    /* while */
    int n = 1000;
    while (!e_primo(n)) ++n;
    printf("primo primo > 1000 = %d\n", n);

    /* do..while: ripeti finche' input invalido */
    int scelta = 0;
    /* simuliamo un input corretto al primo tentativo per non bloccare */
    do {
        scelta = 2;
        if (scelta < 1 || scelta > 3) {
            printf("scelta non valida, riprova\n");
        }
    } while (scelta < 1 || scelta > 3);
    printf("scelta: %d\n", scelta);

    /* Iterazione sicura all'indietro con size_t */
    const char *parola = "Riccardo";
    size_t len = 0;
    while (parola[len] != '\0') ++len;

    printf("al contrario: ");
    for (size_t i = len; i-- > 0; ) {
        putchar(parola[i]);
    }
    putchar('\n');

    /* Annidamento con early exit tramite flag */
    int matrice[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    int target = 5;
    bool trovato = false;
    size_t ri = 0, rj = 0;
    for (size_t i = 0; i < 3 && !trovato; ++i) {
        for (size_t j = 0; j < 3 && !trovato; ++j) {
            if (matrice[i][j] == target) {
                trovato = true;
                ri = i; rj = j;
            }
        }
    }
    if (trovato) {
        printf("target %d trovato in [%zu][%zu]\n", target, ri, rj);
    }

    return 0;
}
