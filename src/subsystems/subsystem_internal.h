//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_SUBSYSTEM_INTERNAL_H
#define MYCOOLGAMEENGINE_SUBSYSTEM_INTERNAL_H

#include <stdbool.h>

#include "subsystem.h"

struct subsystem_vtable {
    void (*on_enable)(struct subsystem *this, struct engine_arguments args);
    void (*on_disable)(struct subsystem *this);
    void (*on_destroy)(struct subsystem *this);
    const char* (*name)(void);
};

struct subsystem_collection;

struct subsystem {
    const struct subsystem_vtable *vtable;
    bool enabled;
    const struct subsystem_collection *subsystems;
};

void subsystem_create(struct subsystem *this, const struct subsystem_vtable *vtable, const struct subsystem_collection *subsystems);

#endif //MYCOOLGAMEENGINE_SUBSYSTEM_INTERNAL_H
