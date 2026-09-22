#include <pthread_time.h>
#include <stdlib.h>

#include "src/core/entity.h"
#include "src/logging/logger.h"
#include "src/core/components/custom/printer.h"
#include "src/core/components/custom/transform.h"
#include "src/core/scene.h"

int main(void) {
    logger_init(LOGGER_LEVEL_DEBUG);

    struct printer* printer = malloc(sizeof(struct printer));
    printer_init(printer, "hi", "bye", 1);

    struct transform* transform = malloc(sizeof(struct transform));
    transform_init(transform, &vector2_zero, &vector2_one);

    struct entity* entity = malloc(sizeof(struct entity));
    entity_init(entity);
    entity_set_name(entity, "My favourite entity");
    entity_capture_component(entity, transform_as_component(transform));

    struct scene scene;
    scene_init(&scene);
    scene_set_name(&scene, "My scene name");
    scene_capture_entity(&scene, entity);

    // dynamic!
    entity_capture_component(entity, printer_as_component(printer));

    struct timespec previous, now;
    struct update_context update_context = {
        .dt = 0
    };

    clock_gettime(CLOCK_MONOTONIC, &previous);

    for (long long i = 0; i < 100000000ll; i++) {
        clock_gettime(CLOCK_MONOTONIC, &now);

        update_context.dt = (double)(now.tv_sec - previous.tv_sec) + (double)(now.tv_nsec - previous.tv_nsec) / 1e9;
        previous = now;

        scene_update(&scene, &update_context);
    }

    scene_destroy(&scene);

    return 0;
}
