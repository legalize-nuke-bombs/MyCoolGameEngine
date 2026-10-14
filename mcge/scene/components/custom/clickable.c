//
// Created by Nikita on 14.10.2026.
//

#include "clickable.h"
#include "mcge/scene/components/component_internal.h"


struct clickable {
    struct component component;

    unsigned int on_mouse_button_pressed_token;
};


const char* clickable_component_key(void) {
    return "clickable";
}


const struct component_vtable clickable_vtable = {
    .component_key = clickable_component_key,
    .size = sizeof(struct clickable),
};