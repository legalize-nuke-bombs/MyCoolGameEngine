//
// Created by nikita on 03.10.2026.
//

#include "component_factory.h"

#include <stddef.h>

#include "../../utils/factory.h"
#include "core/camera.h"
#include "core/idle.h"
#include "core/simulator.h"
#include "movement/controller.h"
#include "movement/keyboard_controller.h"
#include "physics/collider.h"
#include "physics/keyboard_rigid_controller.h"
#include "physics/rigid_body.h"
#include "physics/rigid_surface.h"
#include "rendering/box_light.h"
#include "rendering/box_renderer.h"
#include "rendering/box_renderer_animated.h"
#include "rendering/renderer_settings.h"

static struct factory component_factory;

void component_factory_create(void) {
    component_factory = factory_create("component_factory");

    component_factory_register(idle_component_key(), idle_create);
    component_factory_register(camera_component_key(), camera_create);
    component_factory_register(renderer_settings_component_key(), renderer_settings_create);
    component_factory_register(box_renderer_component_key(), box_renderer_create);
    component_factory_register(box_renderer_animated_component_key(), box_renderer_animated_create);
    component_factory_register(box_light_component_key(), box_light_create);
    component_factory_register(controller_component_key(), controller_create);
    component_factory_register(keyboard_controller_component_key(), keyboard_controller_create);
    component_factory_register(simulator_component_key(), simulator_create);
    component_factory_register(rigid_surface_component_key(), rigid_surface_create);
    component_factory_register(collider_component_key(), collider_create);
    component_factory_register(rigid_body_component_key(), rigid_body_create);
    component_factory_register(keyboard_rigid_controller_component_key(), keyboard_rigid_controller_create);
}
void component_factory_destroy(void) {
    factory_destroy(&component_factory);
}

void component_factory_register(const char *key, struct component* (*create)(struct parser *parser, struct entity *parent)) {
    factory_register(&component_factory, key, create);
}

struct component* component_factory_produce(const char* key, struct parser *parser, struct entity *parent) {
    struct component*(*create)(struct parser *parser, struct entity *parent) = factory_find(&component_factory, key);
    if (create) {
        return create(parser, parent);
    }
    return NULL;
}
