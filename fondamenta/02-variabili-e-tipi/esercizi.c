/*
 * Lezione 02 — Esercizi: variabili e tipi.
 *
 * Compila con:
 *   gcc -std=c17 -Wall -Wextra -Wpedantic esercizi.c -o esercizi
 *
 * Riempi i blocchi TODO. Ogni esercizio e' isolato in una funzione: il main
 * le chiama in ordine. Se un esercizio non compila, commenta la chiamata
 * corrispondente per andare avanti.
 */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

/* ============================================================
 * Esercizio 1 — sizeof dei tipi sulla tua macchina.
 * ============================================================
 *
 * Stampa sizeof di: char, int, long, long long, float, double, void*.
 * Usa lo specificatore %zu (size_t).
 *
 * Domande da farti dopo aver eseguito:
 *   - long e' 4 o 8 byte? (su Linux 64-bit dovrebbe essere 8)
 *   - sizeof(void*) coincide con sizeof(long)? perche'?
 */
static void esercizio_1(void) {
    printf("== Esercizio 1 — sizeof ==\n");
    /* TODO: stampa sizeof per ciascun tipo. */
    printf("void - %zu\nlong - %zu\n",sizeof(void*),sizeof(long));
}

/* ============================================================
 * Esercizio 2 — uint32_t con PRIu32.
 * ============================================================
 *
 * Dichiara una variabile uint32_t e stampala usando la macro PRIu32 di
 * <inttypes.h>. La sintassi e' un po' insolita: la macro e' una stringa
 * che si concatena al formato.
 *
 *   uint32_t n = 4000000000u;
 *   printf("n = %" PRIu32 "\n", n);
 */
static void esercizio_2(void) {
    printf("\n== Esercizio 2 — uint32_t + PRIu32 ==\n");
    /* TODO: dichiara un uint32_t e stampalo con PRIu32. */
    uint32_t n = 40000000u;
    printf("n = %" PRIu32 "\n", n);
}

/* ============================================================
 * Esercizio 3 — modifica di una const.
 * ============================================================
 *
 * Decommenta le righe sotto e prova a compilare. Leggi attentamente
 * l'errore del compilatore: e' parte dell'esercizio.
 * Poi rimettilo come commento per far compilare di nuovo il file.
 */
static void esercizio_3(void) {
    printf("\n== Esercizio 3 — const ==\n");
    const double PI = 3.14159;
    printf("PI = %.5f\n", PI);

    /* TODO: decommenta e osserva l'errore.
     *   PI = 3.0;
     *   printf("PI dopo modifica = %.5f\n", PI);
     */
}

/* ============================================================
 * Esercizio 4 — int / int vs (double)int / int.
 * ============================================================
 *
 * Dati a = 7, b = 2:
 *   - calcola e stampa a / b (divisione intera).
 *   - calcola e stampa (double)a / b (divisione floating).
 * Spiega a te stesso perche' i due risultati differiscono.
 */
static void esercizio_4(void) {
    printf("\n== Esercizio 4 — divisione intera vs double ==\n");
    int a = 7;
    int b = 2;
    (void)a; (void)b;
    /* TODO: stampa a / b e (double)a / b con specificatori corretti. */
    printf("%.2f\n",(double)a/b);
    printf("%f\n",(float)a/b);
}

int main(void) {
    esercizio_1();
    esercizio_2();
    esercizio_3();
    esercizio_4();
    return 0;
}
