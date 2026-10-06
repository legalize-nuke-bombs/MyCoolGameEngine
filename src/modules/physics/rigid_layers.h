#ifndef MYCOOLGAMEENGINE_RIGID_LAYERS_H
#define MYCOOLGAMEENGINE_RIGID_LAYERS_H

struct rigid_layer;
struct asset_type;

extern const struct asset_type rigid_layers_asset_type;

struct rigid_layer* rigid_layers_get(const char *name);

#endif //MYCOOLGAMEENGINE_RIGID_LAYERS_H
