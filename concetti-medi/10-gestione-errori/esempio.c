/*
 * Lezione 10 (medi) — Gestione degli errori.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <limits.h>

/* ---------- Mini logger ---------- */

typedef enum { LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR } LogLevel;
static LogLevel log_soglia = LOG_INFO;

static const char *log_nome(LogLevel l) {
    switch (l) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO:  return "INFO";
        case LOG_WARN:  return "WARN";
        case LOG_ERROR: return "ERROR";
    }
    return "?";
}

static void log_msg(LogLevel l, const char *fmt, ...) {
    if (l < log_soglia) return;
    fprintf(stderr, "[%-5s] ", log_nome(l));
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);
}

/* ---------- Parsing robusto ---------- */

/* Ritorna 0 su successo, -1 altrimenti. out non viene modificato in caso di errore. */
static int parse_int_stretto(const char *s, int *out) {
    if (!s || !out) { errno = EINVAL; return -1; }
    if (*s == '\0') { errno = EINVAL; return -1; }

    char *end = NULL;
    errno = 0;
    long v = strtol(s, &end, 10);

    if (errno != 0)            return -1;   /* overflow/underflow */
    if (end == s)              { errno = EINVAL; return -1; } /* nessuna cifra */
    if (*end != '\0')          { errno = EINVAL; return -1; } /* spazzatura in coda */
    if (v < INT_MIN || v > INT_MAX) { errno = ERANGE; return -1; }

    *out = (int)v;
    return 0;
}

/* ---------- Cleanup single-exit ---------- */

static int copia_duplica(const char *src, const char *dst) {
    int   rc  = -1;
    FILE *in  = NULL;
    FILE *out = NULL;
    char *buf = NULL;

    in = fopen(src, "rb");
    if (!in) {
        log_msg(LOG_ERROR, "fopen(%s): %s", src, strerror(errno));
        goto cleanup;
    }

    out = fopen(dst, "wb");
    if (!out) {
        log_msg(LOG_ERROR, "fopen(%s): %s", dst, strerror(errno));
        goto cleanup;
    }

    buf = malloc(4096);
    if (!buf) {
        log_msg(LOG_ERROR, "malloc fallita");
        goto cleanup;
    }

    size_t n;
    while ((n = fread(buf, 1, 4096, in)) > 0) {
        if (fwrite(buf, 1, n, out) != n) {
            log_msg(LOG_ERROR, "fwrite: %s", strerror(errno));
            goto cleanup;
        }
    }

    rc = 0;

cleanup:
    free(buf);
    if (in)  fclose(in);
    if (out) fclose(out);
    return rc;
}

int main(void) {
    log_soglia = LOG_DEBUG;  /* mostra tutto */

    int v;
    const char *prove[] = { "42", "  42", "42abc", "", "99999999999999", "-10" };
    for (size_t i = 0; i < sizeof prove / sizeof prove[0]; ++i) {
        if (parse_int_stretto(prove[i], &v) == 0) {
            log_msg(LOG_INFO, "'%s' -> %d", prove[i], v);
        } else {
            log_msg(LOG_WARN, "'%s' rifiutato (%s)", prove[i], strerror(errno));
        }
    }

    /* Invariante di programma tramite assert */
    int x = 5;
    assert(x > 0);
    (void)x;

    /* Copia via cleanup single-exit */
    const char *src = "/tmp/lez10_src.txt";
    FILE *s = fopen(src, "w");
    if (s) {
        fputs("Ciao Riccardo!\n", s);
        fclose(s);
    }
    const char *dst = "/tmp/lez10_dst.txt";
    if (copia_duplica(src, dst) == 0) {
        log_msg(LOG_INFO, "copia %s -> %s riuscita", src, dst);
    }

    return 0;
}
