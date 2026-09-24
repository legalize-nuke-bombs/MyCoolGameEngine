//
// Created by nikita on 24.09.2026.
//

#include "renderer_layer.h"

#include <stdlib.h>


struct renderer_layer {
    char *name;
    int priority;
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