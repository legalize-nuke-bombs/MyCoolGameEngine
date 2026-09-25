//
// Created by Nikita on 25.09.2026.
//


#include "engine_restarter.h"

#include <stdlib.h>
#include "../../logging/logger.h"


struct engine_restarter {

};


struct engine_restarter* engine_restarter_create(struct engine *engine, struct engine_execution_arguments arguments) {
    logger_info("Engine restarter is creating...");
    struct engine_restarter *this = malloc(sizeof(struct engine_restarter));
    return this;
}
void engine_restarter_destroy(struct engine_restarter *this) {
    logger_info("Engine restarter is destroying...");
    free(this);
}