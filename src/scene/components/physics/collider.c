//
// Created by nikita on 02.10.2026.
//

#include "collider.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../../utils/dictionary.h"
#include "../../../utils/pointer_dictionary.h"
#include "../../scene.h"


struct collider {
    struct component base;

    struct dictionary* intersections;

    const struct chunks* chunks;
};

const char* collider_component_key(void) {
    return "collider";
}

static struct component* collider_clone(struct component base, const struct component *component);
static bool collider_is_chunkable() {
    return true;
}

static void collider_on_destroy(struct component *component);
static void collider_on_awake(struct component *base);
static void collider_on_movement(struct component *component);

static const struct component_vtable collider_vtable = {
    .component_key = collider_component_key,
    .on_clone = collider_clone,
    .on_destroy = collider_on_destroy,
    .on_awake = collider_on_awake,
    .is_chunkable = collider_is_chunkable,
    .on_movement = collider_on_movement
};

struct component* collider_create(struct parser *parser, struct entity *parent) {
    struct collider *this = calloc(1, sizeof(struct collider));
    struct component* base = (struct component*)this;
    component_base_create(base, &collider_vtable, parent);
    return base;
}

static void collider_on_destroy(struct component *component) {
    struct collider *this = (struct collider*)component;
    if (this->intersections) {
        dictionary_destroy(this->intersections);
        this->intersections = NULL;
    }
}

struct component* collider_clone(struct component base, const struct component *component) {
    struct collider *collider = (struct collider*)component;

    struct collider *this = calloc(1, sizeof(struct collider));
    this->base = base;
    return (struct component*)this;
}

static void collider_on_awake(struct component *base) {
    struct collider *this = (struct collider*)base;

    this->chunks = scene_get_chunks(component_get_scene(base));
}

static void collider_on_movement(struct component *component) {
    struct collider* this = (struct collider*)component;

}