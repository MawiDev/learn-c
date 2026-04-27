#include "commands.h"
#include "store.h"
#include "format.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>

/* ---------- Handler ---------- */

/* TODO: implementa tutti gli handler.
 * Tutti rispettano la firma CmdHandler.
 * Regola: stampa su stdout i risultati "di dominio" (OK, valori, list, ecc.).
 * Usa LOG_E/LOG_W per diagnostica. */

static CmdResult cmd_set (KVStore *s, const char *args);
static CmdResult cmd_get (KVStore *s, const char *args);
static CmdResult cmd_del (KVStore *s, const char *args);
static CmdResult cmd_list(KVStore *s, const char *args);
static CmdResult cmd_save(KVStore *s, const char *args);
static CmdResult cmd_load(KVStore *s, const char *args);
static CmdResult cmd_help(KVStore *s, const char *args);
static CmdResult cmd_quit(KVStore *s, const char *args);

/* ---------- Dispatch table ---------- */

static const Command g_commands[] = {
    { "set",  cmd_set,  "set <chiave> <i|d|s> <valore> — imposta una voce"       },
    { "get",  cmd_get,  "get <chiave> — stampa il valore"                         },
    { "del",  cmd_del,  "del <chiave> — rimuove la voce"                          },
    { "list", cmd_list, "list — elenca tutte le voci"                             },
    { "save", cmd_save, "save <file> — salva lo store su file binario"            },
    { "load", cmd_load, "load <file> — carica lo store da file binario"           },
    { "help", cmd_help, "help — mostra questo messaggio"                          },
    { "quit", cmd_quit, "quit — esce dal programma"                               },
};

CmdResult commands_dispatch(KVStore *s, const char *nome, const char *args) {
    for (size_t i = 0; i < sizeof g_commands / sizeof g_commands[0]; ++i) {
        if (strcmp(nome, g_commands[i].nome) == 0) {
            return g_commands[i].handler(s, args ? args : "");
        }
    }
    fprintf(stderr, "errore: comando sconosciuto '%s' (prova 'help')\n", nome);
    return CMD_ERROR;
}

void commands_help(void) {
    puts("Comandi disponibili:");
    for (size_t i = 0; i < sizeof g_commands / sizeof g_commands[0]; ++i) {
        printf("  %s\n", g_commands[i].aiuto);
    }
}

/* ---------- Implementazioni stub ---------- */

static CmdResult cmd_set(KVStore *s, const char *args) {
    /* TODO: parse "chiave tipo valore...".
     * - tipo 'i': strtoll su valore, return errore se trailing garbage.
     * - tipo 'd': strtod su valore, idem.
     * - tipo 's': tutto il resto della riga come stringa (strdup).
     * Invoca store_set. Se store_set fallisce, ricorda di liberare
     * la stringa allocata per 's'.
     * Stampa "OK" su successo.
     */
    (void)s; (void)args;
    return CMD_ERROR;
}

static CmdResult cmd_get(KVStore *s, const char *args) {
    /* TODO: usa store_get; se NULL stampa "(nil)"; altrimenti value_print. */
    (void)s; (void)args;
    return CMD_ERROR;
}

static CmdResult cmd_del(KVStore *s, const char *args) {
    /* TODO */
    (void)s; (void)args;
    return CMD_ERROR;
}

static CmdResult cmd_list(KVStore *s, const char *args) {
    (void)args;
    store_list(s, stdout);
    return CMD_OK;
}

static CmdResult cmd_save(KVStore *s, const char *args) {
    /* TODO: estrai path, chiama format_save, stampa "OK (N voci, M byte)". */
    (void)s; (void)args;
    return CMD_ERROR;
}

static CmdResult cmd_load(KVStore *s, const char *args) {
    /* TODO */
    (void)s; (void)args;
    return CMD_ERROR;
}

static CmdResult cmd_help(KVStore *s, const char *args) {
    (void)s; (void)args;
    commands_help();
    return CMD_OK;
}

static CmdResult cmd_quit(KVStore *s, const char *args) {
    (void)s; (void)args;
    return CMD_QUIT;
}
