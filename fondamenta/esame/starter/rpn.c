/*
 * Esame Fondamenta — Calcolatrice RPN.
 *
 * STARTER: completa le parti marcate con TODO.
 *
 * Compilazione:
 *   gcc -std=c17 -Wall -Wextra -Wpedantic -Werror rpn.c -o rpn
 *   gcc -std=c17 -g -O1 -fsanitize=address,undefined rpn.c -o rpn
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>

#define STACK_MAX 64
#define LINE_MAX  1024

/* ---------- Stack ---------- */

/* TODO: implementa queste funzioni.
 * Regola: nessuna di queste funzioni stampa su stderr;
 * in caso di errore ritorna false e il chiamante decidera' cosa fare.
 */
static bool push(double *stack, size_t *sp, double v);
static bool pop (double *stack, size_t *sp, double *out);
static bool peek2(const double *stack, size_t sp, double *a, double *b);

/* ---------- Tokenizer ---------- */

/*
 * Trova il prossimo token in *p partendo dalla posizione corrente.
 * Avanza *p oltre il token. Scrive in buf (dim. bufcap) il token come stringa
 * terminata, e ritorna true. Se non ci sono piu' token ritorna false.
 * Ignora whitespace iniziale. Tratta '#' come inizio commento: tutto il resto
 * della riga viene ignorato.
 *
 * TODO: implementa.
 */
static bool prossimo_token(const char **p, char *buf, size_t bufcap);

/* ---------- Parse number ---------- */

/*
 * Prova a parsare 'token' come numero double. Accetta TUTTO il token
 * (non ammette spazzatura in coda). Ritorna true su successo.
 *
 * TODO: implementa usando strtod.
 */
static bool parse_double(const char *token, double *out);

/* ---------- Operazioni ---------- */

/*
 * Applica un operatore binario. op e' uno di '+', '-', '*', '/', '%'.
 * Ritorna true su successo; false se stack underflow o divisione per zero.
 * In caso di errore, lo stack DEVE restare invariato.
 *
 * TODO: implementa.
 */
static bool applica_op(double *stack, size_t *sp, char op, const char **errmsg);

/*
 * Modulo su double, SENZA usare <math.h>.
 * TODO: implementa. Definizione: a - b * trunc(a/b)
 * Attenzione ai segni e al caso b == 0 (non gestirlo qui, lo fa applica_op).
 */
static double mod_d(double a, double b);

/* ---------- Comandi ---------- */

static void stampa_stack(const double *stack, size_t sp);
static void stampa_aiuto(void);

/* ---------- Main loop ---------- */

/*
 * Processa un singolo token gia' estratto dalla tokenizzazione.
 * Ritorna:
 *    0  ok
 *   -1  errore (riga abortita ma non il programma)
 *    1  richiesta di uscita (token 'quit')
 * In caso di errore, stampa su stderr.
 *
 * TODO: implementa.
 */
static int esegui_token(const char *tok,
                        double *stack, size_t *sp);

int main(void) {
    double stack[STACK_MAX];
    size_t sp = 0;

    char line[LINE_MAX];

    while (fgets(line, sizeof line, stdin) != NULL) {
        const char *p = line;
        bool aborta_riga = false;
        bool esci = false;

        char tok[64];
        while (prossimo_token(&p, tok, sizeof tok)) {
            int rc = esegui_token(tok, stack, &sp);
            if (rc == 1) { esci = true; break; }
            if (rc < 0)  { aborta_riga = true; break; }
        }

        if (esci) break;
        (void)aborta_riga;   /* lo stato dello stack e' quello che e' */
        stampa_stack(stack, sp);
    }

    return 0;
}

/* =============================================================== */
/* STUB: sostituisci con le tue implementazioni                    */
/* =============================================================== */

static bool push(double *stack, size_t *sp, double v) {
    (void)stack; (void)sp; (void)v;
    /* TODO */
    return false;
}

static bool pop(double *stack, size_t *sp, double *out) {
    (void)stack; (void)sp; (void)out;
    /* TODO */
    return false;
}

static bool peek2(const double *stack, size_t sp, double *a, double *b) {
    (void)stack; (void)sp; (void)a; (void)b;
    /* TODO */
    return false;
}

static bool prossimo_token(const char **p, char *buf, size_t bufcap) {
    (void)p; (void)buf; (void)bufcap;
    /* TODO */
    return false;
}

static bool parse_double(const char *token, double *out) {
    (void)token; (void)out;
    /* TODO */
    return false;
}

static double mod_d(double a, double b) {
    (void)a; (void)b;
    /* TODO */
    return 0.0;
}

static bool applica_op(double *stack, size_t *sp, char op, const char **errmsg) {
    (void)stack; (void)sp; (void)op; (void)errmsg;
    /* TODO */
    return false;
}

static void stampa_stack(const double *stack, size_t sp) {
    printf("[");
    for (size_t i = 0; i < sp; ++i) {
        printf(" %g", stack[i]);
    }
    printf(" ]\n");
}

static void stampa_aiuto(void) {
    puts("operatori: + - * / %");
    puts("comandi  : dup drop swap clear print help quit");
    puts("# avvia un commento fino a fine riga");
}

static int esegui_token(const char *tok,
                        double *stack, size_t *sp) {
    (void)tok; (void)stack; (void)sp;
    /* TODO: riconosci numero / operatore / comando e dispatcha */
    stampa_aiuto();  /* placeholder */
    return 0;
}
