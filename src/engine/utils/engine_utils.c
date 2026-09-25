//
// Created by Nikita on 25.09.2026.
//


#include "engine_utils.h"
#include "custom/engine_closer.h"
#include <stdlib.h>
#include "../../logging/logger.h"
#include "custom/engine_restarter.h"


struct engine_utils {
    struct engine_closer* closer;
    struct engine_restarter* restarter;
};



struct engine_utils* engine_utils_create(struct engine *engine, struct engine_init_arguments arguments) {
    logger_info("Engine utils are creating...");
    struct engine_utils* this = malloc(sizeof(struct engine_utils));
    this->closer = engine_closer_create(engine);
    this->restarter = engine_restarter_create(engine, arguments);
    return this;
}
void engine_utils_destroy(struct engine_utils *this) {
    logger_info("Engine utils are destroying...");
    engine_closer_destroy(this->closer);
    engine_restarter_destroy(this->restarter);
    free(this);
}

void engine_utils_awake(const struct engine_utils *this) {
    logger_info("Engine utils are awaking...");
    engine_closer_awake(this->closer);
    engine_restarter_awake(this->restarter);
}
void engine_utils_disable(const struct engine_utils *this) {
    logger_info("Engine utils are disabling...");
    engine_closer_disable(this->closer);
    engine_restarter_disable(this->restarter);
}
