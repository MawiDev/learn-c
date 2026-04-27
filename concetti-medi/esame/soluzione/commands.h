#ifndef KVDB_COMMANDS_H
#define KVDB_COMMANDS_H

#include "store.h"

typedef enum {
    CMD_OK    =  0,
    CMD_ERROR = -1,
    CMD_QUIT  =  1
} CmdResult;

typedef CmdResult (*CmdHandler)(KVStore *store, const char *args);

typedef struct {
    const char *nome;
    CmdHandler  handler;
    const char *aiuto;
} Command;

CmdResult commands_dispatch(KVStore *s, const char *nome, const char *args);
void      commands_help(void);

#endif
