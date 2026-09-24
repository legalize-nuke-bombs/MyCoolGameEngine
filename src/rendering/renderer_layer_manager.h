//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_LAYER_MANAGER_H
#define MYCOOLGAMEENGINE_RENDERER_LAYER_MANAGER_H
#include <stdbool.h>

#include "renderer_layer.h"

struct renderer_layer_manager;

struct renderer_layer_manager* renderer_layer_manager_create();
void renderer_layer_manager_destroy(struct renderer_layer_manager *this);

void renderer_layer_manager_capture(const struct renderer_layer_manager *this, struct renderer_layer* layer);

struct renderer_layer* renderer_layer_manager_try_get(const struct renderer_layer_manager *this, const char *layer_name);

#endif //MYCOOLGAMEENGINE_RENDERER_LAYER_MANAGER_H
