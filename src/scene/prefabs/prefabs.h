#ifndef MYCOOLGAMEENGINE_PREFABS_H
#define MYCOOLGAMEENGINE_PREFABS_H

struct prefab;
struct asset_type;

extern const struct asset_type prefabs_asset_type;

struct prefab* prefabs_get(const char *name);

#endif //MYCOOLGAMEENGINE_PREFABS_H
