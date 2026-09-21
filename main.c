#include "src/core/entity.h"
#include "src/logging/logger.h"
#include "src/core/components/custom/printer.h"

int main(void) {
    logger_init(LOGGER_LEVEL_DEBUG);

    struct printer printer;
    printer_init(&printer, "hi", "bye", 1);

    struct entity firstEntity;
    entity_init(&firstEntity);
    entity_set_name(&firstEntity, "my favourite entity");
    entity_add_component(&firstEntity, printer_as_component(&printer));

    entity_print(&firstEntity);

    struct update_context context = {
        .dt = 10
    };
    entity_update(&firstEntity, &context);

    entity_destroy(&firstEntity);

    return 0;
}
