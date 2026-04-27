/*
 * Lezione 02 — Variabili e tipi.
 *
 * Compila:
 *   gcc -std=c17 -Wall -Wextra -Wpedantic esempio.c -o esempio
 */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

int main(void) {
    /* Tipi "classici" */
    int      eta      = 30;
    double   altezza  = 1.82;
    char     iniziale = 'R';

    /* Tipo booleano (C99+) */
    bool     attivo   = true;

    /* Dimensioni esatte (C99+) */
    uint8_t  byte_valore = 255;
    int32_t  contatore   = -1;
    uint64_t nanosecondi = 1234567890ULL;

    /* const: promessa di immutabilità */
    const double PI = 3.14159265358979;

    /* Stampa con specificatori di formato corretti */
    printf("eta       = %d\n", eta);
    printf("altezza   = %.2f m\n", altezza);
    printf("iniziale  = %c (codice ASCII %d)\n", iniziale, iniziale);
    printf("attivo    = %s\n", attivo ? "si" : "no");
    printf("byte      = %" PRIu8  "\n", byte_valore);
    printf("contatore = %" PRId32 "\n", contatore);
    printf("nanosec   = %" PRIu64 "\n", nanosecondi);
    printf("PI        = %.10f\n", PI);

    /* sizeof: operatore a tempo di compilazione */
    printf("\n-- Dimensioni sulla tua macchina --\n");
    printf("sizeof(char)      = %zu\n", sizeof(char));
    printf("sizeof(int)       = %zu\n", sizeof(int));
    printf("sizeof(long)      = %zu\n", sizeof(long));
    printf("sizeof(long long) = %zu\n", sizeof(long long));
    printf("sizeof(float)     = %zu\n", sizeof(float));
    printf("sizeof(double)    = %zu\n", sizeof(double));
    printf("sizeof(void*)     = %zu\n", sizeof(void*));

    /* Divisione intera vs floating point */
    int a = 7, b = 2;
    printf("\n%d / %d (int)    = %d\n", a, b, a / b);
    printf("%d / %d (double) = %.2f\n", a, b, (double)a / b);

    return 0;
}
