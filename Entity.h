#ifndef MYCOOLGAMEENGINE_ENTITY_H
#define MYCOOLGAMEENGINE_ENTITY_H

struct Entity {
    const char *name;
};


void init_entity(struct Entity *entity, const char *name);

const char *get_entity_name(const struct Entity *entity);

void print_entity(const struct Entity *entity);


#endif
