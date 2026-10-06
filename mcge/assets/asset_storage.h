#ifndef MYCOOLGAMEENGINE_ASSET_STORAGE_H
#define MYCOOLGAMEENGINE_ASSET_STORAGE_H

#include "../api.h"

struct dictionary;

// A helper for the asset types whose id is a single string. An asset type is free to keep its assets any other way.
struct asset_storage {
    const char *_key;
    void (*_destroy_item)(void *item);
    struct dictionary *_items;
};

MCGE_API void asset_storage_add(struct asset_storage *this, const char *name, void *item);
MCGE_API void* asset_storage_try_get(const struct asset_storage *this, const char *name);
MCGE_API void* asset_storage_get(const struct asset_storage *this, const char *name);

MCGE_API void asset_storage_clear(struct asset_storage *this);
MCGE_API void asset_storage_destroy(struct asset_storage *this);

#endif //MYCOOLGAMEENGINE_ASSET_STORAGE_H
