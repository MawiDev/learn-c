/*
 * Lezione 08 — Esercizi: array.
 *
 * Compila con:
 *   gcc -std=c17 -Wall -Wextra -Wpedantic esercizi.c -o esercizi
 */

#include <stdio.h>
#include <stddef.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

/* ============================================================
 * Esercizio 1 — Array dei quadrati 0..81.
 * ============================================================
 *
 * Riempi un array int q[10] tale che q[i] = i*i. Stampalo.
 */
static void esercizio_1(void) {
    printf("== Esercizio 1 — quadrati ==\n");
    int q[10];
    (void)q;
    /* TODO: riempi q con i quadrati e stampalo. */
}

/* ============================================================
 * Esercizio 2 — somma(a, n).
 * ============================================================
 *
 * Ritorna la somma dei primi n elementi di a. Usa const per dichiarare che
 * non modifichi.
 */
static int somma(const int *a, size_t n) {
    (void)a; (void)n;
    /* TODO: somma in un loop, ritorna il totale. */
    return 0;
}

static void esercizio_2(void) {
    printf("\n== Esercizio 2 — somma ==\n");
    int a[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    printf("somma = %d (atteso 39)\n", somma(a, ARRAY_SIZE(a)));
}

/* ============================================================
 * Esercizio 3 — indice del massimo.
 * ============================================================
 *
 * Ritorna l'indice dell'elemento massimo. In caso di parita', ritorna il
 * primo. Per n == 0 ritorna 0 (o gestiscilo a piacere).
 */
static size_t indice_max(const int *a, size_t n) {
    (void)a; (void)n;
    /* TODO: scorri e tieni traccia dell'indice del massimo. */
    return 0;
}

static void esercizio_3(void) {
    printf("\n== Esercizio 3 — indice_max ==\n");
    int a[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    size_t i = indice_max(a, ARRAY_SIZE(a));
    printf("max = %d a indice %zu (atteso 9 a indice 5)\n", a[i], i);
}

/* ============================================================
 * Esercizio 4 — Inverti un array in-place.
 * ============================================================
 *
 * Niente array d'appoggio. Scorri due indici dai due capi verso il centro
 * e scambia. Fermati a metà.
 */
static void inverti(int *a, size_t n) {
    (void)a; (void)n;
    /* TODO: scambia a[0]<->a[n-1], a[1]<->a[n-2], ... */
}

static void esercizio_4(void) {
    printf("\n== Esercizio 4 — inverti ==\n");
    int a[] = {1, 2, 3, 4, 5};
    inverti(a, ARRAY_SIZE(a));
    printf("dopo inverti: ");
    for (size_t i = 0; i < ARRAY_SIZE(a); ++i) {
        printf("%d ", a[i]);
    }
    printf("\n(atteso: 5 4 3 2 1)\n");
}

/* ============================================================
 * Esercizio 5 — Matrice 5x5 stampata a tabella.
 * ============================================================
 *
 * Crea una matrice 5x5 in cui m[i][j] = (i+1) * (j+1) e stampala con
 * colonne allineate (%4d).
 */
static void esercizio_5(void) {
    printf("\n== Esercizio 5 — matrice 5x5 ==\n");
    /* TODO: dichiara int m[5][5], riempila, stampala con due cicli annidati. */
}

int main(void) {
    esercizio_1();
    esercizio_2();
    esercizio_3();
    esercizio_4();
    esercizio_5();
    return 0;
}
