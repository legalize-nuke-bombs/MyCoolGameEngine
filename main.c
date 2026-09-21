#include <pthread_time.h>

#include "src/core/entity.h"
#include "src/logging/logger.h"
#include "src/core/components/custom/printer.h"
#include "src/core/entity_collection.h"
#include "src/core/scene.h"

#include <unistd.h>

int main(void) {
    logger_init(LOGGER_LEVEL_DEBUG);

    struct printer printer;
    printer_init(&printer, "hi", "bye", 1);

    struct entity entity;
    entity_init(&entity);
    entity_set_name(&entity, "my favourite entity");
    entity_add_component(&entity, printer_as_component(&printer));

    struct entity_collection entity_collection;
    entity_collection_init(&entity_collection);
    entity_collection_add_entity(&entity_collection, &entity);

    struct scene scene;
    scene_init(&scene);
    scene_set_name(&scene, "my scene name");
    scene_set_entity_collection(&scene, &entity_collection);

    struct timespec start, end;
    struct update_context update_context = {
        update_context.dt = 0
    };

    for (long long i = 0; i < 100000000ll; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);

        scene_update(&scene, &update_context);

        clock_gettime(CLOCK_MONOTONIC, &end);

        update_context.dt = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1e9;
    }

    scene_destroy(&scene);

    return 0;
}
