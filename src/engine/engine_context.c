//
// Created by nikita on 24.09.2026.
//

#include "engine_context.h"

#include <stdlib.h>

struct engine_context {
    struct renderer_pipeline *renderer_pipeline;
};

struct engine_context* engine_context_create(struct renderer_pipeline *renderer_pipeline) {
    struct engine_context *engine_context = malloc(sizeof(struct engine_context));
    engine_context->renderer_pipeline = renderer_pipeline;
    return engine_context;
}
void engine_context_destroy(struct engine_context* this) {
    free(this);
}

struct renderer_pipeline* engine_context_get_renderer_pipeline(const struct engine_context *this) {
    return this->renderer_pipeline;
}