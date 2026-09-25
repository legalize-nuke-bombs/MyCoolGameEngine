//
// Created by nikita on 24.09.2026.
//

#include "interpreter_scene_new_entity.h"
#include <stdlib.h>
#include <string.h>

#include "../../engine/engine.h"
#include "../interpreter_command_internal.h"
#include "../../utils/parser.h"
#include "../../scene/entity.h"
#include "../../scene/scene.h"
#include "../../scene/components/component_fabric.h"
#include "../../subsystems/subsystem_collection.h"


struct interpreter_scene_new_entity {
    struct interpreter_command base;
};

static const char* interpreter_scene_new_entity_get_key(const struct interpreter_command *this) {
    return "new_entity";
}

static void interpreter_scene_new_entity_execute(const struct interpreter_command *this, struct parser *parser, const struct subsystem_collection *subsystems) {
    struct scene *scene = (struct scene*)subsystem_collection_get(subsystems, "scene");

    char* entity_name = parser_next_dup(parser);

    struct entity *entity = entity_create(entity_name, scene);

    const struct component_fabric *component_fabric = scene_get_component_fabric(scene);
    for (; ;) {
        const char* word = parser_next(parser);
        if (word == NULL || strcmp(word, "end") == 0) {
            break;
        }
        struct component* component = component_fabric_try_produce_component(component_fabric, word, parser, entity);
        if (component == NULL) {
            continue;
        }
        entity_capture_component(entity, component);
    }

    scene_capture_entity(scene, entity);
}

static const struct interpreter_command_vtable scene_new_entity_vtable = {
    .key = interpreter_scene_new_entity_get_key,
    .execute = interpreter_scene_new_entity_execute,
    .on_destroy = NULL
};

struct interpreter_command* interpreter_scene_new_entity_create() {
    struct interpreter_scene_new_entity *this = malloc(sizeof(struct interpreter_scene_new_entity));
    this->base.vtable = &scene_new_entity_vtable;
    return (struct interpreter_command*)this;
}