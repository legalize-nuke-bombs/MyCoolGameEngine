#include <SDL3/SDL.h>

#include "src/scene/entity.h"
#include "src/logging/logger.h"
#include "src/scene/components/custom/printer.h"
#include "src/scene/components/transform.h"
#include "src/scene/scene.h"
#include "src/scene/components/custom/camera.h"
#include "src/engine/engine_context.h"
#include "src/rendering/renderer_pipeline.h"
#include "src/scene/components/custom/box_renderer.h"
#include "src/utils/colors.h"

int main(void) {
    logger_init(LOGGER_LEVEL_DEBUG);


    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (!SDL_CreateWindowAndRenderer("MyCoolGameEngine", 1280, 720, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_SetRenderVSync(renderer, 1);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    struct renderer_pipeline *renderer_pipeline = renderer_pipeline_create(renderer);

    const struct engine_context engine_context = {
        .renderer_pipeline = renderer_pipeline
    };

    struct scene* scene = scene_create(&engine_context);
    scene_set_name(scene, "My scene name");

    struct entity* entity = entity_create(scene);
    entity_set_name(entity, "My favourite entity");

    struct printer* printer = printer_create(entity, "hi", "bye", 1);
    struct transform* transform = transform_create(entity, &vector2_zero, &vector2_one);
    struct camera* camera = camera_create(entity);
    component_set_local_scale(camera_as_component(camera), &(struct vector2){
                                  .x = 1280,
                                  .y = 720
                              });
    struct box_renderer* box_renderer = box_renderer_create(entity, color_blue);
    component_set_local_scale(box_renderer_as_component(box_renderer), &(struct vector2){
                                  .x = 100,
                                  .y = 100
                              });

    entity_capture_component(entity, transform_as_component(transform));
    entity_capture_component(entity, camera_as_component(camera));
    entity_capture_component(entity, printer_as_component(printer));
    entity_capture_component(entity, box_renderer_as_component(box_renderer));

    scene_capture_entity(scene, entity);

    scene_awake(scene);

    struct update_context update_context = {
        .dt = 0
    };

    Uint64 previous = SDL_GetTicksNS();

    for (long long i = 0; i < 120; i++) {
        const Uint64 now = SDL_GetTicksNS();
        update_context.dt = (double)(now - previous) / 1e9;
        previous = now;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {}

        scene_update(scene, &update_context);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        renderer_pipeline_flush(renderer_pipeline);
        SDL_RenderPresent(renderer);
    }

    scene_destroy(scene);
    renderer_pipeline_destroy(renderer_pipeline);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
