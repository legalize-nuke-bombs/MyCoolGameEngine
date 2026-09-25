//
// Created by Nikita on 25.09.2026.
//

#include "subsystem_internal.h"
#include "../logging/logger.h"


void subsystem_create(struct subsystem *this, const struct subsystem_vtable *vtable, const struct subsystem_collection *subsystems) {
    this->vtable = vtable;
    logger_info("Subsystem `%s` is creating...", subsystem_get_name(this));
    this->enabled = false;
    this->subsystems = subsystems;
}
void subsystem_destroy(struct subsystem *this) {
    subsystem_disable(this);
    logger_info("Subsystem `%s` is destroying...", subsystem_get_name(this));
    if (this->vtable->on_destroy) {
        this->vtable->on_destroy(this);
    }
}

void subsystem_enable(struct subsystem *this) {
    if (this->enabled) {
        return;
    }
    logger_info("Subsystem `%s` is enabling...", subsystem_get_name(this));
    if (this->vtable->on_enable) {
        this->vtable->on_enable(this);
    }
}
void subsystem_disable(struct subsystem *this) {
    if (!this->enabled) {
        return;
    }
    logger_info("Subsystem `%s` is disabling...", subsystem_get_name(this));
    if (this->vtable->on_disable) {
        this->vtable->on_disable(this);
    }
}

const char* subsystem_get_name(const struct subsystem *this) {
    return this->vtable->name();
}