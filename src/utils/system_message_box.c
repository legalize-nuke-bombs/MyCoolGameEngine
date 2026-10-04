//
// Created by nikita on 04.10.2026.
//

#include "system_message_box.h"

#include "../logging/logger.h"

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    #define USE_WINAPI 1
    #include <windows.h>
#else
    #define USE_WINAPI 0
#endif

void system_message_box_show(const char *title, const char *message) {
    #if USE_WINAPI
        MessageBoxA(NULL, message, title, MB_OK | MB_TASKMODAL);
    #else
        logger_info("System message box content (support for proper message boxes for this system is not implemented): %s %s", title, message);
    #endif
}