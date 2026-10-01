//
// Created by Nikita on 27.09.2026.
//

#include "texture_manager.h"

#include <stdlib.h>

#include "texture.h"
#include "../../logging/logger.h"
#include "../../utils/dictionary.h"
#include "../../utils/list.h"
#include "../../utils/string_dictionary.h"


struct texture_manager {
    struct list list;
    struct dictionary* dict;
};


struct texture_manager* texture_manager_create() {
    logger_info("Texture manager is creating...");
    struct texture_manager* this = calloc(1, sizeof(struct texture_manager));
    this->list = list_create(1024);
    this->dict = string_dictionary_build(4);
    return this;
}
void texture_manager_destroy(struct texture_manager* this) {
    logger_info("Texture manager is destroying...");
    texture_manager_clear(this);
    list_destroy(&this->list);
    dictionary_destroy(this->dict);
    free(this);
}

void texture_manager_capture(struct texture_manager* this, struct texture* texture) {
    if (dictionary_try_add(this->dict, (void*)texture_get_id(texture), texture)) {
        list_add(&this->list, texture);
    }
    else {
        logger_warn("Texture manager failed to capture %s", texture_get_id(texture));
        texture_destroy(texture);
    }
}

struct texture* texture_manager_try_get_texture(const struct texture_manager* this, const char* texture_id) {
    return dictionary_get(this->dict, (void*)texture_id);
}

void texture_manager_clear(struct texture_manager* this) {
    logger_info("Texture manager is clearing...");
    for (int i = 0; i < list_count(&this->list); i++) {
        struct texture* texture = list_get(&this->list, i);
        texture_destroy(texture);
    }
    list_clear(&this->list);
    dictionary_clear(this->dict);
}