#include "printer.h"
#include "../component_internal.h"
#include "../../../logging/logger.h"
#include "../../entity.h"

#include <stdio.h>
#include <stdlib.h>

struct printer {
    struct component base;
    const char* intervalString;
    double interval;
    double timer;
    const char* lastString;
};

static const char* printer_component_key(void);
static void printer_update(struct component *base, const struct update_context *context);
static void printer_destroy(struct component *base);

static const struct component_vtable printer_vtable = {
    .component_key = printer_component_key,
    .on_awake = NULL,
    .on_update = printer_update,
    .on_destroy = printer_destroy,
    .on_free = NULL
};

static const char* printer_component_key(void) {
    return "printer";
}

struct printer* printer_create(const char *intervalString, const char *lastString, double interval) {
    struct printer *this = malloc(sizeof(struct printer));
    component_init(printer_as_component(this), &printer_vtable);

    this->intervalString = intervalString;
    this->lastString = lastString;
    this->interval = interval;
    this->timer = 0.0;

    return this;
}

static void printer_update(struct component *base, const struct update_context *context) {
    struct printer *this = (struct printer *)base;

    if (this->intervalString == NULL) {
        return;
    }

    this->timer += context->dt;
    if (this->timer >= this->interval) {
        logger_info("Entity %s: %s", entity_get_name(component_get_parent(base)), this->intervalString);
        this->timer -= this->interval;
    }
}

static void printer_destroy(struct component *base) {
    const struct printer *this = (struct printer *)base;

    if (this->lastString == NULL) {
        return;
    }
    logger_info("Entity %s: %s", entity_get_name(component_get_parent(base)), this->lastString);
}

struct component *printer_as_component(struct printer *this) {
    return (struct component *) this;
}
