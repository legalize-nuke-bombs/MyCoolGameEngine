#ifndef MYCOOLGAMEENGINE_COMPONENT_H
#define MYCOOLGAMEENGINE_COMPONENT_H

struct component;

struct component_vtable {
    void (*on_update)(struct component *self, double dt);
    void (*on_destroy)(struct component *self);
};

struct component {
    struct component_vtable *_vtable;
    struct entity *_parent;
};

void component_init(struct component *component);

struct entity* component_get_parent(struct component *this);
void component_set_parent(struct component *this, struct entity *parent);

#endif
