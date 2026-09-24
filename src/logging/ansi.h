//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ANSI_H
#define MYCOOLGAMEENGINE_ANSI_H

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif
#endif

#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_DEBUG   "\x1b[36m"
#define ANSI_COLOR_INFO    "\x1b[32m"
#define ANSI_COLOR_WARN    "\x1b[33m"
#define ANSI_COLOR_ERROR   "\x1b[31m"

#endif //MYCOOLGAMEENGINE_ANSI_H
