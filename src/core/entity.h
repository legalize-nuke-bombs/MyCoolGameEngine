#ifndef MYCOOLGAMEENGINE_ENTITY_H
#define MYCOOLGAMEENGINE_ENTITY_H

struct entity {
    const char *name;
};


void init_entity(struct entity *entity);

const char *get_entity_name(const struct entity *entity);
void set_entity_name(struct entity *entity, const char *name);

void print_entity(const struct entity *entity);


#endif
