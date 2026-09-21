#include "logger.h"

#include <stdarg.h>
#include <stdio.h>

static int logger_level;

void logger_init(int level) {
    if (level >= LOGGER_LEVEL_MIN && level <= LOGGER_LEVEL_MAX) {
        logger_level = level;
    }
    else {
        logger_level = LOGGER_LEVEL_DISABLED;
    }
}

void logger_log(const int level, const char* format, ...) {
    if (level < logger_level) {
        return;
    }

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}