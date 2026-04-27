/*
 * Lezione 08 — Array.
 */

#include <stdio.h>
#include <stddef.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void stampa(const int *a, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        printf("%d%s", a[i], (i + 1 < n) ? " " : "\n");
    }
}

static int somma(const int *a, size_t n) {
    int s = 0;
    for (size_t i = 0; i < n; ++i) s += a[i];
    return s;
}

static size_t indice_max(const int *a, size_t n) {
    size_t m = 0;
    for (size_t i = 1; i < n; ++i) {
        if (a[i] > a[m]) m = i;
    }
    return m;
}

static void inverti(int *a, size_t n) {
    for (size_t i = 0, j = n; i < j; ++i) {
        --j;
        int t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
}

int main(void) {
    int quadrati[10];
    for (size_t i = 0; i < ARRAY_SIZE(quadrati); ++i) {
        quadrati[i] = (int)(i * i);
    }

    printf("quadrati: ");
    stampa(quadrati, ARRAY_SIZE(quadrati));
    printf("somma: %d\n", somma(quadrati, ARRAY_SIZE(quadrati)));
    printf("indice max: %zu (valore %d)\n",
           indice_max(quadrati, ARRAY_SIZE(quadrati)),
           quadrati[indice_max(quadrati, ARRAY_SIZE(quadrati))]);

    inverti(quadrati, ARRAY_SIZE(quadrati));
    printf("invertiti: ");
    stampa(quadrati, ARRAY_SIZE(quadrati));

    /* Designated initializer */
    int priorita[10] = { [3] = 7, [7] = 9 };
    printf("priorita : ");
    stampa(priorita, ARRAY_SIZE(priorita));

    /* Matrice 3x4 */
    int m[3][4] = {
        { 1,  2,  3,  4},
        { 5,  6,  7,  8},
        { 9, 10, 11, 12}
    };
    printf("\nmatrice 3x4:\n");
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            printf("%3d ", m[i][j]);
        }
        putchar('\n');
    }

    return 0;
}
