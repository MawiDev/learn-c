#ifndef KVDB_STORE_H
#define KVDB_STORE_H

#include "common.h"
#include <stdio.h>

typedef enum {
    V_INT    = 0,
    V_DOUBLE = 1,
    V_STRING = 2
} ValueKind;

typedef struct {
    ValueKind kind;
    union {
        int64_t  i;
        double   d;
        char    *s;
    };
} Value;

typedef struct {
    char  *key;
    Value  value;
} Record;

typedef struct {
    Record *items;
    size_t  len;
    size_t  cap;
} KVStore;

void         store_init(KVStore *s);
void         store_free(KVStore *s);

int          store_set(KVStore *s, const char *key, Value value);
const Value *store_get(const KVStore *s, const char *key);
int          store_del(KVStore *s, const char *key);

void         value_print(const Value *v, FILE *f);
void         store_list(const KVStore *s, FILE *f);

#endif
