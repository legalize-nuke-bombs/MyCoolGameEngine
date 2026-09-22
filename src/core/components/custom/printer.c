#include "printer.h"
#include "../../../logging/logger.h"
#include "../../entity.h"

#include <stdio.h>

static struct component_vtable printer_vtable = {
    .on_update = printer_update,
    .on_destroy = printer_destroy
};

void printer_init(struct printer *this, const char *intervalString, const char *lastString, double interval) {
    struct component *base = printer_as_component(this);

    component_init(base);
    base->_vtable = &printer_vtable;

    this->intervalString = intervalString;
    this->lastString = lastString;
    this->interval = interval;
    this->timer = 0.0;
}

void printer_update(struct component *base, const struct update_context *context) {
    struct printer *this = (struct printer *)base;

    if (this->intervalString == NULL) {
        return;
    }

    this->timer += context->dt;
    if (this->timer >= this->interval) {
        logger_info("Entity `%s` component printer: %s\n", entity_get_name(component_get_parent(base)), this->intervalString);
        this->timer -= this->interval;
    }
}

void printer_destroy(struct component *base) {
    const struct printer *this = (struct printer *)base;

    if (this->lastString == NULL) {
        return;
    }
    logger_info("Entity `%s` component printer: %s\n", entity_get_name(component_get_parent(base)), this->lastString);
}

struct component *printer_as_component(struct printer *this) {
    return (struct component *) this;
}
