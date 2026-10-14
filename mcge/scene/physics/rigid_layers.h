#ifndef MYCOOLGAMEENGINE_RIGID_LAYERS_H
#define MYCOOLGAMEENGINE_RIGID_LAYERS_H

#include "../../api.h"

struct rigid_layer;
struct asset_type;

MCGE_API extern const struct asset_type rigid_layers_asset_type;

MCGE_API struct rigid_layer* rigid_layers_get(const char *name);

#endif //MYCOOLGAMEENGINE_RIGID_LAYERS_H
