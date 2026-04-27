/*
 * Esame Fondamenta — Soluzione di riferimento: Calcolatrice RPN.
 *
 * Compilazione:
 *   gcc -std=c17 -Wall -Wextra -Wpedantic -Werror rpn.c -o rpn
 *   gcc -std=c17 -g -O1 -fsanitize=address,undefined rpn.c -o rpn
 *
 * Esempio d'uso:
 *   echo "3 4 + 2 *" | ./rpn
 *   -> [ 14 ]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>

#define STACK_MAX 64
#define LINE_MAX  1024
#define TOKEN_MAX 64

/* ============================================================= */
/*  Stack: operazioni primitive, nessun side effect di stampa.   */
/* ============================================================= */

static bool push(double *stack, size_t *sp, double v) {
    if (*sp >= STACK_MAX) return false;
    stack[(*sp)++] = v;
    return true;
}

static bool pop(double *stack, size_t *sp, double *out) {
    if (*sp == 0) return false;
    *out = stack[--(*sp)];
    return true;
}

/* Legge i due elementi in cima senza modificare lo stack.
 * *a finisce con il penultimo, *b con l'ultimo (quello sopra). */
static bool peek2(const double *stack, size_t sp, double *a, double *b) {
    if (sp < 2) return false;
    *a = stack[sp - 2];
    *b = stack[sp - 1];
    return true;
}

/* ============================================================= */
/*  Tokenizer manuale, scorrendo la riga con un puntatore.       */
/* ============================================================= */

static bool prossimo_token(const char **p, char *buf, size_t bufcap) {
    const char *s = *p;

    while (*s != '\0' && isspace((unsigned char)*s)) ++s;

    if (*s == '\0' || *s == '#' || *s == '\n') {
        *p = s;
        return false;
    }

    size_t i = 0;
    while (*s != '\0' && !isspace((unsigned char)*s) && *s != '#') {
        if (i + 1 >= bufcap) {
            /* token troppo lungo: lo tronchiamo ma marchiamo con \0 */
            break;
        }
        buf[i++] = *s++;
    }
    buf[i] = '\0';
    *p = s;
    return i > 0;
}

/* ============================================================= */
/*  Parsing numerico: accetta solo se strtod consuma TUTTO.      */
/* ============================================================= */

static bool parse_double(const char *token, double *out) {
    if (*token == '\0') return false;
    char *end = NULL;
    double v = strtod(token, &end);
    if (end == token) return false;
    while (*end != '\0') {
        if (!isspace((unsigned char)*end)) return false;
        ++end;
    }
    *out = v;
    return true;
}

/* ============================================================= */
/*  Modulo su double senza <math.h>.                              */
/* ============================================================= */

static double mod_d(double a, double b) {
    /* quoziente troncato verso lo zero */
    double q = a / b;
    long long qi = (long long)q;   /* truncation, gestisce gia' i segni */
    return a - (double)qi * b;
}

/* ============================================================= */
/*  Applicazione operatore binario. Non modifica lo stack se     */
/*  fallisce.                                                    */
/* ============================================================= */

static bool applica_op(double *stack, size_t *sp, char op, const char **errmsg) {
    double a, b;
    if (!peek2(stack, *sp, &a, &b)) {
        *errmsg = "stack underflow";
        return false;
    }

    double r;
    switch (op) {
        case '+': r = a + b; break;
        case '-': r = a - b; break;
        case '*': r = a * b; break;
        case '/':
            if (b == 0.0) { *errmsg = "divisione per zero"; return false; }
            r = a / b;
            break;
        case '%':
            if (b == 0.0) { *errmsg = "divisione per zero"; return false; }
            r = mod_d(a, b);
            break;
        default:
            *errmsg = "operatore non riconosciuto";
            return false;
    }

    /* A questo punto i calcoli sono andati, applichiamo le modifiche. */
    *sp -= 2;
    stack[(*sp)++] = r;
    return true;
}

/* ============================================================= */
/*  Output e aiuto.                                              */
/* ============================================================= */

static void stampa_stack(const double *stack, size_t sp) {
    printf("[");
    for (size_t i = 0; i < sp; ++i) {
        printf(" %g", stack[i]);
    }
    printf(" ]\n");
}

static void stampa_aiuto(void) {
    puts("  operatori: + - * / %");
    puts("  comandi  : dup drop swap clear print help quit");
    puts("  numeri   : notazione C (es. 3.14, -2, 1e-3)");
    puts("  '#' inizia un commento fino a fine riga");
}

/* ============================================================= */
/*  Esecuzione di un singolo token.                               */
/*  Ritorna: 0 ok, -1 errore, 1 quit.                            */
/* ============================================================= */

static int esegui_token(const char *tok, double *stack, size_t *sp) {
    /* Operatori (token di un carattere) */
    if (tok[1] == '\0' && strchr("+-*/%", tok[0])) {
        /* Attenzione: '-2' viene gestito prima come numero, quindi
         * qui entra solo se il token e' letteralmente un singolo '-'. */
        const char *err = NULL;
        if (!applica_op(stack, sp, tok[0], &err)) {
            fprintf(stderr, "errore: %s (operatore '%c')\n", err, tok[0]);
            return -1;
        }
        return 0;
    }

    /* Comandi */
    if (strcmp(tok, "dup") == 0) {
        double v;
        if (*sp == 0) { fprintf(stderr, "errore: dup su stack vuoto\n"); return -1; }
        v = stack[*sp - 1];
        if (!push(stack, sp, v)) { fprintf(stderr, "errore: stack overflow\n"); return -1; }
        return 0;
    }
    if (strcmp(tok, "drop") == 0) {
        double v;
        if (!pop(stack, sp, &v)) { fprintf(stderr, "errore: drop su stack vuoto\n"); return -1; }
        return 0;
    }
    if (strcmp(tok, "swap") == 0) {
        if (*sp < 2) { fprintf(stderr, "errore: swap richiede 2 elementi\n"); return -1; }
        double t = stack[*sp - 1];
        stack[*sp - 1] = stack[*sp - 2];
        stack[*sp - 2] = t;
        return 0;
    }
    if (strcmp(tok, "clear") == 0) { *sp = 0; return 0; }
    if (strcmp(tok, "print") == 0) { stampa_stack(stack, *sp); return 0; }
    if (strcmp(tok, "help")  == 0) { stampa_aiuto(); return 0; }
    if (strcmp(tok, "quit")  == 0) { return 1; }

    /* Numero? */
    double v;
    if (parse_double(tok, &v)) {
        if (!push(stack, sp, v)) {
            fprintf(stderr, "errore: stack overflow (push di %g)\n", v);
            return -1;
        }
        return 0;
    }

    fprintf(stderr, "errore: token sconosciuto '%s'\n", tok);
    return -1;
}

/* ============================================================= */
/*  Main: read-eval-print loop su righe.                         */
/* ============================================================= */

int main(void) {
    double stack[STACK_MAX];
    size_t sp = 0;
    char   line[LINE_MAX];

    while (fgets(line, sizeof line, stdin) != NULL) {
        const char *p = line;
        bool esci = false;
        bool aborta = false;

        char tok[TOKEN_MAX];
        while (prossimo_token(&p, tok, sizeof tok)) {
            int rc = esegui_token(tok, stack, &sp);
            if (rc == 1) { esci = true; break; }
            if (rc < 0)  { aborta = true; break; }
        }

        if (esci) break;
        (void)aborta;   /* lo stack riflette comunque lo stato reale */
        stampa_stack(stack, sp);
    }

    return 0;
}
