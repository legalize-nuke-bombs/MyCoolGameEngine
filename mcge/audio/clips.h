//
// Created by nikita on 11.10.2026.
//

#ifndef MYCOOLGAMEENGINE_CLIPS_H
#define MYCOOLGAMEENGINE_CLIPS_H

#include "../api.h"

struct clip;
struct asset_type;

MCGE_API extern const struct asset_type clips_asset_type;

MCGE_API struct clip* clips_get(const char *name);

#endif //MYCOOLGAMEENGINE_CLIPS_H
