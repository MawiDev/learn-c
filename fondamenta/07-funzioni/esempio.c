/*
 * Lezione 07 — Funzioni.
 */

#include <stdio.h>
#include <stdbool.h>

/* Prototipi (dichiarazioni) */
static int      abs_int(int x);
static bool     e_primo(int n);
static void     scambia(int *a, int *b);
static int      contatore(void);

typedef struct { int q, r; } DivResult;
static DivResult dividi(int a, int b);

int main(void) {
    printf("|-5| = %d\n", abs_int(-5));

    printf("primi <= 20:");
    for (int i = 2; i <= 20; ++i) {
        if (e_primo(i)) printf(" %d", i);
    }
    putchar('\n');

    int x = 10, y = 3;
    scambia(&x, &y);
    printf("dopo scambia: x=%d y=%d\n", x, y);

    DivResult d = dividi(17, 5);
    printf("17 / 5 = %d resto %d\n", d.q, d.r);

    /* Variabile statica: persiste tra chiamate */
    printf("contatori: %d %d %d\n",
           contatore(), contatore(), contatore());

    return 0;
}

static int abs_int(int x) {
    return x < 0 ? -x : x;
}

static bool e_primo(int n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (int d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

static void scambia(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

static DivResult dividi(int a, int b) {
    DivResult r = { .q = a / b, .r = a % b };
    return r;
}

static int contatore(void) {
    static int n = 0;
    return ++n;
}
