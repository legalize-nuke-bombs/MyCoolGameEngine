//
// Created by nikita on 14.10.2026.
//

#ifndef MYCOOLGAMEENGINE_FONTS_H
#define MYCOOLGAMEENGINE_FONTS_H

#include "../../../api.h"


MCGE_API extern const struct asset_type fonts_asset_type;

MCGE_API struct font* fonts_get(const char* name);

#endif //MYCOOLGAMEENGINE_FONTS_H
