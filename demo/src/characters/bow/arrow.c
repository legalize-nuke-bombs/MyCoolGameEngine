//
// Created by Nikita on 08.10.2026.
//

#include "arrow.h"
#include <mcge/mcge.h>


struct arrow {
    struct component base;

    uint128_t target_id;
};


const char* arrow_component_key(void) {
    return "arrow";
}

const struct component_vtable arrow_vtable = {
    .component_key = arrow_component_key,
    .size = sizeof(struct arrow)
};

void arrow_launch(struct arrow *this, struct character *target) {
    this->target_id = component_get_id((struct component*)target);
}