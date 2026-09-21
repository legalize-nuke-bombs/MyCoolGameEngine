#include "src/core/entity.h"
#include "src/logging/logger.h"
#include "src/core/components/custom/printer.h"
#include "src/core/entity_collection.h"

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
    entity_collection_print(&entity_collection);
    entity_collection_destroy(&entity_collection);

    return 0;
}
