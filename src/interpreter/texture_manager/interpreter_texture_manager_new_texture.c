//
// Created by nikita on 24.09.2026.
//

#include "interpreter_texture_manager_new_texture.h"

#include <stdlib.h>
#include <string.h>

#include "../interpreter_command_internal.h"
#include "../../logging/logger.h"
#include "../../rendering/renderer.h"
#include "../../utils/parser.h"
#include "../../rendering/textures/texture_manager.h"
#include "../../rendering/textures/texture.h"
#include "../../subsystems/subsystem_collection.h"

struct interpreter_texture_manager_new_texture {
    struct interpreter_command base;
};

static const char* interpreter_texture_manager_new_texture_get_key(const struct interpreter_command *this) {
    return "new_texture";
}

static void interpreter_texture_manager_new_texture_execute(const struct interpreter_command *this, struct parser *parser, const struct subsystem_collection *subsystems) {
    const struct renderer* renderer = (struct renderer*)subsystem_collection_get(subsystems, "renderer");
    struct SDL_Renderer* native_renderer = renderer_get_native_renderer(renderer);
    struct texture_manager *manager = renderer_get_texture_manager(renderer);

    char* texture_id = parser_next_dup(parser);
    int texture_tile_w, texture_tile_h;
    parser_next_int(parser, &texture_tile_w);
    parser_next_int(parser, &texture_tile_h);
    char* texture_path = parser_next_dup(parser);
    char* texture_loading_mode = parser_next_dup(parser);

    enum texture_loading_mode texture_loading_mode_enum;
    if (strcmp(texture_loading_mode, "eager") == 0) {
        texture_loading_mode_enum = texture_loading_mode_eager;
    }
    else if (strcmp(texture_loading_mode, "lazy") == 0) {
        texture_loading_mode_enum = texture_loading_mode_lazy;
    }
    else {
        logger_warn("Unexpected texture loading mode `%s`, `lazy` will be used instead", texture_loading_mode);
        texture_loading_mode_enum = texture_loading_mode_lazy;

    }

    free(texture_loading_mode);

    texture_manager_capture(manager,
            texture_create(texture_id, texture_path, texture_tile_w, texture_tile_h, texture_loading_mode_enum, native_renderer)
    );
}

static const struct interpreter_command_vtable texture_manager_new_texture_vtable = {
    .key = interpreter_texture_manager_new_texture_get_key,
    .execute = interpreter_texture_manager_new_texture_execute,
    .on_destroy = NULL
};

struct interpreter_command* interpreter_texture_manager_new_texture_create() {
    struct interpreter_texture_manager_new_texture *this = calloc(1, sizeof(struct interpreter_texture_manager_new_texture));
    this->base.vtable = &texture_manager_new_texture_vtable;
    return (struct interpreter_command*)this;
}