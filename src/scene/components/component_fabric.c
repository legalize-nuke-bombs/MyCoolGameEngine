//
// Created by nikita on 24.09.2026.
//

#include "component_fabric.h"

#include <stdlib.h>

#include "core/transform.h"
#include "../../utils/dictionary.h"
#include "../../utils/string_dictionary.h"
#include "../../logging/logger.h"
#include "core/controller.h"
#include "core/idle.h"
#include "core/keyboard_controller.h"
#include "demo/armageddon.h"
#include "rendering/box_renderer.h"
#include "rendering/camera.h"
#include "demo/printer.h"
#include "demo/suicidal.h"
#include "rendering/box_light.h"
#include "rendering/renderer_settings.h"

struct component_fabric {
    struct dictionary* types;
};

static void component_fabric_register_component(const struct component_fabric *this, const char* component_key, struct component*(*constructor)(struct parser *parser, struct entity *parent)) {
    if (dictionary_try_add(this->types, (void*)component_key, constructor)) {
        logger_info("Component fabric knows component %s", component_key);
    }
    else {
        logger_warn("Component fabric failed to register component %s", component_key);
    }
}

static void component_fabric_register_all(const struct component_fabric *this) {
    component_fabric_register_component(this, transform_component_key(), transform_create);
    component_fabric_register_component(this, idle_component_key(), idle_create);
    component_fabric_register_component(this, camera_component_key(), camera_create);
    component_fabric_register_component(this, renderer_settings_component_key(), renderer_settings_create);
    component_fabric_register_component(this, box_renderer_component_key(), box_renderer_create);
    component_fabric_register_component(this, box_light_component_key(), box_light_create);
    component_fabric_register_component(this, controller_component_key(), controller_create);
    component_fabric_register_component(this, keyboard_controller_component_key(), keyboard_controller_create);
    component_fabric_register_component(this, printer_component_key(), printer_create);
    component_fabric_register_component(this, suicidal_component_key(), suicidal_create);
    component_fabric_register_component(this, armageddon_component_key(), armageddon_create);
}

struct component_fabric* component_fabric_create() {
    logger_info("Component fabric is creating...");
    struct component_fabric* this = malloc(sizeof(struct component_fabric));
    this->types = string_dictionary_build(10);
    component_fabric_register_all(this);
    return this;
}
void component_fabric_destroy(struct component_fabric *this) {
    logger_info("Component fabric is destroying...");
    dictionary_destroy(this->types);
    free(this);
}

struct component* component_fabric_try_produce_component(const struct component_fabric *this, const char *component_key, struct parser *parser, struct entity *parent) {
    struct component*(*constructor)(struct parser *parser, struct entity *parent) = dictionary_get(this->types, (void*)component_key);
    if (constructor) {
        logger_debug("Component fabric is producing component %s...", component_key);
        return constructor(parser, parent);
    }
    logger_warn("Component fabric failed to find component %s", component_key);
    return NULL;
}