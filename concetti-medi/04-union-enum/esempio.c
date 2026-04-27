/*
 * Lezione 04 (medi) — union ed enum.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    LUN, MAR, MER, GIO, VEN, SAB, DOM
} Giorno;

static const char *nome_giorno(Giorno g) {
    static const char *nomi[] = {
        "Lunedi", "Martedi", "Mercoledi", "Giovedi",
        "Venerdi", "Sabato", "Domenica"
    };
    return nomi[g];
}

typedef enum { T_INT, T_DOUBLE, T_STRING } Tipo;

typedef struct {
    Tipo tipo;
    union {
        int         i;
        double      d;
        const char *s;
    };
} Valore;

static void stampa(const Valore *v) {
    switch (v->tipo) {
        case T_INT:    printf("int(%d)\n", v->i);    break;
        case T_DOUBLE: printf("double(%g)\n", v->d); break;
        case T_STRING: printf("string(\"%s\")\n", v->s); break;
    }
}

static bool uguali(const Valore *a, const Valore *b) {
    if (a->tipo != b->tipo) return false;
    switch (a->tipo) {
        case T_INT:    return a->i == b->i;
        case T_DOUBLE: return a->d == b->d;
        case T_STRING: return strcmp(a->s, b->s) == 0;
    }
    return false;
}

int main(void) {
    for (Giorno g = LUN; g <= DOM; ++g) {
        printf("%d -> %s\n", g, nome_giorno(g));
    }

    Valore vals[] = {
        { .tipo = T_INT,    .i = 42 },
        { .tipo = T_DOUBLE, .d = 3.14 },
        { .tipo = T_STRING, .s = "ciao" },
    };
    for (size_t i = 0; i < sizeof vals / sizeof vals[0]; ++i) {
        stampa(&vals[i]);
    }

    printf("a == b ? %s\n", uguali(&vals[0], &vals[0]) ? "si" : "no");
    printf("a == c ? %s\n", uguali(&vals[0], &vals[2]) ? "si" : "no");

    /* Type-punning sicuro via memcpy */
    uint32_t bits = 0x40490FDB;   /* ~ pi in float */
    float f;
    memcpy(&f, &bits, sizeof f);
    printf("bits 0x%08X come float = %f\n", bits, (double)f);

    return 0;
}
