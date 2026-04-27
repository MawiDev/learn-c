/*
 * Lezione 07 (medi) — Puntatori a funzione.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*BinOp)(int, int);

static int somma(int a, int b) { return a + b; }
static int diff (int a, int b) { return a - b; }
static int prod (int a, int b) { return a * b; }

static int cmp_int(const void *a, const void *b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

static int cmp_str(const void *a, const void *b) {
    const char *sa = *(const char *const*)a;
    const char *sb = *(const char *const*)b;
    return strcmp(sa, sb);
}

typedef void (*Callback)(void *ctx, int *x);

static void for_each(int *a, size_t n, Callback fn, void *ctx) {
    for (size_t i = 0; i < n; ++i) fn(ctx, &a[i]);
}

static void incr_di(void *ctx, int *x) {
    int d = *(int *)ctx;
    *x += d;
}

int main(void) {
    BinOp ops[] = { somma, diff, prod };
    const char *nomi[] = { "somma", "diff", "prod" };
    for (size_t i = 0; i < 3; ++i) {
        printf("%s(6,4) = %d\n", nomi[i], ops[i](6, 4));
    }

    /* qsort su int */
    int a[] = {5, 2, 8, 1, 9, 4};
    qsort(a, 6, sizeof a[0], cmp_int);
    printf("int ordinati:");
    for (size_t i = 0; i < 6; ++i) printf(" %d", a[i]);
    putchar('\n');

    /* qsort su stringhe */
    const char *frutti[] = { "pera", "mela", "banana", "arancia" };
    qsort(frutti, 4, sizeof frutti[0], cmp_str);
    printf("frutti ordinati:");
    for (size_t i = 0; i < 4; ++i) printf(" %s", frutti[i]);
    putchar('\n');

    /* Callback con contesto */
    int delta = 10;
    int vals[] = {1, 2, 3, 4, 5};
    for_each(vals, 5, incr_di, &delta);
    printf("dopo +%d:", delta);
    for (size_t i = 0; i < 5; ++i) printf(" %d", vals[i]);
    putchar('\n');

    return 0;
}
