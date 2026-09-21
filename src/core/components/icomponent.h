#ifndef MYCOOLGAMEENGINE_COMPONENT_H
#define MYCOOLGAMEENGINE_COMPONENT_H

struct icomponent;

struct icomponent_vtable {
    void (*awake)(struct icomponent *self);
    void (*update)(struct icomponent *self);
    void (*on_destroy)(struct icomponent *self);
};

struct icomponent {
    struct icomponent_vtable *vtable;
};

#endif
