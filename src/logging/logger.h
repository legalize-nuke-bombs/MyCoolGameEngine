#ifndef MYCOOLGAMEENGINE_LOGGER_H
#define MYCOOLGAMEENGINE_LOGGER_H

#define LOGGER_LEVEL_DEBUG 0
#define LOGGER_LEVEL_INFO 1
#define LOGGER_LEVEL_WARN 2
#define LOGGER_LEVEL_ERROR 3
#define LOGGER_LEVEL_DISABLED 4

#define LOGGER_LEVEL_MIN LOGGER_LEVEL_DEBUG
#define LOGGER_LEVEL_MAX LOGGER_LEVEL_DISABLED

void logger_init(int level);

void logger_log(int level, const char* format, ...) __attribute__((format(printf, 2, 3)));

#define logger_debug(...) logger_log(LOGGER_LEVEL_DEBUG, __VA_ARGS__)
#define logger_info(...) logger_log(LOGGER_LEVEL_INFO, __VA_ARGS__)
#define logger_warn(...) logger_log(LOGGER_LEVEL_WARN, __VA_ARGS__)
#define logger_error(...) logger_log(LOGGER_LEVEL_ERROR, __VA_ARGS__)

#endif
