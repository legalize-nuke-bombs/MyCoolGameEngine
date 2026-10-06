#ifndef MYCOOLGAMEENGINE_TEXTURES_H
#define MYCOOLGAMEENGINE_TEXTURES_H

struct texture;
struct asset_type;

extern const struct asset_type textures_asset_type;

struct texture* textures_get(const char *name);

#endif //MYCOOLGAMEENGINE_TEXTURES_H
