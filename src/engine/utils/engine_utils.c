//
// Created by Nikita on 25.09.2026.
//


#include "engine_utils.h"
#include "custom/engine_closer.h"
#include <stdlib.h>
#include "../../logging/logger.h"


struct engine_utils {
    struct engine_closer* closer;
};



struct engine_utils* engine_utils_create(struct engine *engine) {
    logger_info("Engine utils are creating...");
    struct engine_utils* this = malloc(sizeof(struct engine_utils));
    this->closer = engine_closer_create(engine);
    return this;
}
void engine_utils_destroy(struct engine_utils *this) {
    logger_info("Engine utils are destroying...");
    free(this->closer);
    free(this);
}

void engine_utils_awake(const struct engine_utils *this) {
    logger_info("Engine utils are awaking...");
    engine_closer_awake(this->closer);
}
void engine_utils_disable(const struct engine_utils *this) {
    logger_info("Engine utils are disabling...");
    engine_closer_disable(this->closer);
}
