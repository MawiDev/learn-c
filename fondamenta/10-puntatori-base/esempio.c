/*
 * Lezione 10 — Puntatori (le basi).
 */

#include <stdio.h>
#include <stddef.h>

static void scambia(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

static void zero_array(int *a, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        *(a + i) = 0;
    }
}

static int *cerca(int *a, size_t n, int target) {
    for (size_t i = 0; i < n; ++i) {
        if (a[i] == target) return &a[i];
    }
    return NULL;
}

int main(void) {
    int x = 42;
    int *p = &x;

    printf("x    = %d\n", x);
    printf("&x   = %p\n", (void*)&x);
    printf("p    = %p\n", (void*)p);
    printf("*p   = %d\n", *p);

    *p = 100;
    printf("dopo *p = 100 -> x = %d\n", x);

    int a = 1, b = 2;
    scambia(&a, &b);
    printf("scambia: a=%d b=%d\n", a, b);

    int arr[5] = {10, 20, 30, 40, 50};
    int *res = cerca(arr, 5, 30);
    if (res != NULL) {
        printf("trovato 30 all'indice %td\n", res - arr);
    }

    /* Const pointer: dimostrazione */
    int n = 7;
    int *const q = &n;     /* q non puo' puntare altrove, ma *q si' */
    *q = 99;
    printf("n via const ptr = %d\n", n);
    /* q = &a; -> errore: assignment of read-only variable 'q' */

    /* Pointer to const: dimostrazione */
    const int *r = &n;
    /* *r = 0;  -> errore: assignment of read-only location */
    r = &a;  /* lui puo' spostarsi */
    (void)r;

    zero_array(arr, 5);
    printf("dopo zero: %d %d %d %d %d\n",
           arr[0], arr[1], arr[2], arr[3], arr[4]);

    return 0;
}
