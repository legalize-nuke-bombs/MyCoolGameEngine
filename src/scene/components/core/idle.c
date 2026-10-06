//
// Created by nikita on 25.09.2026.
//

#include "idle.h"

#include <stdlib.h>
#include "../component_internal.h"

struct idle {
    struct component base;
};


const struct component_vtable idle_vtable = {
    .component_key = idle_component_key,
    .size = sizeof(struct idle),
    .on_awake = NULL,
    .on_update = NULL,
    .on_disable = NULL
};

const char* idle_component_key(void) {
    return "idle";
}


