#include "log.h"
#include <stdio.h>
#include <stdarg.h>

static LogLevel g_level = LOG_INFO;

void log_set_level(LogLevel l) {
    g_level = l;
}

static const char *name_of(LogLevel l) {
    switch (l) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO:  return "INFO";
        case LOG_WARN:  return "WARN";
        case LOG_ERROR: return "ERROR";
    }
    return "?";
}

void log_write(LogLevel l, const char *file, int line, const char *func,
               const char *fmt, ...) {
    if (l < g_level) return;
    fprintf(stderr, "[%-5s %s:%d %s] ", name_of(l), file, line, func);
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);
}
