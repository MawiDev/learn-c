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
    /* TODO: filtra se l < g_level; stampa su stderr nel formato:
     *   [LEVEL file:line func] messaggio
     * Usa vfprintf per la parte variadica.
     */
    (void)l; (void)file; (void)line; (void)func; (void)fmt;
    (void)name_of;
}
