#include <SDL3/SDL.h>

#include "src/core/entity.h"
#include "src/logging/logger.h"
#include "src/core/components/custom/printer.h"
#include "src/core/components/transform/transform.h"
#include "src/core/scene.h"
#include "src/core/components/rendering/camera.h"

int main(void) {
    logger_init(LOGGER_LEVEL_DEBUG);

    struct printer* printer = printer_create("hi", "bye", 1);

    struct transform* transform = transform_create(&vector2_zero, &vector2_one);

    struct camera* camera = camera_create();

    struct entity* entity = entity_create();
    entity_set_name(entity, "My favourite entity");
    entity_capture_component(entity, transform_as_component(transform));
    entity_capture_component(entity, camera_as_component(camera));

    struct scene* scene = scene_create();
    scene_set_name(scene, "My scene name");
    scene_capture_entity(scene, entity);

    scene_awake(scene);

    // dynamic!
    entity_capture_component(entity, printer_as_component(printer));

    struct update_context update_context = {
        .dt = 0
    };

    Uint64 previous = SDL_GetTicksNS();

    for (long long i = 0; i < 100000000ll; i++) {
        const Uint64 now = SDL_GetTicksNS();

        update_context.dt = (double)(now - previous) / 1e9;
        previous = now;

        scene_update(scene, &update_context);
    }

    scene_destroy(scene);

    return 0;
}
