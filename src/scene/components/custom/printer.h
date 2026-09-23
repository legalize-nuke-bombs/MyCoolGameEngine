#ifndef MYCOOLGAMEENGINE_PRINTER_H
#define MYCOOLGAMEENGINE_PRINTER_H

#include "../component.h"

struct printer;

struct printer* printer_create(struct entity *parent, const char *intervalString, const char *lastString, double interval);

struct component* printer_as_component(struct printer *this);

#endif
