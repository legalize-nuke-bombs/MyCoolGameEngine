//
// Created by Nikita on 25.09.2026.
//

#include "subsystem_collection.h"

#include <stdlib.h>
#include "../logging/logger.h"


struct subsystem_collection {

};


struct subsystem_collection* subsystem_collection_create() {
    logger_info("Subsystem collection is creating...");
    struct subsystem_collection* this = malloc(sizeof(struct subsystem_collection));
    return this;
}
void subsystem_collection_destroy(struct subsystem_collection* this) {
    logger_info("Subsystem collection is destroying...");
    free(this);
}

void subsystem_collection_enable_all(struct subsystem_collection* this) {
    logger_info("Subsystem collection is enabling all subsystems....");
}
void subsystem_collection_disable_all(struct subsystem_collection* this) {
    logger_info("Subsystem collection is disabling all subsystems...");
}

struct subsystem* subsystem_collection_get(struct subsystem_collection* this, const char* name) {
    return NULL;
}