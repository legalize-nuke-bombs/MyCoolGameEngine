//
// Created by nikita on 24.09.2026.
//

#include "interpreter_renderer_layer_manager_new_layer.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../interpreter_command_internal.h"
#include "../../engine/engine.h"
#include "../../utils/parser.h"
#include "../../logging/logger.h"
#include "../../rendering/renderer_layer_manager.h"

struct interpreter_renderer_layer_manager_new_layer {
    struct interpreter_command base;
};

static const char* interpreter_renderer_layer_manager_new_layer_get_key(const struct interpreter_command *this) {
    return "new_layer";
}

static void interpreter_renderer_layer_manager_new_layer_execute(const struct interpreter_command *this, struct parser *parser, struct engine *engine) {
    const char* token_name = parser_next(parser);
    if (token_name == NULL) {
        logger_warn("Interpreter failed to find first arg (layer name) for renderer_layer_manager new_layer");
        return;
    }
    char* name = strdup(token_name);

    const char* token_priority = parser_next(parser);
    if (token_priority == NULL) {
        logger_warn("Interpreter failed to find second arg (layer priority) for renderer_layer_manager new_layer");
        free(name);
        return;
    }

    char* end_ptr = NULL;
    uint8_t priority = strtol(token_priority, &end_ptr, 10);
    if (*end_ptr != '\0' || end_ptr == token_priority) {
        logger_warn("Interpreter failed to cast second arg (layer priority) for renderer_layer_manager new_layer to integer");
        free(name);
        return;
    }

    struct renderer_layer *layer = renderer_layer_create(name, priority);

    const struct renderer_layer_manager *manager = engine_get_renderer_layer_manager(engine);
    if (!renderer_layer_manager_try_capture(manager, layer)) {
        renderer_layer_destroy(layer);
    }
}

static const struct interpreter_command_vtable renderer_layer_manager_new_layer_vtable_create = {
    .key = interpreter_renderer_layer_manager_new_layer_get_key,
    .execute = interpreter_renderer_layer_manager_new_layer_execute,
    .on_destroy = NULL
};

struct interpreter_command* interpreter_renderer_layer_manager_new_layer_create() {
    struct interpreter_renderer_layer_manager_new_layer *this = malloc(sizeof(struct interpreter_renderer_layer_manager_new_layer));
    this->base.vtable = &renderer_layer_manager_new_layer_vtable_create;
    return (struct interpreter_command*)this;
}