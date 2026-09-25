//
// Created by nikita on 24.09.2026.
//

#include "renderer_layer_manager.h"

#include <stdlib.h>

#include "../logging/logger.h"
#include "../utils/dictionary.h"
#include "../utils/string_dictionary.h"


struct renderer_layer_manager {
    struct dictionary *dictionary;
};


struct renderer_layer_manager* renderer_layer_manager_create() {
    logger_info("Renderer layer manager is creating...");
    struct renderer_layer_manager *this = malloc(sizeof(struct renderer_layer_manager));
    this->dictionary = string_dictionary_build(8);
    return this;
}
void renderer_layer_manager_destroy(struct renderer_layer_manager *this) {
    logger_info("Renderer layer manager is destroying...");
    for (int i = 0; i < dictionary_capacity(this->dictionary); i++) {
        const struct dictionary_node node = dictionary_get_node(this->dictionary, i);
        if (node.value == NULL) {
            continue;
        }
        renderer_layer_destroy(node.value);
    }
    dictionary_destroy(this->dictionary);
    free(this);
}

void renderer_layer_manager_capture(const struct renderer_layer_manager *this, struct renderer_layer* layer) {
    const char *layer_name = renderer_layer_get_name(layer);
    if (dictionary_try_add(this->dictionary, (void*)layer_name, layer) == 1) {
        logger_debug("Renderer layer manager captured layer %s", layer_name);
        return;
    }
    logger_warn("Renderer layer manager failed to register layer %s", layer_name);
    renderer_layer_destroy(layer);
}

struct renderer_layer* renderer_layer_manager_try_get(const struct renderer_layer_manager *this, const char *layer_name) {
    return dictionary_get(this->dictionary, (void*)layer_name);
}

void renderer_layer_manager_clear(const struct renderer_layer_manager *this) {
    for (int i = 0; i < dictionary_capacity(this->dictionary); i++) {
        const struct dictionary_node node = dictionary_get_node(this->dictionary, i);
        if (node.value == NULL) {
            continue;
        }
        renderer_layer_destroy(node.value);
    }
    dictionary_clear(this->dictionary);
}