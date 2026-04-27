#include "commands.h"
#include "store.h"
#include "format.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <inttypes.h>

/* ---------- Prototipi degli handler ---------- */

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
    { "set",  cmd_set,  "set <chiave> <i|d|s> <valore> — imposta una voce"  },
    { "get",  cmd_get,  "get <chiave> — stampa il valore"                    },
    { "del",  cmd_del,  "del <chiave> — rimuove la voce"                     },
    { "list", cmd_list, "list — elenca tutte le voci"                        },
    { "save", cmd_save, "save <file> — salva lo store su file binario"       },
    { "load", cmd_load, "load <file> — carica lo store da file binario"      },
    { "help", cmd_help, "help — mostra questo messaggio"                     },
    { "quit", cmd_quit, "quit — esce dal programma"                          },
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

/* ---------- Helper parsing ---------- */

static const char *skip_ws(const char *s) {
    while (*s && isspace((unsigned char)*s)) ++s;
    return s;
}

/* Estrae un "word" (sequenza non-ws) da *p in buf (dim cap). Avanza *p oltre
 * il token e lo whitespace successivo. Ritorna true se ha trovato qualcosa. */
static bool next_word(const char **p, char *buf, size_t cap) {
    const char *s = skip_ws(*p);
    if (*s == '\0') { *p = s; return false; }

    size_t i = 0;
    while (*s && !isspace((unsigned char)*s)) {
        if (i + 1 >= cap) break;
        buf[i++] = *s++;
    }
    buf[i] = '\0';
    *p = skip_ws(s);
    return i > 0;
}

static char *dup_cstr(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}

/* ---------- Handler: set ---------- */

static CmdResult cmd_set(KVStore *s, const char *args) {
    char key[256];
    char tipo[8];

    const char *p = args;
    if (!next_word(&p, key, sizeof key)) {
        fprintf(stderr, "uso: set <chiave> <i|d|s> <valore>\n");
        return CMD_ERROR;
    }
    if (!next_word(&p, tipo, sizeof tipo)) {
        fprintf(stderr, "uso: set <chiave> <i|d|s> <valore>\n");
        return CMD_ERROR;
    }

    if (tipo[1] != '\0' || !strchr("ids", tipo[0])) {
        fprintf(stderr, "errore: tipo '%s' non valido (usa i, d, o s)\n", tipo);
        return CMD_ERROR;
    }

    Value v;
    v.kind = V_INT;    /* default */

    const char *rest = p;   /* gia' trimmato a sinistra da next_word */

    if (tipo[0] == 'i') {
        if (*rest == '\0') { fprintf(stderr, "errore: valore int mancante\n"); return CMD_ERROR; }
        errno = 0;
        char *end = NULL;
        long long lv = strtoll(rest, &end, 10);
        if (errno != 0 || end == rest) {
            fprintf(stderr, "errore: parsing int\n"); return CMD_ERROR;
        }
        const char *trail = skip_ws(end);
        if (*trail != '\0') {
            fprintf(stderr, "errore: spazzatura dopo l'intero: '%s'\n", trail);
            return CMD_ERROR;
        }
        v.kind = V_INT;
        v.i = (int64_t)lv;
    } else if (tipo[0] == 'd') {
        if (*rest == '\0') { fprintf(stderr, "errore: valore double mancante\n"); return CMD_ERROR; }
        errno = 0;
        char *end = NULL;
        double dv = strtod(rest, &end);
        if (errno != 0 || end == rest) {
            fprintf(stderr, "errore: parsing double\n"); return CMD_ERROR;
        }
        const char *trail = skip_ws(end);
        if (*trail != '\0') {
            fprintf(stderr, "errore: spazzatura dopo il double: '%s'\n", trail);
            return CMD_ERROR;
        }
        v.kind = V_DOUBLE;
        v.d = dv;
    } else {
        /* stringa: il resto della riga */
        v.kind = V_STRING;
        v.s = dup_cstr(rest);
        if (!v.s) {
            LOG_E("OOM duplicando valore stringa");
            return CMD_ERROR;
        }
    }

    if (store_set(s, key, v) != 0) {
        /* Pulisci la stringa allocata se store non ne ha preso possesso. */
        if (v.kind == V_STRING) free(v.s);
        return CMD_ERROR;
    }
    puts("OK");
    return CMD_OK;
}

/* ---------- Handler: get ---------- */

static CmdResult cmd_get(KVStore *s, const char *args) {
    char key[256];
    const char *p = args;
    if (!next_word(&p, key, sizeof key)) {
        fprintf(stderr, "uso: get <chiave>\n");
        return CMD_ERROR;
    }
    const Value *v = store_get(s, key);
    if (!v) {
        puts("(nil)");
        return CMD_OK;
    }
    value_print(v, stdout);
    fputc('\n', stdout);
    return CMD_OK;
}

/* ---------- Handler: del ---------- */

static CmdResult cmd_del(KVStore *s, const char *args) {
    char key[256];
    const char *p = args;
    if (!next_word(&p, key, sizeof key)) {
        fprintf(stderr, "uso: del <chiave>\n");
        return CMD_ERROR;
    }
    if (store_del(s, key) == 0) {
        puts("OK");
    } else {
        puts("not found");
    }
    return CMD_OK;
}

/* ---------- Handler: list ---------- */

static CmdResult cmd_list(KVStore *s, const char *args) {
    (void)args;
    store_list(s, stdout);
    return CMD_OK;
}

/* ---------- Handler: save ---------- */

static CmdResult cmd_save(KVStore *s, const char *args) {
    char path[1024];
    const char *p = args;
    if (!next_word(&p, path, sizeof path)) {
        fprintf(stderr, "uso: save <file>\n");
        return CMD_ERROR;
    }
    size_t bytes = 0;
    if (format_save(s, path, &bytes) != 0) {
        return CMD_ERROR;
    }
    printf("OK (%zu voci scritte, %zu byte)\n", s->len, bytes);
    return CMD_OK;
}

/* ---------- Handler: load ---------- */

static CmdResult cmd_load(KVStore *s, const char *args) {
    char path[1024];
    const char *p = args;
    if (!next_word(&p, path, sizeof path)) {
        fprintf(stderr, "uso: load <file>\n");
        return CMD_ERROR;
    }
    size_t count = 0;
    if (format_load(s, path, &count) != 0) {
        return CMD_ERROR;
    }
    printf("OK (%zu voci caricate)\n", count);
    return CMD_OK;
}

/* ---------- Handler: help / quit ---------- */

static CmdResult cmd_help(KVStore *s, const char *args) {
    (void)s; (void)args;
    commands_help();
    return CMD_OK;
}

static CmdResult cmd_quit(KVStore *s, const char *args) {
    (void)s; (void)args;
    return CMD_QUIT;
}
