/*
 * Lezione 04 — Esercizi: operatori.
 *
 * Compila con:
 *   gcc -std=c17 -Wall -Wextra -Wpedantic esercizi.c -o esercizi
 */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

/* ============================================================
 * Esercizio 1 — Media aritmetica e geometrica di due double.
 * ============================================================
 *
 * - media aritmetica = (a + b) / 2
 * - media geometrica = sqrt(a * b)
 *
 * Per la radice quadrata in questa lezione NON usiamo <math.h>: implementa
 * un'approssimazione con il metodo di Newton (basta una decina di iterazioni).
 *   x_{n+1} = (x_n + v / x_n) / 2
 *
 * Hint: parti da x_0 = v / 2. Se v == 0, ritorna 0.
 */
static double radice_quadrata(double v) {
    (void)v;
    /* TODO: implementa Newton. */
    return 0.0;
}

static void esercizio_1(void) {
    printf("== Esercizio 1 — medie ==\n");
    double a = 4.0;
    double b = 9.0;
    (void)a; (void)b;
    (void)radice_quadrata;   /* silenzia "unused" finche' non la chiami davvero */
    /* TODO: calcola e stampa media aritmetica e geometrica usando radice_quadrata. */
}

/* ============================================================
 * Esercizio 2 — pari/dispari con & invece di %.
 * ============================================================
 *
 * Un numero e' pari se l'ultimo bit e' 0, dispari se e' 1.
 * Quindi: (n & 1) == 0  ⇒  pari.
 *
 * Implementa la funzione e_pari usando SOLO l'operatore &.
 */
static int e_pari(int n) {
    (void)n;
    /* TODO: ritorna 1 se n e' pari, 0 altrimenti, usando '&'. */
    return 0;
}

static void esercizio_2(void) {
    printf("\n== Esercizio 2 — pari/dispari con & ==\n");
    for (int i = 0; i < 6; ++i) {
        printf("%d -> %s\n", i, e_pari(i) ? "pari" : "dispari");
    }
}

/* ============================================================
 * Esercizio 3 — stampa gli 8 bit di un uint8_t.
 * ============================================================
 *
 * Dato un uint8_t, stampa la sua rappresentazione binaria a 8 cifre
 * (dal bit piu' significativo al meno significativo).
 *
 * Hint: per estrarre il bit i (i da 7 a 0):  (b >> i) & 1u
 */
static void stampa_bit(uint8_t b) {
    (void)b;
    /* TODO: stampa 8 caratteri '0'/'1', dal bit 7 al bit 0. */
}

static void esercizio_3(void) {
    printf("\n== Esercizio 3 — bit di un uint8_t ==\n");
    uint8_t valori[] = {0, 1, 2, 5, 170, 255};
    for (size_t i = 0; i < sizeof valori / sizeof valori[0]; ++i) {
        printf("%3" PRIu8 " = ", valori[i]);
        stampa_bit(valori[i]);
        putchar('\n');
    }
}

/* ============================================================
 * Esercizio 4 — max(a, b) con il ternario.
 * ============================================================
 *
 * Una sola riga, niente if.
 */
static int max_int(int a, int b) {
    (void)a; (void)b;
    /* TODO: ritorna il maggiore tra a e b usando l'operatore ternario. */
    return 0;
}

static void esercizio_4(void) {
    printf("\n== Esercizio 4 — max con ternario ==\n");
    printf("max(3, 7)  = %d\n", max_int(3, 7));
    printf("max(-2, -9)= %d\n", max_int(-2, -9));
    printf("max(5, 5)  = %d\n", max_int(5, 5));
}

/* ============================================================
 * Esercizio 5 — precedenza: 1 + 2 * 3 << 1.
 * ============================================================
 *
 * Calcola il valore A MENTE applicando la tabella di precedenza:
 *   - * ha precedenza piu' alta di +
 *   - + ha precedenza piu' alta di <<
 * Quindi l'espressione si valuta come ((1 + (2 * 3)) << 1).
 *
 * Scrivi qui sotto la TUA risposta nella variabile 'mia_risposta', poi
 * lascia che il programma calcoli quella vera e confronti.
 */
static void esercizio_5(void) {
    printf("\n== Esercizio 5 — precedenza ==\n");
    int mia_risposta = 0;          /* TODO: scrivi qui la tua previsione. */

    /* NOTA: il compilatore emette un warning -Wparentheses su questa riga.
     * E' VOLUTO. Te lo segnala perche' il misto di '+' e '<<' senza parentesi
     * confonde chi legge: e' parte della lezione sulla precedenza. */
    int risposta_vera = 1 + 2 * 3 << 1;

    printf("la mia risposta = %d\n", mia_risposta);
    printf("vera risposta   = %d\n", risposta_vera);
    printf("indovinato? %s\n", mia_risposta == risposta_vera ? "si'" : "no");
}

int main(void) {
    esercizio_1();
    esercizio_2();
    esercizio_3();
    esercizio_4();
    esercizio_5();
    return 0;
}
