#ifndef KVDB_LOG_H
#define KVDB_LOG_H

#include "common.h"

typedef enum { LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR } LogLevel;

void log_set_level(LogLevel l);
void log_write(LogLevel l, const char *file, int line, const char *func,
               const char *fmt, ...);

#define LOG(lvl, ...) log_write((lvl), __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_D(...)    LOG(LOG_DEBUG, __VA_ARGS__)
#define LOG_I(...)    LOG(LOG_INFO,  __VA_ARGS__)
#define LOG_W(...)    LOG(LOG_WARN,  __VA_ARGS__)
#define LOG_E(...)    LOG(LOG_ERROR, __VA_ARGS__)

#endif
