//
// Created by nikita on 24.09.2026.
//

#include "renderer_layer.h"

#include <stdlib.h>
#include <string.h>

#include "../catalogs/catalog.h"
#include "../utils/parser.h"


struct renderer_layer {
    char *name;
    int priority;
};


static const char* renderer_layer_catalog_key(void) {
    return "renderer_layer";
}

static void* renderer_layer_on_create_item(const char *name, struct parser *parser, const struct subsystem_collection *subsystems) {
    uint8_t priority;
    parser_next_uint8(parser, &priority);
    return renderer_layer_create(strdup(name), priority);
}
static void renderer_layer_on_destroy_item(void *item) {
    renderer_layer_destroy(item);
}

const struct catalog_vtable renderer_layer_catalog_vtable = {
    .key = renderer_layer_catalog_key,
    .on_create_item = renderer_layer_on_create_item,
    .on_destroy_item = renderer_layer_on_destroy_item
};


struct renderer_layer* renderer_layer_create(char *name, uint8_t priority) {
    struct renderer_layer *layer = malloc(sizeof(struct renderer_layer));
    layer->name = name;
    layer->priority = priority;
    return layer;
}
void renderer_layer_destroy(struct renderer_layer *layer) {
    free(layer->name);
    free(layer);
}

const char* renderer_layer_get_name(const struct renderer_layer *layer) {
    if (layer == NULL) {
        return "NULL";
    }
    return layer->name;
}
uint8_t renderer_layer_get_priority(const struct renderer_layer *layer) {
    if (layer == NULL) {
        return 0;
    }
    return layer->priority;
}