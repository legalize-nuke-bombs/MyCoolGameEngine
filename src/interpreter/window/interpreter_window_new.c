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
    char* name = parser_next_dup(parser);
    int width, height;
    parser_next_int(parser, &width);
    parser_next_int(parser, &height);

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    if (SDL_CreateWindowAndRenderer(name, width, height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        engine_capture_window(engine, window);
        engine_capture_renderer(engine, renderer);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    }
    else {
        logger_error("Interpreter failed to create window: %s", SDL_GetError());
    }

    free(name);
}

static const struct interpreter_command_vtable window_new_vtable = {
    .key = interpreter_window_new_get_key,
    .execute = interpreter_window_new_execute,
    .on_destroy = NULL
};

struct interpreter_command* interpreter_window_new_create() {
    struct interpreter_window_new *this = malloc(sizeof(struct interpreter_window_new));
    this->base.vtable = &window_new_vtable;
    return (struct interpreter_command*)this;
}