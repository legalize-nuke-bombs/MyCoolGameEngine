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
#include "../../scene/entity_parser.h"
#include "../../scene/scene.h"
#include "../../subsystems/subsystem_collection.h"


struct interpreter_scene_add {
    struct interpreter_command base;
};

static const char* interpreter_scene_add_get_key(const struct interpreter_command *this) {
    return "add";
}

static void interpreter_scene_add_execute(const struct interpreter_command *this, struct parser *parser, const struct subsystem_collection *subsystems) {
    struct scene *scene = (struct scene*)subsystem_collection_get(subsystems, "scene");

    const char* type = parser_next(parser);
    if (type == NULL) return;

    if (strcmp(type, "entity") != 0) {
        logger_warn("Interpreter scene add unexpected type `%s`, parsing it as entity", type);
    }
    struct entity *root_entity = entity_parse(parser, scene, parser_next_dup(parser));
    if (root_entity != NULL) {
        scene_capture_entity(scene, root_entity);
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