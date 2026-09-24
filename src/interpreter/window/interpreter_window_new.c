//
// Created by nikita on 24.09.2026.
//

#include "interpreter_window_new.h"
#include "../interpreter_command.h"
#include "../interpreter_command_internal.h"
#include <stdlib.h>
#include <string.h>

#include "../../utils/parser.h"
#include "../../logging/logger.h"
#include <SDL3/SDL.h>

#include "../../engine/engine.h"


struct interpreter_window_new {
    struct interpreter_command base;
};

static const char* interpreter_window_new_get_key(const struct interpreter_command *this) {
    return "new";
}

static void interpreter_window_new_execute(const struct interpreter_command *this, struct parser *parser, struct engine *engine) {
    const char* token_name = parser_next(parser);
    if (token_name == NULL) {
        logger_warn("Interpreter failed to find fist arg (window name) for window new");
        return;
    }
    char* name = strdup(token_name);

    const char* token_width = parser_next(parser);
    const char* token_height = parser_next(parser);
    if (token_width == NULL || token_height == NULL) {
        logger_warn("Interpreter failed to find second and third args (window size) for window new");
        free(name);
        return;
    }

    char* end_ptr_w = NULL;
    char* end_ptr_h = NULL;
    int width = strtol(token_width, &end_ptr_w, 10);
    int height = strtol(token_height, &end_ptr_h, 10);
    if (*end_ptr_w != '\0' || end_ptr_w == token_width || *end_ptr_h != '\0' || end_ptr_h == token_height) {
        logger_warn("Interpreter failed to cast second and third args (window size) for window new to integer");
        free(name);
        return;
    }

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    if (SDL_CreateWindowAndRenderer(name, width, height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        engine_capture_window(engine, window);
        engine_capture_renderer(engine, renderer);
        SDL_SetRenderVSync(renderer, 1);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    }
    else {
        logger_error("Interpreter failed to create window: %s", SDL_GetError());
    }

    free(name);
}

static const struct interpreter_command_vtable renderer_layer_manager_new_layer_vtable = {
    .key = interpreter_window_new_get_key,
    .execute = interpreter_window_new_execute,
    .on_destroy = NULL
};

struct interpreter_command* interpreter_window_new_create() {
    struct interpreter_window_new *this = malloc(sizeof(struct interpreter_window_new));
    this->base.vtable = &renderer_layer_manager_new_layer_vtable;
    return (struct interpreter_command*)this;
}