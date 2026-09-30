//
// Created by Nikita on 30.09.2026.
//

#ifndef MYCOOLGAMEENGINE_CHUNK_H
#define MYCOOLGAMEENGINE_CHUNK_H

struct component;
struct dictionary;

struct chunk {
    struct dictionary* components;
    struct dictionary* types;
};

void chunk_destroy(struct chunk *this);

void chunk_try_add_component(struct chunk *this, struct component* component);
void chunk_try_remove_component(const struct chunk *this, struct component* component);

struct dictionary* chunk_get_components(const struct chunk* this);
struct dictionary* chunk_get_components_by_type(const struct chunk* this, const char* component_type);

#endif //MYCOOLGAMEENGINE_CHUNK_H
