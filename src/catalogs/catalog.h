#ifndef MYCOOLGAMEENGINE_CATALOG_H
#define MYCOOLGAMEENGINE_CATALOG_H

struct catalog;
struct parser;
struct subsystem_collection;

struct catalog_vtable {
    const char* (*key)(void);
    void* (*on_create_item)(const char *name, struct parser *parser, const struct subsystem_collection *subsystems);
    void (*on_destroy_item)(void *item);
};

struct catalog* catalog_create(const struct catalog_vtable *vtable);
void catalog_destroy(struct catalog *this);

void catalog_clear(struct catalog *this);

const char* catalog_get_key(const struct catalog *this);

void catalog_add(struct catalog *this, struct parser *parser, const struct subsystem_collection *subsystems);
void* catalog_get(const struct catalog *this, const char *name);

#endif //MYCOOLGAMEENGINE_CATALOG_H
