/*
 * Lezione 05 (medi) — File I/O.
 *
 * Questo esempio:
 *   1. scrive un file di testo
 *   2. lo rilegge riga per riga contando righe, parole, caratteri
 *   3. fa una copia binaria del file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int conta(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) { perror(path); return -1; }

    size_t righe = 0, parole = 0, caratteri = 0;
    int c, in_parola = 0;
    while ((c = fgetc(f)) != EOF) {
        ++caratteri;
        if (c == '\n') ++righe;
        if (isspace(c)) {
            in_parola = 0;
        } else if (!in_parola) {
            in_parola = 1;
            ++parole;
        }
    }
    fclose(f);
    printf("%-20s %5zu righe %5zu parole %5zu caratteri\n",
           path, righe, parole, caratteri);
    return 0;
}

static int copia_binaria(const char *src, const char *dst) {
    FILE *in  = fopen(src, "rb");
    if (!in) { perror(src); return -1; }

    FILE *out = fopen(dst, "wb");
    if (!out) { perror(dst); fclose(in); return -1; }

    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof buf, in)) > 0) {
        if (fwrite(buf, 1, n, out) != n) {
            perror("fwrite");
            fclose(in); fclose(out);
            return -1;
        }
    }

    if (ferror(in)) { perror("fread"); }

    fclose(in);
    if (fclose(out) != 0) { perror("fclose out"); return -1; }
    return 0;
}

int main(void) {
    const char *path = "/tmp/lez05_demo.txt";

    /* Scrittura */
    FILE *f = fopen(path, "w");
    if (!f) { perror(path); return 1; }
    fprintf(f, "Ciao Riccardo.\n");
    fprintf(f, "Questa e' una seconda riga.\n");
    fprintf(f, "E qui ce ne sono tre parole.\n");
    if (fclose(f) != 0) { perror("fclose"); return 1; }

    /* Lettura + conteggio */
    conta(path);

    /* Copia binaria */
    const char *copia = "/tmp/lez05_copia.txt";
    if (copia_binaria(path, copia) == 0) {
        printf("copia OK: %s -> %s\n", path, copia);
    }

    return 0;
}
