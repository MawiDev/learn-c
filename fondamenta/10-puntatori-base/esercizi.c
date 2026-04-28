/*
 * Lezione 10 — Esercizi: puntatori (basi).
 *
 * Compila con:
 *   gcc -std=c17 -Wall -Wextra -Wpedantic esercizi.c -o esercizi
 */

#include <stdio.h>
#include <stddef.h>

/* ============================================================
 * Esercizio 1 — scambia(int *a, int *b).
 * ============================================================
 *
 * Stesso esercizio della lezione 07, ora che abbiamo capito davvero cosa
 * succede dietro le quinte: stai dereferenziando puntatori e usando
 * l'address-of (&) nel main.
 */
static void scambia(int *a, int *b) {
    (void)a; (void)b;
    /* TODO: scambia *a e *b con una temp. */
}

/* ============================================================
 * Esercizio 2 — zero_array con aritmetica dei puntatori.
 * ============================================================
 *
 * Azzera tutti gli elementi dell'array. VINCOLO: niente '[]'. Usa solo
 * dereferenza (*) e incremento di puntatore (++).
 *
 * Schema tipico:
 *   for (int *end = a + n; a < end; ++a) *a = 0;
 */
static void zero_array(int *a, size_t n) {
    (void)a; (void)n;
    /* TODO: azzera tutti gli n elementi senza usare '[]'. */
}

/* ============================================================
 * Esercizio 3 — cerca: ritorna puntatore o NULL.
 * ============================================================
 *
 * Cerca target negli n elementi di a. Se lo trovi, ritorna il puntatore
 * all'elemento. Altrimenti ritorna NULL. E' un'idioma classico del C:
 * "ritorna un puntatore valido o NULL come segnale di assenza".
 */
static int *cerca(int *a, size_t n, int target) {
    (void)a; (void)n; (void)target;
    /* TODO: scorri e ritorna &a[i] al primo match, NULL se non trovato. */
    return NULL;
}

/* ============================================================
 * Esercizio 4 — Conta gli spazi con un puntatore che scorre.
 * ============================================================
 *
 * Stile idiomatico:
 *   for (const char *p = s; *p; ++p) {
 *       if (*p == ' ') ++count;
 *   }
 * Ritorna il numero totale di spazi ' '.
 */
static size_t conta_spazi(const char *s) {
    (void)s;
    /* TODO: scorri con puntatore++ e conta gli spazi. */
    return 0;
}

/* ============================================================
 * Esercizio 5 — int *p vs int *const p.
 * ============================================================
 *
 * Differenza:
 *   int *p          : posso modificare *p E posso riassegnare p.
 *   int *const p    : posso modificare *p, NON posso riassegnare p.
 *   const int *p    : NON posso modificare *p, posso riassegnare p.
 *
 * Decommenta le righe TODO una alla volta e osserva quale produce errore.
 */
static void esercizio_5(void) {
    printf("\n== Esercizio 5 — const e puntatori ==\n");
    int x = 1;
    int y = 2;

    int *p = &x;
    *p = 10;          /* OK: modifico x via p */
    p = &y;           /* OK: riassegno p */
    printf("p mutabile: x=%d, y=%d, *p=%d\n", x, y, *p);

    int *const q = &x;
    *q = 20;          /* OK: modifico x via q */
    /* TODO: decommenta e prova a compilare:
     *   q = &y;       // ERRORE: q e' const, non lo posso riassegnare
     */
    printf("q (int *const): *q=%d\n", *q);

    const int *r = &x;
    /* TODO: decommenta e prova a compilare:
     *   *r = 30;      // ERRORE: r punta a int const
     */
    r = &y;           /* OK: riassegno r */
    printf("r (const int *): *r=%d\n", *r);
}

int main(void) {
    /* Esercizio 1 */
    printf("== Esercizio 1 — scambia ==\n");
    int a = 1, b = 9;
    scambia(&a, &b);
    printf("dopo scambia: a=%d, b=%d (atteso a=9, b=1)\n", a, b);

    /* Esercizio 2 */
    printf("\n== Esercizio 2 — zero_array ==\n");
    int v[] = {1, 2, 3, 4, 5};
    zero_array(v, sizeof v / sizeof v[0]);
    printf("dopo zero_array: ");
    for (size_t i = 0; i < sizeof v / sizeof v[0]; ++i) {
        printf("%d ", v[i]);
    }
    printf("\n(atteso: 0 0 0 0 0)\n");

    /* Esercizio 3 */
    printf("\n== Esercizio 3 — cerca ==\n");
    int w[] = {10, 20, 30, 40, 50};
    int *trovato = cerca(w, sizeof w / sizeof w[0], 30);
    if (trovato) {
        printf("trovato 30 a indice %ld\n", (long)(trovato - w));
    } else {
        printf("30 non trovato (sospetto bug, doveva esserci)\n");
    }
    int *mancante = cerca(w, sizeof w / sizeof w[0], 999);
    printf("999 -> %s\n", mancante == NULL ? "NULL (corretto)" : "non NULL?!");

    /* Esercizio 4 */
    printf("\n== Esercizio 4 — conta_spazi ==\n");
    const char *s = "ciao a tutti voi";
    printf("'%s' -> %zu spazi (atteso 3)\n", s, conta_spazi(s));

    /* Esercizio 5 */
    esercizio_5();

    return 0;
}
