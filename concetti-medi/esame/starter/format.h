#ifndef KVDB_FORMAT_H
#define KVDB_FORMAT_H

#include "store.h"

#define KVDB_MAGIC   "KVDB"
#define KVDB_VERSION 1u

/* Serializza lo store in binario.
 * Ritorna 0 su successo, -1 altrimenti. Su errore, *bytes_out non e' affidabile. */
int format_save(const KVStore *s, const char *path, size_t *bytes_out);

/* Legge un file e ripopola lo store (che viene prima svuotato).
 * Ritorna 0 su successo, -1 altrimenti. Se fallisce, lo store viene lasciato
 * vuoto (non parzialmente caricato). */
int format_load(KVStore *s, const char *path, size_t *count_out);

#endif
