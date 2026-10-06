#ifndef MYCOOLGAMEENGINE_RENDERER_LAYERS_H
#define MYCOOLGAMEENGINE_RENDERER_LAYERS_H

struct renderer_layer;
struct asset_type;

extern const struct asset_type renderer_layers_asset_type;

struct renderer_layer* renderer_layers_try_get(const char *name);

#endif //MYCOOLGAMEENGINE_RENDERER_LAYERS_H
