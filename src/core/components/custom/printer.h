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

void printer_update(struct component *base, const struct update_context *context);
void printer_destroy(struct component *base);

static struct component_vtable printer_vtable = {
    .on_update = printer_update,
    .on_destroy = printer_destroy
};

void printer_init(struct printer *this, const char *intervalString, const char *lastString, double interval);

struct component* printer_as_component(struct printer *this);

#endif
