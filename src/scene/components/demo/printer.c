#include "printer.h"
#include "../component_internal.h"
#include "../../../logging/logger.h"
#include "../../entity.h"

#include <stdio.h>
#include <stdlib.h>
#include "../../../utils/parser.h"

struct printer {
    struct component base;
    char* intervalString;
    char* lastString;
    double interval;
    double timer;
};

static void printer_update(struct component *base, const struct update_context *context);
static void printer_on_disable(struct component *base);
static void printer_destroy(struct component *base);

static const struct component_vtable printer_vtable = {
    .component_key = printer_component_key,
    .on_awake = NULL,
    .on_update = printer_update,
    .on_disable = printer_on_disable,
    .on_destroy = printer_destroy
};

const char* printer_component_key(void) {
    return "printer";
}

struct component* printer_create(struct parser *parser, struct entity *parent) {
    struct printer *this = malloc(sizeof(struct printer));
    struct component *base = (struct component *)this;
    component_create(base, &printer_vtable, parser, parent);

    this->intervalString = parser_next_dup(parser);
    this->lastString = parser_next_dup(parser);
    parser_next_double(parser, &this->interval);
    this->timer = 0.0;

    return base;
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

static void printer_on_disable(struct component *base) {
    const struct printer *this = (struct printer *)base;

    if (this->lastString == NULL) {
        return;
    }
    logger_info("Entity %s: %s", entity_get_name(component_get_parent(base)), this->lastString);
}

static void printer_destroy(struct component *base) {
    const struct printer *this = (struct printer *)base;

    if (this->intervalString != NULL) {
        free(this->intervalString);
    }
    if (this->lastString != NULL) {
        free(this->lastString);
    }
}
