#ifndef MYCOOLGAMEENGINE_TEXTURES_H
#define MYCOOLGAMEENGINE_TEXTURES_H

#include "../api.h"

struct texture;
struct asset_type;

MCGE_API extern const struct asset_type textures_asset_type;

MCGE_API struct texture* textures_get(const char *name);

#endif //MYCOOLGAMEENGINE_TEXTURES_H
