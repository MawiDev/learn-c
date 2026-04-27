#include "matematica.h"

/* funzione di utilita' privata: static = solo per questo file */
static int iterativo_fatt(int n) {
    int r = 1;
    for (int i = 2; i <= n; ++i) r *= i;
    return r;
}

int somma(int a, int b)      { return a + b; }
int moltiplica(int a, int b) { return a * b; }
int fattoriale(int n)        { return iterativo_fatt(n); }
