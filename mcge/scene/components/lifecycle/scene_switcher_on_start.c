//
// Created by nikita on 16.10.2026.
//

#include "scene_switcher_on_start.h"

#include "mcge/allocator/allocator.h"
#include "mcge/scene/scene.h"
#include "mcge/utils/fields.h"
#include "mcge/scene/components/component_internal.h"


struct scene_switcher_on_start {
    struct component base;

    char* map_path;
};

static void scene_switcher_on_start_on_create(struct component *base, struct fields *fields);
static void scene_switcher_on_start_awake(struct component *base);
static void scene_switcher_on_start_on_destroy(struct component *base);

const struct component_vtable scene_switcher_on_start_vtable = {
    .component_key = scene_switcher_on_start_component_key,
    .size = sizeof(struct scene_switcher_on_start),
    .on_create = scene_switcher_on_start_on_create,
    .on_awake = scene_switcher_on_start_awake,
    .on_destroy = scene_switcher_on_start_on_destroy
};

const char* scene_switcher_on_start_component_key(void) {
    return "scene_switcher_on_start";
}

static void scene_switcher_on_start_on_create(struct component *base, struct fields *fields) {
    struct scene_switcher_on_start *this = (struct scene_switcher_on_start *) base;
    this->map_path = fields_dup_string(fields, "script", "");
}
static void scene_switcher_on_start_awake(struct component *base) {
    const struct scene_switcher_on_start *this = (struct scene_switcher_on_start *) base;
    scene_mark_switch(this->map_path);
}
static void scene_switcher_on_start_on_destroy(struct component *base) {
    const struct scene_switcher_on_start *this = (struct scene_switcher_on_start *) base;
    allocator_delete(this->map_path);
}