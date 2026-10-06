#ifndef MYCOOLGAMEENGINE_ASSETS_H
#define MYCOOLGAMEENGINE_ASSETS_H

#include <stdbool.h>
#include "../api.h"

struct msystem;
struct asset_type;
struct fields;

MCGE_API extern const struct msystem assets_msystem;

MCGE_API void assets_register(const struct asset_type *type);

MCGE_API bool assets_add(struct fields *fields);

#endif //MYCOOLGAMEENGINE_ASSETS_H
