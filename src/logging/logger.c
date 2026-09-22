#include "logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

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

    time_t now;
    time(&now);

    const struct tm *tm = localtime(&now);

    char time_buffer[24];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", tm);

    printf("[%s] ", time_buffer);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}
