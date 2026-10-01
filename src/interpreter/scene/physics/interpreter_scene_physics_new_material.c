//
// Created by nikita on 01.10.2026.
//

#include "interpreter_scene_physics_new_material.h"

#include <stdint.h>
#include <stdlib.h>

#include "../../interpreter_command_internal.h"
#include "../../../scene/scene.h"
#include "../../../scene/physics/physics.h"
#include "../../../scene/physics/rigid_material.h"
#include "../../../scene/physics/rigid_materials.h"
#include "../../../utils/parser.h"
#include "../../../subsystems/subsystem_collection.h"

struct interpreter_scene_physics_new_material {
    struct interpreter_command base;
};

static const char* interpreter_scene_physics_new_material_get_key(const struct interpreter_command *this) {
    return "new_material";
}

static void interpreter_scene_physics_new_material_execute(const struct interpreter_command *this, struct parser *parser, const struct subsystem_collection *subsystems) {
    char* name = parser_next_dup(parser);
    double frication;
    parser_next_double(parser, &frication);

    struct rigid_material *material = rigid_material_create(name, frication);

    const struct rigid_materials* materials = physics_get_materials(scene_get_physics((struct scene*)subsystem_collection_get(subsystems, "scene")));
    rigid_materials_capture(materials, material);
}

static const struct interpreter_command_vtable scene_physics_new_material_vtable = {
    .key = interpreter_scene_physics_new_material_get_key,
    .execute = interpreter_scene_physics_new_material_execute
};

struct interpreter_command* interpreter_scene_physics_new_material_create() {
    struct interpreter_scene_physics_new_material *this = malloc(sizeof(struct interpreter_scene_physics_new_material));
    this->base.vtable = &scene_physics_new_material_vtable;
    return (struct interpreter_command*)this;
}