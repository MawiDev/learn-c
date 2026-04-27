#include "store.h"
#include "log.h"

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

static ptrdiff_t find_key(const KVStore *s, const char *key) {
    for (size_t i = 0; i < s->len; ++i) {
        if (strcmp(s->items[i].key, key) == 0) {
            return (ptrdiff_t)i;
        }
    }
    return -1;
}

static void value_dispose(Value *v) {
    if (v->kind == V_STRING) {
        free(v->s);
        v->s = NULL;
    }
}

static char *dup_cstr(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    if (!p) return NULL;
    memcpy(p, s, n);
    return p;
}

void store_init(KVStore *s) {
    s->items = NULL;
    s->len   = 0;
    s->cap   = 0;
}

void store_free(KVStore *s) {
    if (!s) return;
    for (size_t i = 0; i < s->len; ++i) {
        free(s->items[i].key);
        value_dispose(&s->items[i].value);
    }
    free(s->items);
    s->items = NULL;
    s->len = s->cap = 0;
}

static int store_grow(KVStore *s) {
    size_t ncap = s->cap ? s->cap * 2 : 8;
    Record *tmp = realloc(s->items, ncap * sizeof *tmp);
    if (!tmp) {
        LOG_E("realloc fallita per %zu record", ncap);
        return -1;
    }
    s->items = tmp;
    s->cap   = ncap;
    return 0;
}

int store_set(KVStore *s, const char *key, Value value) {
    if (!key) return -1;

    ptrdiff_t idx = find_key(s, key);
    if (idx >= 0) {
        /* sovrascrivi: dispose del vecchio valore, poi assegna. */
        value_dispose(&s->items[idx].value);
        s->items[idx].value = value;
        return 0;
    }

    if (s->len == s->cap && store_grow(s) != 0) {
        return -1;
    }

    char *kcopy = dup_cstr(key);
    if (!kcopy) {
        LOG_E("OOM duplicando chiave '%s'", key);
        return -1;
    }

    s->items[s->len].key   = kcopy;
    s->items[s->len].value = value;
    ++s->len;
    return 0;
}

const Value *store_get(const KVStore *s, const char *key) {
    ptrdiff_t idx = find_key(s, key);
    if (idx < 0) return NULL;
    return &s->items[idx].value;
}

int store_del(KVStore *s, const char *key) {
    ptrdiff_t idx = find_key(s, key);
    if (idx < 0) return -1;

    free(s->items[idx].key);
    value_dispose(&s->items[idx].value);

    /* swap-remove con l'ultimo per evitare shift O(n). */
    size_t last = s->len - 1;
    if ((size_t)idx != last) {
        s->items[idx] = s->items[last];
    }
    --s->len;
    return 0;
}

void value_print(const Value *v, FILE *f) {
    switch (v->kind) {
        case V_INT:
            fprintf(f, "%" PRId64, v->i);
            break;
        case V_DOUBLE:
            fprintf(f, "%g", v->d);
            break;
        case V_STRING:
            fprintf(f, "\"%s\"", v->s ? v->s : "");
            break;
    }
}

void store_list(const KVStore *s, FILE *f) {
    for (size_t i = 0; i < s->len; ++i) {
        const Record *r = &s->items[i];
        const char *tname =
            r->value.kind == V_INT    ? "int"    :
            r->value.kind == V_DOUBLE ? "double" :
            r->value.kind == V_STRING ? "string" : "?";
        fprintf(f, "%-12s = %-6s  ", r->key, tname);
        value_print(&r->value, f);
        fputc('\n', f);
    }
}
