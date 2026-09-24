//
// Created by nikita on 24.09.2026.
//

#include "engine.h"
#include "../logging/logger.h"
#include <SDL3/SDL.h>
#include "../rendering/renderer_pipeline.h"
#include "../interpreter/interpreter.h"
#include "../rendering/renderer_layer_manager.h"

#include "../scene/scene.h"
#include "../scene/entity.h"
#include "../scene/components/component.h"
#include "../scene/components/custom/box_renderer.h"
#include "../scene/components/custom/camera.h"
#include "../scene/components/custom/printer.h"
#include "../scene/components/transform.h"
#include "../utils/colors.h"

struct engine {
    struct renderer_layer_manager *renderer_layer_manager;
    SDL_Window *window;
    SDL_Renderer *renderer;
    struct renderer_pipeline *renderer_pipeline;

    struct interpreter *interpreter;
    struct scene *scene;
};

struct engine* engine_create() {
    logger_init(LOGGER_LEVEL_DEBUG);
    logger_info("Engine is creating...");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        logger_error("SDL_Init failed: %s", SDL_GetError());
        return NULL;
    }
    struct engine *this = malloc(sizeof(struct engine));
    this->renderer_layer_manager = renderer_layer_manager_create();
    this->window = NULL;
    this->renderer = NULL;
    this->renderer_pipeline = NULL;

    this->interpreter = interpreter_create(this);
    this->scene = NULL;

    return this;
}
void engine_destroy(struct engine *this) {
    logger_info("Engine is destroying...");

    if (this->scene != NULL) {
        scene_destroy(this->scene);
    }
    if (this->interpreter != NULL) {
        interpreter_destroy(this->interpreter);
    }

    if (this->renderer_pipeline != NULL) {
        renderer_pipeline_destroy(this->renderer_pipeline);
    }
    if (this->renderer != NULL) {
        SDL_DestroyRenderer(this->renderer);
    }
    if (this->window != NULL) {
        SDL_DestroyWindow(this->window);
    }
    SDL_Quit();
    if (this->renderer_layer_manager != NULL) {
        renderer_layer_manager_destroy(this->renderer_layer_manager);
    }

    free(this);
}

void engine_execute(struct engine *this, const char *script_path) {
    if (script_path == NULL) {
        logger_warn("Engine will not start: script is not set");
        return;
    }

    logger_info("Engine is executing script %s...", script_path);
    logger_info("Interpreter finished with exit code %d", interpreter_eval(this->interpreter, script_path));

    char* scene_name = strdup("My scene");
    engine_capture_scene(this, scene_create(scene_name, this));

    struct entity* entity = entity_create("My favourite entity", this->scene);

    struct printer* printer = printer_create(entity, "hi", "bye", 1);
    struct transform* transform = transform_create(entity, &vector2_zero, &vector2_one);
    struct camera* camera = camera_create(entity);
    struct box_renderer* box_renderer = box_renderer_create(entity, color_blue);
    component_set_local_scale(box_renderer_as_component(box_renderer), &(struct vector2){
                                  .x = 100,
                                  .y = 100
                              });

    entity_capture_component(entity, transform_as_component(transform));
    entity_capture_component(entity, camera_as_component(camera));
    entity_capture_component(entity, printer_as_component(printer));
    entity_capture_component(entity, box_renderer_as_component(box_renderer));

    scene_capture_entity(this->scene, entity);

    scene_awake(this->scene);

    struct update_context update_context = {
        .dt = 0
    };

    Uint64 previous = SDL_GetTicksNS();

    for (long long i = 0; i < 200; i++) {
        const Uint64 now = SDL_GetTicksNS();
        update_context.dt = (double)(now - previous) / 1e9;
        previous = now;

        if (i == 101) {
            entity_mark_destroyed(entity);
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {}

        scene_update(this->scene, &update_context);

        SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
        SDL_RenderClear(this->renderer);
        renderer_pipeline_flush(this->renderer_pipeline);
        SDL_RenderPresent(this->renderer);
    }
}

struct renderer_layer_manager* engine_get_renderer_layer_manager(struct engine *this) {
    return this->renderer_layer_manager;
}
void engine_capture_window(struct engine *this, SDL_Window *window) {
    logger_info("Engine is capturing window...");
    if (this->window != NULL) {
        SDL_DestroyWindow(this->window);
    }
    this->window = window;
}
void engine_capture_renderer(struct engine *this, SDL_Renderer *renderer) {
    logger_info("Engine is capturing renderer...");
    if (this->renderer != NULL) {
        SDL_DestroyRenderer(this->renderer);
    }
    this->renderer = renderer;
    if (this->renderer_pipeline != NULL) {
        renderer_pipeline_destroy(this->renderer_pipeline);
    }
    this->renderer_pipeline = renderer_pipeline_create(renderer);
}
struct renderer_pipeline *engine_get_renderer_pipeline(const struct engine *this) {
    return this->renderer_pipeline;
}

void engine_capture_scene(struct engine *this, struct scene *scene) {
    logger_info("Engine is capturing scene...");
    if (this->scene != NULL) {
        scene_destroy(this->scene);
    }
    this->scene = scene;
}