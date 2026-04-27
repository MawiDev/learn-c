/*
 * kvdb — Key-Value store persistente (soluzione esame Concetti Medi).
 */

#include "common.h"
#include "log.h"
#include "store.h"
#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINE_MAX_SZ 4096

/* Separa "comando args..." modificando la riga in-place.
 * Ritorna puntatore al nome, e tramite *out_args il puntatore al resto
 * (sempre non-NULL; se vuoto, punta a '\0'). */
static char *split_comando(char *linea, char **out_args) {
    /* skip whitespace iniziale */
    while (*linea && isspace((unsigned char)*linea)) ++linea;
    char *nome = linea;

    /* scorri il nome */
    while (*linea && !isspace((unsigned char)*linea)) ++linea;

    if (*linea == '\0') {
        *out_args = linea;   /* stringa vuota */
        return nome;
    }

    *linea++ = '\0';

    /* skip whitespace prima degli args */
    while (*linea && isspace((unsigned char)*linea)) ++linea;
    *out_args = linea;
    return nome;
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    log_set_level(LOG_WARN);   /* LOG_DEBUG per vedere tutto */

    KVStore store;
    store_init(&store);

    char linea[LINE_MAX_SZ];
    while (fgets(linea, sizeof linea, stdin) != NULL) {
        size_t n = strlen(linea);
        if (n > 0 && linea[n - 1] == '\n') linea[n - 1] = '\0';

        const char *probe = linea;
        while (*probe && isspace((unsigned char)*probe)) ++probe;
        if (*probe == '\0' || *probe == '#') continue;

        char *args = NULL;
        char *nome = split_comando(linea, &args);

        CmdResult rc = commands_dispatch(&store, nome, args);
        if (rc == CMD_QUIT) break;
    }

    store_free(&store);
    return 0;
}
