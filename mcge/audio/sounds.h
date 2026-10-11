//
// Created by nikita on 11.10.2026.
//

#ifndef MYCOOLGAMEENGINE_SOUNDS_H
#define MYCOOLGAMEENGINE_SOUNDS_H

#include "../api.h"

struct sound;
struct asset_type;

MCGE_API extern const struct asset_type sounds_asset_type;

MCGE_API struct sound* sounds_get(const char *name);

#endif //MYCOOLGAMEENGINE_SOUNDS_H
