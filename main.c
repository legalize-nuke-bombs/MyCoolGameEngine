#include "src/core/entity.h"
#include "src/logging/logger.h"

int main(void) {
    logger_init(LOGGER_LEVEL_DEBUG);

    struct entity firstEntity;
    entity_init(&firstEntity);
    entity_print(&firstEntity);


    return 0;
}
