//
// Created by nikita on 24.09.2026.
//

#include "interpreter_renderer_layer_manager_new_layer.h"

#include <stdint.h>
#include <stdlib.h>

#include "../interpreter_command_internal.h"
#include "../../engine/engine.h"
#include "../../engine/engine_execution_context.h"
#include "../../utils/parser.h"
#include "../../rendering/renderer_layer_manager.h"

struct interpreter_renderer_layer_manager_new_layer {
    struct interpreter_command base;
};

static const char* interpreter_renderer_layer_manager_new_layer_get_key(const struct interpreter_command *this) {
    return "new_layer";
}

static void interpreter_renderer_layer_manager_new_layer_execute(const struct interpreter_command *this, struct parser *parser, struct engine *engine) {
    char* layer_name = parser_next_dup(parser);
    uint8_t layer_priority;
    parser_next_uint8(parser, &layer_priority);

    struct renderer_layer *layer = renderer_layer_create(layer_name, layer_priority);

    const struct renderer_layer_manager *manager = engine_execution_context_get_renderer_layer_manager(engine_get_execution_context(engine));
    renderer_layer_manager_capture(manager, layer);
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