//
// Created by nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RIGID_LAYERS_H
#define MYCOOLGAMEENGINE_RIGID_LAYERS_H

struct rigid_layers;
struct rigid_layer;

struct rigid_layers* rigid_layers_create();
void rigid_layers_destroy(struct rigid_layers* this);

void rigid_layers_clear(const struct rigid_layers* this);

void rigid_layers_capture(const struct rigid_layers* this, struct rigid_layer* layer);
struct rigid_layer* rigid_layers_get(const struct rigid_layers* this, const char *name);

#endif //MYCOOLGAMEENGINE_RIGID_LAYERS_H
