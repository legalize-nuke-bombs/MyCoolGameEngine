#ifndef MYCOOLGAMEENGINE_COLLISION_RULES_H
#define MYCOOLGAMEENGINE_COLLISION_RULES_H

#include "collision_response.h"
#include "../../api.h"

struct collision_layer;
struct asset_type;

MCGE_API extern const struct asset_type collision_rules_asset_type;

MCGE_API enum collision_response collision_rules_get_response(const struct collision_layer *layer1, const struct collision_layer *layer2);

#endif //MYCOOLGAMEENGINE_COLLISION_RULES_H
