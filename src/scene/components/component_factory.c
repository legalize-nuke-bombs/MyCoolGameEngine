//
// Created by nikita on 03.10.2026.
//

#include "component_factory.h"

#include <stdlib.h>

#include "../../factories/factory_internal.h"
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
#include "../../demo/special/pulsator.h"
#include "../../demo/world/forest.h"
#include "../../demo/scenes/scene_switcher.h"
#include "../../demo/scenes/game_closer.h"
#include "../../demo/enviornment/clock.h"
#include "../../demo/enviornment/sky.h"
#include "../../demo/characters/player.h"
#include "../../demo/characters/bt/behaviour_agent.h"
#include "../../demo/characters/mana.h"
#include "../../demo/characters/skills/skilled.h"
#include "../../demo/characters/skills/custom/fear_balls/fear_ball.h"

struct component_factory {
    struct factory base;
};

static const struct factory_vtable component_factory_vtable = {
    .key = "component_factory"
};

static void component_factory_register_all(const struct factory *base) {
    // Core components
    factory_register(base, idle_component_key(), idle_create);
    factory_register(base, camera_component_key(), camera_create);
    factory_register(base, renderer_settings_component_key(), renderer_settings_create);
    factory_register(base, box_renderer_component_key(), box_renderer_create);
    factory_register(base, box_renderer_animated_component_key(), box_renderer_animated_create);
    factory_register(base, box_light_component_key(), box_light_create);
    factory_register(base, controller_component_key(), controller_create);
    factory_register(base, keyboard_controller_component_key(), keyboard_controller_create);
    factory_register(base, simulator_component_key(), simulator_create);
    factory_register(base, rigid_surface_component_key(), rigid_surface_create);
    factory_register(base, collider_component_key(), collider_create);
    factory_register(base, rigid_body_component_key(), rigid_body_create);
    factory_register(base, keyboard_rigid_controller_component_key(), keyboard_rigid_controller_create);

    // Demo components
    factory_register(base, pulsator_component_key(), pulsator_create);
    factory_register(base, forest_component_key(), forest_create);
    factory_register(base, scene_switcher_component_key(), scene_switcher_create);
    factory_register(base, game_closer_component_key(), game_closer_create);
    factory_register(base, clock_component_key(), clock_create);
    factory_register(base, sky_component_key(), sky_create);
    factory_register(base, player_component_key(), player_create);
    factory_register(base, behaviour_agent_component_key(), behaviour_agent_create);
    factory_register(base, mana_component_key(), mana_create);
    factory_register(base, skilled_component_key(), skilled_create);
    factory_register(base, fear_ball_component_key(), fear_ball_create);
}

struct factory* component_factory_create() {
    struct component_factory* this = calloc(1, sizeof(struct component_factory));
    struct factory* base = (struct factory*)this;
    factory_base_create(base, &component_factory_vtable);
    component_factory_register_all(base);
    return base;
}

struct component* component_factory_produce(const struct component_factory *this, const char* key, struct parser *parser, struct entity *parent) {
    const struct factory* base = (struct factory*)this;
    struct component*(*constructor)(struct parser *parser, struct entity *parent) = factory_get_constructor(base, key);
    if (constructor) {
        return constructor(parser, parent);
    }
    return NULL;
}