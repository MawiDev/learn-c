#include "format.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Helper di scrittura little-endian */
static void put_u32le(uint8_t *p, uint32_t v) {
    /* TODO */
    (void)p; (void)v;
}

static void put_u64le(uint8_t *p, uint64_t v) {
    /* TODO */
    (void)p; (void)v;
}

static uint32_t get_u32le(const uint8_t *p) {
    /* TODO */
    (void)p;
    return 0;
}

static uint64_t get_u64le(const uint8_t *p) {
    /* TODO */
    (void)p;
    return 0;
}

/* Riferimenti per silenziare -Wunused-function finche' gli stub
 * non sono completati. Elimina quando hai implementato save/load. */
static void unused_refs(void) {
    (void)put_u32le; (void)put_u64le;
    (void)get_u32le; (void)get_u64le;
}

int format_save(const KVStore *s, const char *path, size_t *bytes_out) {
    unused_refs();
    /* TODO:
     * 1. apri file "wb" (goto cleanup on failure).
     * 2. scrivi magic "KVDB" + version LE + count LE.
     * 3. per ogni record:
     *    - scrivi key_len + key bytes
     *    - scrivi kind (1 byte)
     *    - scrivi il dato in base al tipo, sempre LE
     * 4. chiudi file, controlla fclose.
     * 5. *bytes_out = total scritti.
     */
    (void)s; (void)path; (void)bytes_out;
    return -1;
}

int format_load(KVStore *s, const char *path, size_t *count_out) {
    /* TODO:
     * 1. apri file "rb".
     * 2. leggi e verifica magic.
     * 3. leggi version: se != KVDB_VERSION, errore.
     * 4. leggi count.
     * 5. costruisci uno store TEMPORANEO (cosi' se fallisce a meta', quello
     *    vero resta intatto).
     * 6. per ogni record: leggi key, kind, dato -> store_set.
     * 7. se tutto ok: store_free(s); *s = tmp; *count_out = count.
     *    se errore: store_free(&tmp); return -1;
     */
    (void)s; (void)path; (void)count_out;
    return -1;
}
