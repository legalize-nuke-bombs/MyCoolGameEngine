#ifndef MYCOOLGAMEENGINE_RIGID_MATERIALS_H
#define MYCOOLGAMEENGINE_RIGID_MATERIALS_H

struct rigid_material;
struct asset_type;

extern const struct asset_type rigid_materials_asset_type;

struct rigid_material* rigid_materials_get(const char *name);

#endif //MYCOOLGAMEENGINE_RIGID_MATERIALS_H
