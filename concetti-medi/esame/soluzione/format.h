#ifndef KVDB_FORMAT_H
#define KVDB_FORMAT_H

#include "store.h"

#define KVDB_MAGIC   "KVDB"
#define KVDB_VERSION 1u

int format_save(const KVStore *s, const char *path, size_t *bytes_out);
int format_load(KVStore *s, const char *path, size_t *count_out);

#endif
