#ifndef MYCOOLGAMEENGINE_PREFABS_H
#define MYCOOLGAMEENGINE_PREFABS_H

#include "../../api.h"

struct prefab;
struct asset_type;

MCGE_API extern const struct asset_type prefabs_asset_type;

MCGE_API struct prefab* prefabs_get(const char *name);

#endif //MYCOOLGAMEENGINE_PREFABS_H
