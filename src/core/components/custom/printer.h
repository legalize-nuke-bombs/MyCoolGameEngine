#ifndef MYCOOLGAMEENGINE_PRINTER_H
#define MYCOOLGAMEENGINE_PRINTER_H

#include "../component.h"

struct printer {
    struct component base;
    const char* intervalString;
    double interval;
    double timer;
    const char* lastString;
};

void printer_update(struct component *self, double dt);
void printer_destroy(struct component *self);

static struct component_vtable printer_vtable = {
    .on_update = printer_update,
    .on_destroy = printer_destroy
};

void printer_init(struct component *self, struct entity *parent, const char *string, double interval);

#endif
