//
// Created by nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RIGID_LAYER_H
#define MYCOOLGAMEENGINE_RIGID_LAYER_H

struct rigid_layer;

struct rigid_layer* rigid_layer_create(char *name, double friction);
void rigid_layer_destroy(struct rigid_layer *layer);

const char* rigid_layer_get_name(const struct rigid_layer *layer);
double rigid_layer_get_friction(const struct rigid_layer *layer);

#endif //MYCOOLGAMEENGINE_RIGID_LAYER_H
