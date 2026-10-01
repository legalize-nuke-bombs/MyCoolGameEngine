//
// Created by nikita on 01.10.2026.
//

#include "interpreter_scene_physics_new_layer.h"

#include <stdint.h>
#include <stdlib.h>

#include "../../interpreter_command_internal.h"
#include "../../../scene/scene.h"
#include "../../../scene/physics/physics.h"
#include "../../../scene/physics/rigid_layer.h"
#include "../../../scene/physics/rigid_layers.h"
#include "../../../utils/parser.h"
#include "../../../subsystems/subsystem_collection.h"

struct interpreter_scene_physics_new_layer {
    struct interpreter_command base;
};

static const char* interpreter_scene_physics_new_layer_get_key(const struct interpreter_command *this) {
    return "new_layer";
}

static void interpreter_scene_physics_new_layer_execute(const struct interpreter_command *this, struct parser *parser, const struct subsystem_collection *subsystems) {
    char* name = parser_next_dup(parser);
    uint8_t priority;
    parser_next_uint8(parser, &priority);

    struct rigid_layer *layer = rigid_layer_create(name, priority);

    const struct rigid_layers* layers = physics_get_layers(scene_get_physics((struct scene*)subsystem_collection_get(subsystems, "scene")));
    rigid_layers_capture(layers, layer);
}

static const struct interpreter_command_vtable scene_physics_new_layer_vtable = {
    .key = interpreter_scene_physics_new_layer_get_key,
    .execute = interpreter_scene_physics_new_layer_execute
};

struct interpreter_command* interpreter_scene_physics_new_layer_create() {
    struct interpreter_scene_physics_new_layer *this = malloc(sizeof(struct interpreter_scene_physics_new_layer));
    this->base.vtable = &scene_physics_new_layer_vtable;
    return (struct interpreter_command*)this;
}