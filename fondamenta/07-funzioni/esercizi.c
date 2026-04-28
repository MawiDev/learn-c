/*
 * Lezione 07 — Esercizi: funzioni.
 *
 * Compila con:
 *   gcc -std=c17 -Wall -Wextra -Wpedantic esercizi.c -o esercizi
 */

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

/* ============================================================
 * Esercizio 1 — abs_int senza <stdlib.h>.
 * ============================================================
 *
 * Ritorna |x|. Attento al caso INT_MIN: il suo opposto non e'
 * rappresentabile in un int (overflow signed = UB). Per ora puoi ignorarlo,
 * o gestirlo ritornando INT_MIN o un valore convenzionale.
 */
static int abs_int(int x) {
    (void)x;
    /* TODO: ritorna il valore assoluto. */
    return 0;
}

/* ============================================================
 * Esercizio 2 — e_primo + primi 50 primi.
 * ============================================================
 *
 * Implementa bool e_primo(int n) (test fino a sqrt(n) — usa d*d <= n).
 * Poi scrivi un loop che stampa i primi 50 numeri primi, uno per riga
 * o separati da spazio.
 */
static bool e_primo(int n) {
    (void)n;
    /* TODO: implementa. */
    return false;
}

/* ============================================================
 * Esercizio 3 — scambia(a, b).
 * ============================================================
 *
 * Una funzione che scambia il contenuto di due int. In C l'unico modo e'
 * passare puntatori — ricorda che i parametri sono passati per valore.
 */
static void scambia(int *a, int *b) {
    (void)a; (void)b;
    /* TODO: scambia *a e *b usando una variabile temporanea. */
}

/* ============================================================
 * Esercizio 4 — DivResult dividi(a, b).
 * ============================================================
 *
 * Restituisce quoziente e resto in una sola chiamata via struct.
 * Se b == 0, ritorna {0, 0} e segnala l'errore via puntatore di output o,
 * piu' semplicemente, un campo bool nella struct (a tua scelta).
 */
typedef struct {
    int quoziente;
    int resto;
    bool ok;          /* false se divisione per zero */
} DivResult;

static DivResult dividi(int a, int b) {
    (void)a; (void)b;
    /* TODO: gestisci b == 0 e calcola q, r. */
    DivResult r = {0, 0, false};
    return r;
}

/* ============================================================
 * Esercizio 5 — Fibonacci ricorsivo vs iterativo.
 * ============================================================
 *
 * Implementa due versioni di fib(n):
 *  - fib_rec: ricorsiva pura (catastrofica per n grandi, ma educational)
 *  - fib_iter: iterativa, lineare
 *
 * Misura il tempo per n = 40 con clock(). Vedrai una differenza enorme.
 */
static long long fib_rec(int n) {
    (void)n;
    /* TODO: caso base + chiamata ricorsiva. */
    return 0;
}

static long long fib_iter(int n) {
    (void)n;
    /* TODO: due variabili a, b, loop n volte. */
    return 0;
}

/* ---------- main: orchestra gli esercizi ---------- */

int main(void) {
    /* Esercizio 1 */
    printf("== Esercizio 1 — abs_int ==\n");
    printf("abs_int(-7) = %d\n", abs_int(-7));
    printf("abs_int( 0) = %d\n", abs_int(0));
    printf("abs_int(42) = %d\n", abs_int(42));

    /* Esercizio 2 */
    printf("\n== Esercizio 2 — primi 50 primi ==\n");
    int trovati = 0;
    for (int n = 2; trovati < 50; ++n) {
        if (e_primo(n)) {
            printf("%d ", n);
            ++trovati;
        }
    }
    putchar('\n');

    /* Esercizio 3 */
    printf("\n== Esercizio 3 — scambia ==\n");
    int x = 1, y = 9;
    scambia(&x, &y);
    printf("dopo scambia: x=%d, y=%d (atteso x=9, y=1)\n", x, y);

    /* Esercizio 4 */
    printf("\n== Esercizio 4 — dividi ==\n");
    DivResult r = dividi(17, 5);
    printf("17 / 5 = %d, resto %d (ok=%d)\n", r.quoziente, r.resto, r.ok);
    DivResult r0 = dividi(1, 0);
    printf("1 / 0 -> ok=%d (atteso 0)\n", r0.ok);

    /* Esercizio 5 */
    printf("\n== Esercizio 5 — Fibonacci rec vs iter ==\n");
    int n = 40;
    clock_t t0 = clock();
    long long ri = fib_iter(n);
    clock_t t1 = clock();
    long long rr = fib_rec(n);
    clock_t t2 = clock();
    printf("fib_iter(%d) = %lld in %.3f s\n",
           n, ri, (double)(t1 - t0) / CLOCKS_PER_SEC);
    printf("fib_rec (%d) = %lld in %.3f s\n",
           n, rr, (double)(t2 - t1) / CLOCKS_PER_SEC);
    return 0;
}
