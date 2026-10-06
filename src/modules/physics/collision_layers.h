#ifndef MYCOOLGAMEENGINE_COLLISION_LAYERS_H
#define MYCOOLGAMEENGINE_COLLISION_LAYERS_H

struct collision_layer;
struct asset_type;

// Together with collision_layer_default, which is always there under the name `default`
#define COLLISION_LAYERS_MAX 32

extern const struct asset_type collision_layers_asset_type;

const struct collision_layer* collision_layers_get(const char *name);

#endif //MYCOOLGAMEENGINE_COLLISION_LAYERS_H
