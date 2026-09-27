//
// Created by Nikita on 27.09.2026.
//

#include "texture_manager.h"

#include <stdlib.h>

#include "texture.h"
#include "../../logging/logger.h"
#include "../../utils/dictionary.h"
#include "../../utils/string_dictionary.h"


struct texture_manager {
    struct dictionary* dict;
};


struct texture_manager* texture_manager_create() {
    logger_info("Texture manager is creating...");
    struct texture_manager* this = calloc(1, sizeof(struct texture_manager));
    this->dict = string_dictionary_build(10);
    return this;
}
void texture_manager_destroy(struct texture_manager* this) {
    logger_info("Texture manager is destroying...");
    texture_manager_clear(this);
    dictionary_destroy(this->dict);
    free(this);
}

void texture_manager_capture(const struct texture_manager* this, struct texture* texture) {
    if (!dictionary_try_add(this->dict, (void*)texture_get_id(texture), texture)) {
        logger_warn("Texture manager failed to capture %s", texture_get_id(texture));
    }
}

void texture_manager_clear(const struct texture_manager* this) {
    logger_info("Texture manager is clearing...");
    for (int i = 0; i < dictionary_count(this->dict); i++) {
        const struct dictionary_node node = dictionary_get_node(this->dict, i);
        if (node.value == NULL) {
            return;
        }
        texture_destroy(node.value);
    }
    dictionary_clear(this->dict);
}