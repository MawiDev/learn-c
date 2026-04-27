/*
 * Lezione 06 (medi) — Preprocessore e macro.
 *
 * Prova anche:
 *   gcc -DDEBUG -std=c17 -Wall -Wextra -Wpedantic esempio.c -o esempio
 */

#include <stdio.h>
#include <assert.h>

/* Static assertion: fallisce a tempo di compilazione se la condizione e' falsa */
_Static_assert(sizeof(int) >= 4, "int deve essere almeno 32 bit");

/* Macro ben parentesizzata */
#define SQUARE(x) ((x) * (x))

/* Macro mal parentesizzata, solo per dimostrazione */
#define BAD_SQUARE(x) x * x

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* Variadic + file:line:func (nota lo spazio prima di '##' non richiesto in C23) */
#define LOG(fmt, ...) \
    fprintf(stderr, "%s:%d [%s] " fmt "\n", \
            __FILE__, __LINE__, __func__, __VA_ARGS__)

#if defined(DEBUG) && DEBUG
    #define DBG(fmt, ...) LOG(fmt, __VA_ARGS__)
#else
    #define DBG(fmt, ...) ((void)0)
#endif

int main(void) {
    int a[10];
    printf("ARRAY_SIZE = %zu\n", ARRAY_SIZE(a));

    printf("SQUARE(1+2)     = %d\n", SQUARE(1 + 2));        /* 9 */
    printf("BAD_SQUARE(1+2) = %d\n", BAD_SQUARE(1 + 2));    /* 5 !!! */

    int x = 3, y = 7;
    printf("MIN(%d, %d) = %d\n", x, y, MIN(x, y));

    LOG("valore di x = %d", x);
    DBG("messaggio di debug visibile solo con -DDEBUG (x=%d)", x);

    /* Stringify e token-pasting */
    #define STR(x) #x
    #define CAT(a, b) a##b
    printf("STR(ciao)    = %s\n", STR(ciao));
    int CAT(var, 42) = 100;
    printf("var42        = %d\n", var42);

    /* Macro predefinite */
    printf("STDC_VERSION = %ldL\n", __STDC_VERSION__);
    printf("file = %s, line = %d, func = %s\n",
           __FILE__, __LINE__, __func__);

    return 0;
}
