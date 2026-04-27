#include "store.h"
#include "log.h"

#include <stdlib.h>
#include <string.h>

/* Helper privato: trova indice della chiave o -1. */
static ptrdiff_t find_key(const KVStore *s, const char *key) {
    /* TODO */
    (void)s; (void)key;
    return -1;
}

/* Libera le risorse di un Value (solo le stringhe dinamiche). */
static void value_dispose(Value *v) {
    /* TODO: se V_STRING, free(v->s); set v->s = NULL. */
    (void)v;
}

void store_init(KVStore *s) {
    s->items = NULL;
    s->len = s->cap = 0;
    /* Riferimenti per silenziare -Wunused-function finche' gli stub
     * non sono completati. Rimuovi queste righe quando hai implementato
     * le funzioni helper. */
    (void)find_key; (void)value_dispose;
}

void store_free(KVStore *s) {
    /* TODO: per ogni record, free(key) e value_dispose. Poi free(items). */
    (void)s;
}

int store_set(KVStore *s, const char *key, Value value) {
    /* TODO:
     * 1. Verifica key non NULL.
     * 2. Se la chiave esiste:
     *    - sostituisci il valore (prima dispose del vecchio).
     *    - return 0.
     * 3. Se nuova:
     *    - grow di items se len == cap (raddoppio).
     *    - items[len].key = strdup(key); controllo errore.
     *    - items[len].value = value;
     *    - ++len; return 0;
     */
    (void)s; (void)key; (void)value;
    return -1;
}

const Value *store_get(const KVStore *s, const char *key) {
    /* TODO */
    (void)s; (void)key;
    return NULL;
}

int store_del(KVStore *s, const char *key) {
    /* TODO: swap-remove con l'ultimo per evitare shift O(n). */
    (void)s; (void)key;
    return -1;
}

void value_print(const Value *v, FILE *f) {
    /* TODO: stampa in base a v->kind.
     *   V_INT    -> "%" PRId64
     *   V_DOUBLE -> "%g"
     *   V_STRING -> "\"%s\""
     */
    (void)v; (void)f;
}

void store_list(const KVStore *s, FILE *f) {
    /* TODO: per ogni record stampa "<key>\t= <tipo>\t<value>". */
    (void)s; (void)f;
}
