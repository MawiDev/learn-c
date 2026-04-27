/*
 * Lezione 09 (medi) — Tipi moderni.
 *
 * Nota: clock_gettime richiede la feature test macro POSIX su Linux.
 * Usiamo timespec_get (C11, standard) per restare portabili.
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <time.h>
#include <math.h>

/* Verifica a compile-time */
_Static_assert(sizeof(int32_t) == 4, "int32_t deve essere 4 byte");
_Static_assert(sizeof(void*) >= 4, "puntatori ad almeno 32 bit");

/* _Generic: 'overload' a compile-time */
#define PRINT(x) _Generic((x),                 \
    int:         printf("int    : %d\n",  (x)),\
    long:        printf("long   : %ld\n", (x)),\
    double:      printf("double : %g\n",  (x)),\
    char *:      printf("string : %s\n",  (x)),\
    const char*: printf("cstring: %s\n",  (x)),\
    default:     printf("(sconosciuto)\n"))

/* static inline: funzione piccola in header style */
static inline int clamp(int x, int lo, int hi) {
    return x < lo ? lo : (x > hi ? hi : x);
}

/* offsetof */
typedef struct {
    char  a;
    int   b;
    double c;
} S;

int main(void) {
    /* Tipi a dimensione esatta */
    uint64_t nanosec = 1234567890123ULL;
    int32_t  delta   = -42;
    printf("nanosec = %" PRIu64 "\n", nanosec);
    printf("delta   = %" PRId32 "\n", delta);

    /* Generic dispatch */
    PRINT(42);
    PRINT(3.14);
    PRINT("ciao");
    long big = 100000L;
    PRINT(big);

    /* clamp */
    printf("clamp(150, 0, 100) = %d\n", clamp(150, 0, 100));
    printf("clamp(-5,  0, 100) = %d\n", clamp(-5, 0, 100));

    /* offsetof */
    printf("offset di S.b = %zu\n", offsetof(S, b));
    printf("offset di S.c = %zu\n", offsetof(S, c));

    /* Misura tempo con clock monotonico */
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    double acc = 0;
    for (int i = 1; i <= 10000000; ++i) {
        acc += 1.0 / i;
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);
    double elapsed = (double)(t1.tv_sec  - t0.tv_sec)
                   + (double)(t1.tv_nsec - t0.tv_nsec) / 1e9;
    printf("armonica(1e7) ~ %.6f, tempo %.3f s\n", acc, elapsed);

    /* fabs per silenziare "unused" su math */
    (void)fabs(0.0);

    return 0;
}
