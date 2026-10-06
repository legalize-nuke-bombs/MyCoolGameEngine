#ifndef MYCOOLGAMEENGINE_FACTORY_H
#define MYCOOLGAMEENGINE_FACTORY_H

#include "../api.h"

struct dictionary;

struct factory {
    const char *_name;
    struct dictionary *_items;
};

MCGE_API struct factory factory_create(const char *name);
MCGE_API void factory_destroy(struct factory *this);

MCGE_API void factory_register(struct factory *this, const char *key, void *item);
MCGE_API void* factory_find(const struct factory *this, const char *key);

#endif //MYCOOLGAMEENGINE_FACTORY_H
