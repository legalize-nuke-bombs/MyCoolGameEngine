#include "logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include "ansi.h"

static int logger_level;

void logger_init(int level) {
    if (level >= LOGGER_LEVEL_MIN && level <= LOGGER_LEVEL_MAX) {
        logger_level = level;
    }
    else {
        logger_level = LOGGER_LEVEL_DISABLED;
    }
}

void logger_log(int level, const char* format, ...) {
    if (level < LOGGER_LEVEL_MIN) {
        level = LOGGER_LEVEL_MIN;
    }
    if (level > LOGGER_LEVEL_MAX) {
        level = LOGGER_LEVEL_MAX;
    }
    if (level < logger_level) {
        return;
    }

    const char* color_code = ANSI_COLOR_RESET;
    const char* level_name = "";

    switch (level) {
        case LOGGER_LEVEL_DEBUG:
            color_code = ANSI_COLOR_DEBUG;
            level_name = "[DEBUG]";
            break;
        case LOGGER_LEVEL_INFO:
            color_code = ANSI_COLOR_INFO;
            level_name = "[INFO] ";
            break;
        case LOGGER_LEVEL_WARN:
            color_code = ANSI_COLOR_WARN;
            level_name = "[WARN] ";
            break;
        case LOGGER_LEVEL_ERROR:
            color_code = ANSI_COLOR_ERROR;
            level_name = "[ERROR]";
            break;
    }

    printf("%s%s" ANSI_COLOR_RESET " ", color_code, level_name);

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

    printf("\n");
}
