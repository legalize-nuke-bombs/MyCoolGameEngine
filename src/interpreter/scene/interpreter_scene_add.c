//
// Created by nikita on 24.09.2026.
//

#include "interpreter_scene_add.h"
#include <stdlib.h>
#include <string.h>

#include "../../engine/engine.h"
#include "../interpreter_command_internal.h"
#include "../../logging/logger.h"
#include "../../utils/parser.h"
#include "../../scene/entity.h"
#include "../../scene/scene.h"
#include "../../scene/components/component_fabric.h"
#include "../../scene/prefabs/prefab.h"
#include "../../scene/prefabs/prefab_manager.h"
#include "../../subsystems/subsystem_collection.h"


struct interpreter_scene_add {
    struct interpreter_command base;
};

static const char* interpreter_scene_add_get_key(const struct interpreter_command *this) {
    return "add";
}

static struct entity* interpreter_scene_parse_entity(struct parser *parser, struct scene *scene, const struct component_fabric *component_fabric, char* entity_name) {
    struct entity *entity = entity_create(entity_name, NULL, scene);

    for (;;) {
        const char* word = parser_next(parser);
        if (word == NULL || strcmp(word, "end") == 0) {
            break;
        }

        if (strcmp(word, "child") == 0 || strcmp(word, "entity") == 0) {
            struct entity *child = interpreter_scene_parse_entity(parser, scene, component_fabric, parser_next_dup(parser));
            if (child != NULL) {
                entity_capture_entity(entity, child);
            }
            continue;
        }

        struct component* component = component_fabric_try_produce_component(component_fabric, word, parser, entity);
        if (component != NULL) {
            entity_capture_component(entity, component);
        }
    }

    return entity;
}

static void interpreter_scene_add_execute(const struct interpreter_command *this, struct parser *parser, const struct subsystem_collection *subsystems) {
    struct scene *scene = (struct scene*)subsystem_collection_get(subsystems, "scene");
    const struct component_fabric *component_fabric = scene_get_component_fabric(scene);

    const char* type = parser_next(parser);
    if (type == NULL) return;

    if (strcmp(type, "prefab") == 0) {
        char* prefab_name = parser_next_dup(parser);
        struct entity *root_entity = interpreter_scene_parse_entity(parser, scene, component_fabric, strdup(prefab_name));

        prefab_manager_capture_prefab(
            scene_get_prefab_manager(scene),
            prefab_create(prefab_name, root_entity)
        );
    }
    else {
        if (strcmp(type, "entity") != 0) {
            logger_warn("Interpreter scene add unexpected type `%s`, parsing it as entity", type);
        }
        struct entity *root_entity = interpreter_scene_parse_entity(parser, scene, component_fabric, parser_next_dup(parser));
        if (root_entity != NULL) {
            scene_capture_entity(scene, root_entity);
        }
    }
}


static const struct interpreter_command_vtable scene_add_vtable = {
    .key = interpreter_scene_add_get_key,
    .execute = interpreter_scene_add_execute,
    .on_destroy = NULL
};

struct interpreter_command* interpreter_scene_add_create() {
    struct interpreter_scene_add *this = malloc(sizeof(struct interpreter_scene_add));
    this->base.vtable = &scene_add_vtable;
    return (struct interpreter_command*)this;
}