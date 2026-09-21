#ifndef MYCOOLGAMEENGINE_COMPONENT_H
#define MYCOOLGAMEENGINE_COMPONENT_H

struct component;

struct component_vtable {
    void (*awake)(struct component *self);
    void (*update)(struct component *self);
    void (*on_destroy)(struct component *self);
};

struct component {
    struct component_vtable *vtable;
    struct entity *parent;
};

void component_init(struct component *component, struct entity *parent);

#endif
