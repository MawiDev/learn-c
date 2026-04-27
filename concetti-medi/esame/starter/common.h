#ifndef KVDB_COMMON_H
#define KVDB_COMMON_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

/* Static assertion fondamentali: ci appoggiamo a rappresentazioni standard. */
_Static_assert(sizeof(double)  == 8, "richiediamo IEEE-754 binary64");
_Static_assert(sizeof(int64_t) == 8, "int64_t deve essere 8 byte");

#endif
