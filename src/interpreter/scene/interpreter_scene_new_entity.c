//
// Created by nikita on 24.09.2026.
//

#include "interpreter_scene_new_entity.h"
#include <stdlib.h>

#include "../../engine/engine.h"
#include "../interpreter_command_internal.h"
#include "../../utils/parser.h"
#include "../../logging/logger.h"
#include "../../scene/entity.h"
#include "../../scene/scene.h"


struct interpreter_scene_new_entity {
    struct interpreter_command base;
};

static const char* interpreter_scene_new_entity_get_key(const struct interpreter_command *this) {
    return "new_entity";
}

static void interpreter_scene_new_entity_execute(const struct interpreter_command *this, struct parser *parser, struct engine *engine) {
    struct scene *scene = engine_get_scene(engine);
    if (scene == NULL) {
        logger_warn("Interpreter failed to execute scene new_entity: scene is not set");
        return;
    }

    char* entity_name = parser_next_dup(parser);

    struct entity *entity = entity_create(entity_name, scene);
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