#ifndef MYCOOLGAMEENGINE_COLLISION_LAYERS_H
#define MYCOOLGAMEENGINE_COLLISION_LAYERS_H

#include "../../api.h"

struct collision_layer;
struct asset_type;

#define COLLISION_LAYERS_MAX 32

MCGE_API extern const struct asset_type collision_layers_asset_type;

MCGE_API const struct collision_layer* collision_layers_get(const char *name);

#endif //MYCOOLGAMEENGINE_COLLISION_LAYERS_H
