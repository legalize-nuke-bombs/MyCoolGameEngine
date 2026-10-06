#ifndef MYCOOLGAMEENGINE_FACTORY_H
#define MYCOOLGAMEENGINE_FACTORY_H

struct dictionary;

struct factory {
    const char *_name;
    struct dictionary *_items;
};

struct factory factory_create(const char *name);
void factory_destroy(struct factory *this);

void factory_register(struct factory *this, const char *key, void *item);
void* factory_find(const struct factory *this, const char *key);

#endif //MYCOOLGAMEENGINE_FACTORY_H
