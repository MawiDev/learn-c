#ifndef KVDB_STORE_H
#define KVDB_STORE_H

#include "common.h"
#include <stdio.h>

typedef enum {
    V_INT    = 0,
    V_DOUBLE = 1,
    V_STRING = 2
} ValueKind;

/* Tagged union con anonymous union (C11). */
typedef struct {
    ValueKind kind;
    union {
        int64_t  i;
        double   d;
        char    *s;   /* owned dal Value quando in store */
    };
} Value;

typedef struct {
    char  *key;   /* owned dal record */
    Value  value;
} Record;

typedef struct {
    Record *items;
    size_t  len;
    size_t  cap;
} KVStore;

/* Lifecycle */
void store_init(KVStore *s);
void store_free(KVStore *s);

/* CRUD.
 * set prende ownership della stringa in value.s (se STRING) e della key
 * solo se il record e' nuovo. Se esiste gia', viene liberato il vecchio
 * valore e la vecchia stringa. Ritorna 0 su successo, -1 su errore
 * (in caso di errore, l'ownership NON viene trasferito: il chiamante
 *  mantiene la responsabilita' di liberare ciò che ha passato).
 */
int store_set(KVStore *s, const char *key, Value value);

/* get: ritorna un puntatore *interno* al Value. Non liberare. NULL se assente. */
const Value *store_get(const KVStore *s, const char *key);

/* del: 0 se rimosso, -1 se non trovato. */
int store_del(KVStore *s, const char *key);

/* Stampa leggibile (usata da "list" e "get"). */
void value_print(const Value *v, FILE *f);
void store_list(const KVStore *s, FILE *f);

#endif
