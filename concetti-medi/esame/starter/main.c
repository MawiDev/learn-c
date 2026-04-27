/*
 * kvdb — Key-Value store persistente (esame Concetti Medi).
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

/* TODO: implementa uno splitter di riga che produca (nome_comando, argomenti).
 * Esempio:
 *   "set nome s Riccardo Savio"  ->  nome="set", args="nome s Riccardo Savio"
 * Modifica la riga in-place inserendo '\0' al primo whitespace, e ritorna
 * un puntatore al resto (trimmato a sinistra).
 */
static char *split_comando(char *linea, char **out_nome);

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    log_set_level(LOG_INFO);

    KVStore store;
    store_init(&store);

    char linea[LINE_MAX_SZ];
    while (fgets(linea, sizeof linea, stdin) != NULL) {
        /* rimuovi newline finale */
        size_t n = strlen(linea);
        if (n > 0 && linea[n - 1] == '\n') linea[n - 1] = '\0';

        /* salta righe vuote */
        char *p = linea;
        while (*p && isspace((unsigned char)*p)) ++p;
        if (*p == '\0' || *p == '#') continue;

        char *nome = NULL;
        char *args = split_comando(p, &nome);

        CmdResult rc = commands_dispatch(&store, nome, args);
        if (rc == CMD_QUIT) break;
    }

    store_free(&store);
    return 0;
}

static char *split_comando(char *linea, char **out_nome) {
    /* TODO */
    *out_nome = linea;
    return "";
}
