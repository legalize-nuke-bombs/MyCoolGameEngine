//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_FACTORY_INTERNAL_H
#define MYCOOLGAMEENGINE_FACTORY_INTERNAL_H

#include "factory.h"

struct factory_vtable {
    const char* key;
    void* (*on_produce)(struct factory *base, const char *key);
};

struct factory {
    const struct factory_vtable *vtable;
    struct dictionary *dict;
};

void factory_base_create(struct factory *this, const struct factory_vtable *vtable);

void factory_register(const struct factory *this, const char *key, void *constructor);

#endif //MYCOOLGAMEENGINE_FACTORY_INTERNAL_H
