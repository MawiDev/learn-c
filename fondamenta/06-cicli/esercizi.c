/*
 * Lezione 06 — Esercizi: cicli.
 *
 * Compila con:
 *   gcc -std=c17 -Wall -Wextra -Wpedantic esercizi.c -o esercizi
 */

#include <stdio.h>
#include <stdbool.h>

/* ============================================================
 * Esercizio 1 — Primi 20 Fibonacci con un for.
 * ============================================================
 *
 * Sequenza: 0 1 1 2 3 5 8 13 21 ...
 * Stampa i primi 20 termini separati da spazio. Usa long long per evitare
 * overflow (anche se a 20 stiamo larghi).
 *
 * Hint: tieni due variabili a, b. Ad ogni passo: stampa a, calcola
 * (a, b) = (b, a + b).
 */
static void esercizio_1(void) {
    printf("== Esercizio 1 — Fibonacci ==\n");
    /* TODO: stampa i primi 20 termini di Fibonacci con un for. */
}

/* ============================================================
 * Esercizio 2 — Somma e media finche' utente non inserisce 0.
 * ============================================================
 *
 * Leggi numeri interi da stdin in un do/while (vogliamo eseguire il
 * corpo almeno una volta). Termina quando l'utente inserisce 0.
 * Alla fine stampa quanti numeri (escluso lo 0), la somma e la media.
 *
 * Attenzione: se l'utente inserisce SOLO 0, la media e' indefinita.
 * Gestisci questo caso esplicitamente.
 */
static void esercizio_2(void) {
    printf("\n== Esercizio 2 — somma/media (0 per terminare) ==\n");
    /* TODO: leggi numeri con scanf, accumula somma e contatore, stampa
     *       somma e media. */
}

/* ============================================================
 * Esercizio 3 — Decremento sicuro di un size_t da n-1 a 0.
 * ============================================================
 *
 * Ricordi la trappola della lezione?
 *   for (size_t i = n - 1; i >= 0; --i) { ... }    // INFINITO
 *
 * Scrivi qui sotto il loop in versione corretta. La forma idiomatica e':
 *   for (size_t i = n; i-- > 0; ) { ... }
 * Spiegati a voce perche' funziona (suggerimento: postdecremento).
 */
static void esercizio_3(void) {
    printf("\n== Esercizio 3 — decremento sicuro ==\n");
    size_t n = 5;
    (void)n;
    /* TODO: scrivi il loop che stampa "i = 4", "i = 3", ..., "i = 0". */
}

/* ============================================================
 * Esercizio 4 — Primo numero primo > 1000.
 * ============================================================
 *
 * Scorri n da 1001 in su. Per ogni n, controlla se e' primo dividendo per
 * tutti i d tra 2 e sqrt(n). Appena ne trovi uno primo, stampalo e break.
 *
 * Hint per evitare <math.h>: invece di sqrt(n), itera finche' d*d <= n.
 */
static bool e_primo(int n) {
    (void)n;
    /* TODO: ritorna true se n e' primo, false altrimenti. */
    return false;
}

static void esercizio_4(void) {
    printf("\n== Esercizio 4 — primo primo > 1000 ==\n");
    (void)e_primo;   /* silenzia "unused" finche' non la chiami davvero */
    /* TODO: trova il primo numero primo > 1000 e stampalo (usa e_primo). */
}

/* ============================================================
 * Esercizio 5 — Tavola pitagorica 10x10.
 * ============================================================
 *
 * Stampa una griglia 10x10 in cui la cella (i, j) contiene i*j (i, j da 1 a 10).
 * Allinea le colonne con %4d cosi' la tabella e' leggibile.
 */
static void esercizio_5(void) {
    printf("\n== Esercizio 5 — tavola pitagorica ==\n");
    /* TODO: due cicli annidati, %4d per allineare. */
}

int main(void) {
    esercizio_1();
    esercizio_2();
    esercizio_3();
    esercizio_4();
    esercizio_5();
    return 0;
}
