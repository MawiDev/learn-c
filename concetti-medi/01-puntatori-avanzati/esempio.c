/*
 * Lezione 01 (medi) — Puntatori avanzati.
 *
 * Compila con sanitizer per debug:
 *   gcc -std=c17 -g -O1 -fsanitize=address,undefined -Wall -Wextra esempio.c -o esempio
 */

#include <stdio.h>
#include <string.h>
#include <stddef.h>

static char *mia_strcpy(char *restrict dst, const char *restrict src) {
    char *ret = dst;
    while ((*dst++ = *src++) != '\0') { /* copia */ }
    return ret;
}

static void rimuovi_elemento(int *a, size_t *n, size_t idx) {
    if (idx >= *n) return;
    for (size_t i = idx + 1; i < *n; ++i) {
        a[i - 1] = a[i];
    }
    --(*n);
}

static size_t conta_char(const char *s, char c) {
    size_t n = 0;
    for (const char *p = s; *p; ++p) {
        if (*p == c) ++n;
    }
    return n;
}

int main(int argc, char **argv) {
    printf("-- argv --\n");
    for (int i = 0; i < argc; ++i) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }

    char buf[16];
    mia_strcpy(buf, "ciao mondo");
    printf("\ncopiato: %s\n", buf);
    printf("spazi  : %zu\n", conta_char(buf, ' '));

    int arr[] = {10, 20, 30, 40, 50};
    size_t n = sizeof arr / sizeof arr[0];
    rimuovi_elemento(arr, &n, 2);
    printf("dopo rimozione indice 2 (n=%zu):", n);
    for (size_t i = 0; i < n; ++i) printf(" %d", arr[i]);
    putchar('\n');

    /* Puntatore a puntatore */
    int x = 5;
    int *p = &x;
    int **pp = &p;
    **pp = 42;
    printf("x via **pp = %d\n", x);

    /* ptrdiff_t: differenza tra puntatori */
    const char *s = "Riccardo";
    const char *dot = strchr(s, 'c');
    if (dot != NULL) {
        ptrdiff_t pos = dot - s;
        printf("prima 'c' a offset %td\n", pos);
    }

    return 0;
}
