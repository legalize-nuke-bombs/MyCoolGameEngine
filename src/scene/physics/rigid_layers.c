//
// Created by nikita on 01.10.2026.
//

#include "rigid_layers.h"

#include <stdlib.h>

#include "../../utils/dictionary.h"
#include "../../utils/string_dictionary.h"
#include "rigid_layer.h"
#include "../../logging/logger.h"

struct rigid_layers {
    struct dictionary* dict;
};


struct rigid_layers* rigid_layers_create() {
    struct rigid_layers* this = calloc(1, sizeof(struct rigid_layers));
    this->dict = string_dictionary_build(3);
    return this;
}
void rigid_layers_destroy(struct rigid_layers* this) {
    rigid_layers_clear(this);
    dictionary_destroy(this->dict);
    free(this);
}

void rigid_layers_clear(const struct rigid_layers* this) {
    struct dictionary_iterator iterator;
    struct dictionary_node node;
    while (dictionary_next(this->dict, &iterator, &node)) {
        struct rigid_layer* layer = node.value;
        rigid_layer_destroy(layer);
    }
    dictionary_clear(this->dict);
}

void rigid_layers_capture(const struct rigid_layers* this, struct rigid_layer* layer) {
    const char* layer_name = rigid_layer_get_name(layer);
    if (dictionary_try_add(this->dict, (void*)layer_name, layer)) {
        logger_debug("Rigid layers captured layer %s", layer_name);
    }
    else {
        logger_warn("Rigid layers failed to capture layer %s", layer_name);
        rigid_layer_destroy(layer);
    }
}
struct rigid_layer* rigid_layers_get(const struct rigid_layers* this, const char *name) {
    return dictionary_get(this->dict, (void*)name);
}