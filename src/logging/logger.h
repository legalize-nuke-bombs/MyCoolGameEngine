#ifndef MYCOOLGAMEENGINE_LOGGER_H
#define MYCOOLGAMEENGINE_LOGGER_H

#define LOGGER_LEVEL_DEBUG 0
#define LOGGER_LEVEL_INFO 1
#define LOGGER_LEVEL_WARN 2
#define LOGGER_LEVEL_ERROR 3
#define LOGGER_LEVEL_DISABLED 4

#define LOGGER_LEVEL_MIN LOGGER_LEVEL_DEBUG
#define LOGGER_LEVEL_MAX LOGGER_LEVEL_DISABLED

static int logger_level;

void logger_init(int level);

void logger_log(int level, const char* string);

void logger_debug(const char* string);
void logger_info(const char* string);
void logger_warn(const char* string);
void logger_error(const char* string);

#endif
