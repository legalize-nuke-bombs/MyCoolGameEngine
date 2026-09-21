#include "logger.h"

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

void logger_log(const int level, const char* string) {
    if (level >= logger_level) {
        printf("%s", string);
    }
}

void logger_debug(const char* string) {
    logger_log(LOGGER_LEVEL_DEBUG, string);
}
void logger_info(const char* string) {
    logger_log(LOGGER_LEVEL_INFO, string);
}
void logger_warn(const char* string) {
    logger_log(LOGGER_LEVEL_WARN, string);
}
void logger_error(const char* string) {
    logger_log(LOGGER_LEVEL_ERROR, string);
}