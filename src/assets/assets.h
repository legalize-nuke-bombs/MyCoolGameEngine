#ifndef MYCOOLGAMEENGINE_ASSETS_H
#define MYCOOLGAMEENGINE_ASSETS_H

#include <stdbool.h>

struct msystem;
struct asset_type;
struct fields;

extern const struct msystem assets_msystem;

void assets_register(const struct asset_type *type);

bool assets_add(struct fields *fields);

#endif //MYCOOLGAMEENGINE_ASSETS_H
