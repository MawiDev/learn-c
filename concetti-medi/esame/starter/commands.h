#ifndef KVDB_COMMANDS_H
#define KVDB_COMMANDS_H

#include "store.h"

/* Codici di ritorno di un handler. */
typedef enum {
    CMD_OK       =  0,
    CMD_ERROR    = -1,
    CMD_QUIT     =  1
} CmdResult;

typedef CmdResult (*CmdHandler)(KVStore *store, const char *args);

typedef struct {
    const char  *nome;
    CmdHandler   handler;
    const char  *aiuto;
} Command;

/* Cerca un comando per nome e lo esegue. Ritorna il codice dell'handler,
 * o CMD_ERROR se il nome non e' riconosciuto. */
CmdResult commands_dispatch(KVStore *s, const char *nome, const char *args);

/* Stampa l'help di tutti i comandi. */
void commands_help(void);

#endif
