#include "format.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* ---------- Little-endian helpers ---------- */

static void put_u32le(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)(v       & 0xFF);
    p[1] = (uint8_t)(v >>  8 & 0xFF);
    p[2] = (uint8_t)(v >> 16 & 0xFF);
    p[3] = (uint8_t)(v >> 24 & 0xFF);
}

static void put_u64le(uint8_t *p, uint64_t v) {
    for (int i = 0; i < 8; ++i) {
        p[i] = (uint8_t)(v & 0xFF);
        v >>= 8;
    }
}

static uint32_t get_u32le(const uint8_t *p) {
    return (uint32_t)p[0]
         | ((uint32_t)p[1] <<  8)
         | ((uint32_t)p[2] << 16)
         | ((uint32_t)p[3] << 24);
}

static uint64_t get_u64le(const uint8_t *p) {
    uint64_t v = 0;
    for (int i = 7; i >= 0; --i) {
        v = (v << 8) | p[i];
    }
    return v;
}

/* ---------- Write helpers (scrivono su FILE, ritornano 0/-1) ---------- */

static int write_bytes(FILE *f, const void *buf, size_t n) {
    return (fwrite(buf, 1, n, f) == n) ? 0 : -1;
}

static int write_u32(FILE *f, uint32_t v) {
    uint8_t b[4];
    put_u32le(b, v);
    return write_bytes(f, b, 4);
}

static int write_u64(FILE *f, uint64_t v) {
    uint8_t b[8];
    put_u64le(b, v);
    return write_bytes(f, b, 8);
}

static int write_double(FILE *f, double d) {
    uint64_t u;
    memcpy(&u, &d, 8);       /* type-punning sicuro via memcpy */
    return write_u64(f, u);
}

static int write_i64(FILE *f, int64_t v) {
    uint64_t u;
    memcpy(&u, &v, 8);
    return write_u64(f, u);
}

/* ---------- Read helpers ---------- */

static int read_bytes(FILE *f, void *buf, size_t n) {
    return (fread(buf, 1, n, f) == n) ? 0 : -1;
}

static int read_u32(FILE *f, uint32_t *out) {
    uint8_t b[4];
    if (read_bytes(f, b, 4) != 0) return -1;
    *out = get_u32le(b);
    return 0;
}

static int read_u64(FILE *f, uint64_t *out) {
    uint8_t b[8];
    if (read_bytes(f, b, 8) != 0) return -1;
    *out = get_u64le(b);
    return 0;
}

static int read_double(FILE *f, double *out) {
    uint64_t u;
    if (read_u64(f, &u) != 0) return -1;
    memcpy(out, &u, 8);
    return 0;
}

static int read_i64(FILE *f, int64_t *out) {
    uint64_t u;
    if (read_u64(f, &u) != 0) return -1;
    memcpy(out, &u, 8);
    return 0;
}

/* ---------- SAVE ---------- */

int format_save(const KVStore *s, const char *path, size_t *bytes_out) {
    int   rc = -1;
    FILE *f  = NULL;

    f = fopen(path, "wb");
    if (!f) {
        LOG_E("fopen(%s): %s", path, strerror(errno));
        goto out;
    }

    /* magic + version + count */
    if (fwrite(KVDB_MAGIC, 1, 4, f) != 4) goto out;
    if (write_u32(f, KVDB_VERSION) != 0) goto out;
    if (s->len > UINT32_MAX) { LOG_E("troppe voci"); goto out; }
    if (write_u32(f, (uint32_t)s->len) != 0) goto out;

    for (size_t i = 0; i < s->len; ++i) {
        const Record *r = &s->items[i];
        size_t klen = strlen(r->key);
        if (klen > UINT32_MAX) { LOG_E("chiave troppo lunga"); goto out; }

        if (write_u32(f, (uint32_t)klen) != 0)          goto out;
        if (write_bytes(f, r->key, klen) != 0)          goto out;

        uint8_t kind = (uint8_t)r->value.kind;
        if (write_bytes(f, &kind, 1) != 0)              goto out;

        switch (r->value.kind) {
            case V_INT:
                if (write_i64(f, r->value.i) != 0) goto out;
                break;
            case V_DOUBLE:
                if (write_double(f, r->value.d) != 0) goto out;
                break;
            case V_STRING: {
                size_t slen = r->value.s ? strlen(r->value.s) : 0;
                if (slen > UINT32_MAX) goto out;
                if (write_u32(f, (uint32_t)slen) != 0) goto out;
                if (slen > 0 && write_bytes(f, r->value.s, slen) != 0) goto out;
                break;
            }
        }
    }

    /* fflush prima di ftell per avere dim reale in caso di buffering */
    if (fflush(f) != 0) goto out;
    long pos = ftell(f);
    if (pos < 0) goto out;

    if (bytes_out) *bytes_out = (size_t)pos;
    rc = 0;

out:
    if (f && fclose(f) != 0 && rc == 0) {
        LOG_E("fclose fallita");
        rc = -1;
    }
    return rc;
}

/* ---------- LOAD ---------- */

int format_load(KVStore *s, const char *path, size_t *count_out) {
    int      rc  = -1;
    FILE    *f   = NULL;
    KVStore  tmp;
    char    *kbuf = NULL;
    char    *sbuf = NULL;

    store_init(&tmp);

    f = fopen(path, "rb");
    if (!f) {
        LOG_E("fopen(%s): %s", path, strerror(errno));
        goto out;
    }

    char magic[4];
    if (read_bytes(f, magic, 4) != 0) {
        LOG_E("file troppo corto per magic");
        goto out;
    }
    if (memcmp(magic, KVDB_MAGIC, 4) != 0) {
        LOG_E("magic number invalido");
        goto out;
    }

    uint32_t version = 0, count = 0;
    if (read_u32(f, &version) != 0) { LOG_E("version mancante"); goto out; }
    if (version != KVDB_VERSION) {
        LOG_E("versione non supportata: %u", version);
        goto out;
    }
    if (read_u32(f, &count) != 0) { LOG_E("count mancante"); goto out; }

    for (uint32_t i = 0; i < count; ++i) {
        uint32_t klen;
        if (read_u32(f, &klen) != 0) goto out;
        kbuf = malloc((size_t)klen + 1);
        if (!kbuf) goto out;
        if (klen > 0 && read_bytes(f, kbuf, klen) != 0) goto out;
        kbuf[klen] = '\0';

        uint8_t kind;
        if (read_bytes(f, &kind, 1) != 0) goto out;

        Value v;
        v.kind = (ValueKind)kind;
        switch (kind) {
            case V_INT:
                if (read_i64(f, &v.i) != 0) goto out;
                break;
            case V_DOUBLE:
                if (read_double(f, &v.d) != 0) goto out;
                break;
            case V_STRING: {
                uint32_t slen;
                if (read_u32(f, &slen) != 0) goto out;
                sbuf = malloc((size_t)slen + 1);
                if (!sbuf) goto out;
                if (slen > 0 && read_bytes(f, sbuf, slen) != 0) goto out;
                sbuf[slen] = '\0';
                v.s = sbuf;
                sbuf = NULL;   /* ownership ceduta al Value */
                break;
            }
            default:
                LOG_E("tipo sconosciuto %u nel file", kind);
                goto out;
        }

        if (store_set(&tmp, kbuf, v) != 0) {
            /* se store_set fallisce, dobbiamo liberare v.s se era STRING */
            if (v.kind == V_STRING) free(v.s);
            goto out;
        }
        free(kbuf);
        kbuf = NULL;
    }

    /* Successo: sostituisci lo store dell'utente con tmp. */
    store_free(s);
    *s = tmp;
    /* Reinizializza tmp a vuoto cosi' lo store_free nel cleanup e' no-op. */
    store_init(&tmp);

    if (count_out) *count_out = count;
    rc = 0;

out:
    free(kbuf);
    free(sbuf);
    if (f) fclose(f);
    store_free(&tmp);   /* no-op se abbiamo ceduto */
    return rc;
}
