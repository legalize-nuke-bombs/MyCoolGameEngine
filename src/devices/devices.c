//
// Created by nikita on 24.09.2026.
//

#include "devices.h"

#include <stdlib.h>

#include "keyboard.h"
#include "../logging/logger.h"


struct devices {
    struct keyboard *keyboard;
};


struct devices* devices_create() {
    logger_info("Devices are creating...");
    struct devices* this = malloc(sizeof(struct devices));
    this->keyboard = keyboard_create();
    return this;
}
void devices_destroy(struct devices* this) {
    logger_info("Devices are destroying...");
    keyboard_destroy(this->keyboard);
    free(this);
}

struct keyboard* devices_get_keyboard(const struct devices *this) {
    return this->keyboard;
}
