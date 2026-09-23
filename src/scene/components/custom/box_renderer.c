//
// Created by nikita on 23.09.2026.
//

#include "box_renderer.h"

#include <stddef.h>
#include <stdlib.h>

#include "../component_internal.h"
#include "../../../logging/logger.h"

struct box_renderer {
    struct component base;
    struct color color;
};

static const char* box_renderer_component_key(void);
static void box_renderer_awake(struct component *base);
static void box_renderer_update(struct component *base, const struct update_context *context);

static const struct component_vtable box_renderer_vtable = {
    .component_key = box_renderer_component_key,
    .on_awake = box_renderer_awake,
    .on_update = box_renderer_update,
    .on_destroy = NULL
};

static const char* box_renderer_component_key(void) {
    return "box_renderer";
}

struct box_renderer* box_renderer_create(struct color color) {
    struct box_renderer *this = malloc(sizeof(struct box_renderer));
    component_init(NULL, &box_renderer_vtable);

    this->color = color;

    return this;
}

static void box_renderer_awake(struct component *base) {
    logger_debug("BOX RENDERER AWAKE");
}

static void box_renderer_update(struct component *base, const struct update_context *context) {
    struct printer *this = (struct printer *)base;

    logger_debug("BOX RENDERER RUN");
}

struct component * box_renderer_as_component(struct box_renderer *this) {
    return (struct component*) this;
}