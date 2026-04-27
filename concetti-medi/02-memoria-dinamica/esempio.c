/*
 * Lezione 02 (medi) — Memoria dinamica.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int    *data;
    size_t  len;
    size_t  cap;
} Vector;

static void vec_init(Vector *v) {
    v->data = NULL;
    v->len  = 0;
    v->cap  = 0;
}

static bool vec_push(Vector *v, int x) {
    if (v->len == v->cap) {
        size_t ncap = v->cap ? v->cap * 2 : 4;
        int *tmp = realloc(v->data, ncap * sizeof *tmp);
        if (!tmp) return false;
        v->data = tmp;
        v->cap  = ncap;
    }
    v->data[v->len++] = x;
    return true;
}

static void vec_free(Vector *v) {
    free(v->data);
    v->data = NULL;
    v->len = v->cap = 0;
}

static int *duplica(const int *src, size_t n) {
    int *dst = malloc(n * sizeof *dst);
    if (!dst) return NULL;
    memcpy(dst, src, n * sizeof *dst);
    return dst;
}

int main(void) {
    /* malloc base */
    int *a = malloc(5 * sizeof *a);
    if (!a) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }
    for (int i = 0; i < 5; ++i) a[i] = i * i;

    int *b = duplica(a, 5);
    if (!b) {
        free(a);
        return 1;
    }

    printf("a:");
    for (int i = 0; i < 5; ++i) printf(" %d", a[i]);
    putchar('\n');

    printf("b:");
    for (int i = 0; i < 5; ++i) printf(" %d", b[i]);
    putchar('\n');

    free(a); a = NULL;
    free(b); b = NULL;

    /* Vector dinamico */
    Vector v;
    vec_init(&v);
    for (int i = 1; i <= 10; ++i) {
        if (!vec_push(&v, i * 10)) {
            fprintf(stderr, "push fallita\n");
            vec_free(&v);
            return 1;
        }
    }
    printf("vector (len=%zu, cap=%zu):", v.len, v.cap);
    for (size_t i = 0; i < v.len; ++i) printf(" %d", v.data[i]);
    putchar('\n');

    vec_free(&v);

    /* calloc: zero-initialized */
    int *zero = calloc(8, sizeof *zero);
    if (!zero) return 1;
    printf("calloc:");
    for (int i = 0; i < 8; ++i) printf(" %d", zero[i]);
    putchar('\n');
    free(zero);

    return 0;
}
