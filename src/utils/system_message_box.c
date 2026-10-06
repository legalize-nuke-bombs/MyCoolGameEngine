//
// Created by nikita on 04.10.2026.
//

#include "system_message_box.h"

#include <SDL3/SDL.h>

#include "../logging/logger.h"

// While the box is open, the key up events go to it and not to the window. SDL knows that and releases every held key
// before it shows the box, a box opened behind its back (MessageBoxA) leaves those keys pressed forever
void system_message_box_show(const char *title, const char *message) {
    if (!SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, title, message, NULL)) {
        logger_warn("System message box failed to show (%s): %s %s", SDL_GetError(), title, message);
    }
}
