//
// Created by nikita on 03.10.2026.
//

#include "component_factory.h"

#include <stdlib.h>

#include "component_internal.h"
#include "../../utils/factory.h"
#include "../../utils/fields.h"
#include "audio/audio_listener.h"
#include "audio/audio_source.h"
#include "core/camera.h"
#include "custom/camera_wheel_zoom.h"
#include "core/idle.h"
#include "core/simulator.h"
#include "mcge/logging/logger.h"
#include "movement/controller.h"
#include "movement/keyboard_controller.h"
#include "physics/collider.h"
#include "physics/keyboard_rigid_controller.h"
#include "physics/rigid_body.h"
#include "physics/rigid_surface.h"
#include "rendering/box_light.h"
#include "rendering/box_renderer.h"
#include "rendering/box_renderer_animated.h"
#include "rendering/box_renderer_rigid_directed.h"
#include "rendering/renderer_settings.h"
#include "custom/clickable.h"
#include "custom/fullscreen_toggler.h"
#include "mcge/allocator/allocator.h"
#include "ui/hud.h"
#include "lifecycle/game_closer_on_click.h"
#include "lifecycle/scene_switcher_on_key_pressed.h"
#include "lifecycle/scene_switcher_on_click.h"
#include "lifecycle/scene_switcher_on_start.h"

static struct factory component_factory;

void component_factory_create(void) {
    component_factory = factory_create("component_factory");

    component_factory_register(&idle_vtable);
    component_factory_register(&camera_vtable);
    component_factory_register(&renderer_settings_vtable);
    component_factory_register(&box_renderer_vtable);
    component_factory_register(&box_renderer_animated_vtable);
    component_factory_register(&box_light_vtable);
    component_factory_register(&controller_vtable);
    component_factory_register(&keyboard_controller_vtable);
    component_factory_register(&simulator_vtable);
    component_factory_register(&rigid_surface_vtable);
    component_factory_register(&collider_vtable);
    component_factory_register(&rigid_body_vtable);
    component_factory_register(&box_renderer_rigid_directed_vtable);
    component_factory_register(&keyboard_rigid_controller_vtable);
    component_factory_register(&audio_listener_vtable);
    component_factory_register(&audio_source_vtable);
    component_factory_register(&hud_vtable);
    component_factory_register(&clickable_vtable);
    component_factory_register(&camera_wheel_zoom_vtable);
    component_factory_register(&fullscreen_toggler_vtable);
    component_factory_register(&game_closer_on_click_vtable);
    component_factory_register(&scene_switcher_on_key_pressed_vtable);
    component_factory_register(&scene_switcher_on_click_vtable);
    component_factory_register(&scene_switcher_on_start_vtable);
}
void component_factory_destroy(void) {
    factory_destroy(&component_factory);
}

void component_factory_register(const struct component_vtable *vtable) {
    factory_register(&component_factory, vtable->component_key(), (void*)vtable);
}

struct component* component_factory_produce(const char* key, struct fields *fields, struct entity *parent) {
    const struct component_vtable *vtable = factory_find(&component_factory, key);
    if (vtable == NULL) {
        return NULL;
    }
    struct component *component = allocator_new0(1, vtable->size);
    component_base_create(component, vtable, parent);
    if (vtable->on_create) {
        vtable->on_create(component, fields);
    }
    fields_warn_unknown(fields);
    return component;
}
