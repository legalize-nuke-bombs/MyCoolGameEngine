//
// Created by Nikita on 27.09.2026.
//

#include "texture_manager.h"

#include <stdlib.h>

#include "../../logging/logger.h"


struct texture_manager {

};


struct texture_manager* texture_manager_create() {
    logger_info("Texture manager is creating...");
    struct texture_manager* this = calloc(1, sizeof(struct texture_manager));
    return this;
}
void texture_manager_destroy(struct texture_manager* this) {
    logger_info("Texture manager is destroying...");
    free(this);
}