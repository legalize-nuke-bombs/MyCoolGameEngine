//
// Created by nikita on 14.10.2026.
//

#include "text_renderer.h"
#include "mcge/scene/components/component_internal.h"


struct text_renderer {
    struct component base;
};


const char* text_renderer_component_key(void) {
    return "text_renderer";
}

const struct component_vtable text_renderer_vtable = {
    .component_key = text_renderer_component_key,
    .size = sizeof(struct text_renderer),
};