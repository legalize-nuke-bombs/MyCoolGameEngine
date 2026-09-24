//
// Created by nikita on 24.09.2026.
//

#include "interpreter_scene_new.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../../engine/engine.h"
#include "../interpreter_command_internal.h"
#include "../../utils/parser.h"
#include "../../logging/logger.h"
#include "../../scene/scene.h"


struct interpreter_scene_new {
    struct interpreter_command base;
};

static const char* interpreter_scene_new_get_key(const struct interpreter_command *this) {
    return "new";
}

static void interpreter_scene_new_execute(const struct interpreter_command *this, struct parser *parser, struct engine *engine) {
    const char* token_name = parser_next(parser);
    if (token_name == NULL) {
        logger_warn("Interpreter failed to find fist arg (scene name) for scene new");
        return;
    }
    char* name = strdup(token_name);

    struct scene *scene = scene_create(name, engine);
    engine_capture_scene(engine, scene);
}

static const struct interpreter_command_vtable window_new_vtable = {
    .key = interpreter_scene_new_get_key,
    .execute = interpreter_scene_new_execute,
    .on_destroy = NULL
};

struct interpreter_command* interpreter_scene_new_create() {
    struct interpreter_scene_new *this = malloc(sizeof(struct interpreter_scene_new));
    this->base.vtable = &window_new_vtable;
    return (struct interpreter_command*)this;
}