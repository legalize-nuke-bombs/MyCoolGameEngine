#ifndef MYCOOLGAMEENGINE_ENTITY_H
#define MYCOOLGAMEENGINE_ENTITY_H

struct entity {
    const char *_name;
};


void entity_init(struct entity *entity);

const char *entity_get_name(const struct entity *entity);
void entity_set_name(struct entity *entity, const char *name);

void entity_print(const struct entity *entity);


#endif
