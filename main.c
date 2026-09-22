#include <pthread_time.h>

#include "src/core/entity.h"
#include "src/logging/logger.h"
#include "src/core/components/custom/printer.h"
#include "src/core/components/custom/transform.h"
#include "src/core/entity_collection.h"
#include "src/core/scene.h"

int main(void) {
    logger_init(LOGGER_LEVEL_DEBUG);

    struct printer printer;
    printer_init(&printer, "hi", "bye", 1);

    struct transform transform;
    transform_init(&transform, &vector2_zero, &vector2_one);

    struct entity entity;
    entity_init(&entity);
    entity_set_name(&entity, "my favourite entity");
    entity_capture_component(&entity, transform_as_component(&transform));
    entity_capture_component(&entity, printer_as_component(&printer));

    struct entity_collection entity_collection;
    entity_collection_init(&entity_collection);
    entity_collection_add_entity(&entity_collection, &entity);

    struct scene scene;
    scene_init(&scene);
    scene_set_name(&scene, "my scene name");
    scene_set_entity_collection(&scene, &entity_collection);

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
