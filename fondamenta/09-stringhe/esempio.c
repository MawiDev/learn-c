/*
 * Lezione 09 — Stringhe.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

static size_t mia_strlen(const char *s) {
    const char *p = s;
    while (*p) ++p;
    return (size_t)(p - s);
}

static void a_maiuscolo(char *s) {
    for (; *s; ++s) {
        *s = (char)toupper((unsigned char)*s);
    }
}

static int conta_vocali(const char *s) {
    int n = 0;
    for (; *s; ++s) {
        char c = (char)tolower((unsigned char)*s);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') ++n;
    }
    return n;
}

static bool palindroma(const char *s) {
    size_t i = 0, j = strlen(s);
    while (i < j) {
        --j;
        char ci = (char)tolower((unsigned char)s[i]);
        char cj = (char)tolower((unsigned char)s[j]);
        if (ci != cj) return false;
        ++i;
    }
    return true;
}

int main(void) {
    const char *nome = "Riccardo";
    printf("mia_strlen('%s') = %zu\n", nome, mia_strlen(nome));

    char copia[32];
    /* copia sicura con snprintf */
    int scritti = snprintf(copia, sizeof copia, "%s", nome);
    if (scritti < 0 || (size_t)scritti >= sizeof copia) {
        fprintf(stderr, "troncamento!\n");
    }
    a_maiuscolo(copia);
    printf("maiuscolo    : %s\n", copia);

    printf("vocali in '%s' = %d\n", nome, conta_vocali(nome));

    const char *parole[] = { "Anna", "ciao", "aIbohphobiA", "ouch" };
    for (size_t i = 0; i < sizeof parole / sizeof parole[0]; ++i) {
        printf("'%s' palindroma? %s\n",
               parole[i], palindroma(parole[i]) ? "si" : "no");
    }

    /* Costruzione sicura di una stringa */
    char linea[64];
    int n = snprintf(linea, sizeof linea, "ID=%04d, nome=%s", 42, nome);
    if (n < 0) {
        fprintf(stderr, "errore snprintf\n");
    } else if ((size_t)n >= sizeof linea) {
        fprintf(stderr, "stringa troncata (servivano %d byte)\n", n + 1);
    }
    printf("linea: %s\n", linea);

    /* strcmp */
    const char *a = "mela", *b = "pera";
    int r = strcmp(a, b);
    printf("strcmp('%s','%s') = %d (%s)\n",
           a, b, r, r < 0 ? "a < b" : r > 0 ? "a > b" : "uguali");

    return 0;
}
