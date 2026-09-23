//
// Created by nikita on 24.09.2026.
//

#include "engine.h"
#include "../logging/logger.h"
#include <SDL3/SDL.h>
#include "../rendering/renderer_pipeline.h"
#include "../interpreter/interpreter.h"

#include "../scene/scene.h"
#include "../scene/entity.h"
#include "../scene/components/component.h"
#include "../scene/components/custom/box_renderer.h"
#include "../scene/components/custom/camera.h"
#include "../scene/components/custom/printer.h"
#include "../scene/components/transform.h"
#include "../utils/colors.h"

struct engine {
    bool running;

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
    this->running = false;
    this->window = NULL;
    this->renderer = NULL;
    if (!SDL_CreateWindowAndRenderer("MyCoolGameEngine", 800, 600, SDL_WINDOW_RESIZABLE, &this->window, &this->renderer)) {
        logger_error("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
        SDL_Quit();
        free(this);
        return NULL;
    }
    SDL_SetRenderVSync(this->renderer, 1);
    SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);

    this->renderer_pipeline = renderer_pipeline_create(this->renderer);
    this->interpreter = interpreter_create(this);

    this->scene = NULL;

    return this;
}
void engine_destroy(struct engine *this) {
    logger_info("Engine is destroying...");

    interpreter_destroy(this->interpreter);
    renderer_pipeline_destroy(this->renderer_pipeline);

    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();

    free(this);
}

void engine_run(struct engine *this, const char *script_path) {
    if (this->running) {
        logger_warn("Engine failed to start: already running");
        return;
    }
    if (script_path == NULL) {
        logger_warn("Engine will not start: script is not set");
        return;
    }

    logger_info("Engine is starting using script %s...", script_path);
    this->running = true;

    logger_info("Interpreter finished with exit code %d", interpreter_eval(this->interpreter, script_path));

    this->scene = scene_create("My scene", this);

    struct entity* entity = entity_create(this->scene);
    entity_set_name(entity, "My favourite entity");

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

    this->running = false;
    if (this->scene != NULL) {
        scene_destroy(this->scene);
    }
    this->scene = NULL;
}

struct renderer_pipeline *engine_get_renderer_pipeline(const struct engine *this) {
    return this->renderer_pipeline;
}