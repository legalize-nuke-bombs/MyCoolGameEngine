//
// Created by nikita on 02.10.2026.
//

#include "collider.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../../utils/dictionary.h"
#include "../../../utils/pointer_dictionary.h"
#include "../../scene.h"
#include "../../../logging/logger.h"
#include "../../../utils/action.h"
#include "../../chunks/chunks.h"


struct collider {
    struct component base;

    struct dictionary* intersections;

    struct action on_trigger_enter;
    struct action on_trigger_exit;

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
static void collider_on_movement(struct component *base);
static void collider_on_disable(struct component *base);

static const struct component_vtable collider_vtable = {
    .component_key = collider_component_key,
    .on_clone = collider_clone,
    .on_destroy = collider_on_destroy,
    .on_awake = collider_on_awake,
    .is_chunkable = collider_is_chunkable,
    .on_movement = collider_on_movement,
    .on_disable = collider_on_disable
};

struct component* collider_create(struct parser *parser, struct entity *parent) {
    struct collider *this = calloc(1, sizeof(struct collider));
    struct component* base = (struct component*)this;
    component_base_create(base, &collider_vtable, parent);
    this->on_trigger_enter = action_create();
    this->on_trigger_exit = action_create();
    return base;
}

static void collider_on_destroy(struct component *component) {
    struct collider *this = (struct collider*)component;
    action_destroy(&this->on_trigger_exit);
    action_destroy(&this->on_trigger_enter);
    if (this->intersections) {
        dictionary_destroy(this->intersections);
        this->intersections = NULL;
    }
}

struct component* collider_clone(struct component base, const struct component *component) {
    struct collider *collider = (struct collider*)component;

    struct collider *this = calloc(1, sizeof(struct collider));
    this->base = base;
    this->on_trigger_enter = action_create();
    this->on_trigger_exit = action_create();
    return (struct component*)this;
}

static void collider_on_awake(struct component *base) {
    struct collider *this = (struct collider*)base;

    this->chunks = scene_get_chunks(component_get_scene(base));
}

static void collider_lazy_create_intersections(struct collider *this) {
    if (this->intersections == NULL) {
        this->intersections = pointer_dictionary_build(1);
    }
}

static void collider_handle_on_trigger_enter(struct collider *this, struct collider *collider, const bool share) {
    collider_lazy_create_intersections(this);
    if (!dictionary_try_add(this->intersections, collider, collider)) {
        return;
    }
    logger_debug("Entity %s on trigger enter %s!", component_get_global_parent_name((struct component*)this), component_get_global_parent_name((struct component*)collider));
    action_invoke(&this->on_trigger_enter, component_get_parent((struct component*)collider));
    if (share) {
        collider_handle_on_trigger_enter(collider, this, false);
    }
}

static void collider_handle_on_trigger_exit(struct collider *this, struct collider *collider, const bool share) {
    if (this->intersections == NULL || dictionary_remove(this->intersections, collider) == 0) {
        return;
    }
    logger_debug("Entity %s on trigger exit %s!", component_get_global_parent_name((struct component*)this), component_get_global_parent_name((struct component*)collider));
    action_invoke(&this->on_trigger_exit, component_get_parent((struct component*)collider));
    if (share) {
        collider_handle_on_trigger_exit(collider, this, false);
    }
}

static void collider_check_new_intersections(struct collider *this, const struct rect rect, const struct dictionary *colliders) {
    struct dictionary_iterator iterator = dictionary_begin(colliders);
    struct dictionary_node node;
    while (dictionary_next(colliders, &iterator, &node)) {
        struct collider *collider = node.value;
        if (collider == this) {
            continue;
        }
        const struct rect collider_rect = component_get_rect((struct component*)collider);
        if (rects_intersection(rect, collider_rect)) {
            collider_handle_on_trigger_enter(this, collider, true);
        }
    }
}

static void collider_validate_old_intersections(struct collider *this, const struct rect rect) {
    if (this->intersections == NULL) {
        return;
    }
    struct dictionary_iterator intersections_iterator = dictionary_begin(this->intersections);
    struct dictionary_node node;
    while (dictionary_next(this->intersections, &intersections_iterator, &node)) {
        struct collider *collider = node.value;
        const struct rect collider_rect = component_get_rect((struct component*)collider);
        if (!rects_intersection(rect, collider_rect)) {
            collider_handle_on_trigger_exit(this, collider, true);
        }
    }
    if (dictionary_count(this->intersections) == 0) {
        dictionary_destroy(this->intersections);
        this->intersections = NULL;
    }
}

static void collider_on_disable(struct component *base) {
    struct collider *this = (struct collider*)base;

    if (this->intersections == NULL) {
        return;
    }
    struct dictionary_iterator intersections_iterator = dictionary_begin(this->intersections);
    struct dictionary_node node;
    while (dictionary_next(this->intersections, &intersections_iterator, &node)) {
        collider_handle_on_trigger_exit(this, node.value, true);
    }
}

static void collider_on_movement(struct component *base) {
    struct collider* this = (struct collider*)base;

    const struct rect rect = component_get_rect(base);

    // It is better to validate old intersections sooner than checking new intersections in order to skip validating fresh valid intersections
    collider_validate_old_intersections(this, rect);

    int x_start, x_end, y_start, y_end;
    chunks_get_rect_indexes(this->chunks, rect, &x_start, &x_end, &y_start, &y_end);

    for (int x = x_start; x <= x_end; x++) {
        for (int y = y_start; y <= y_end; y++) {
            const struct dictionary *colliders = chunks_chunk_get_components_by_type(this->chunks, x, y, "collider");
            if (colliders == NULL) {
                continue;
            }

            collider_check_new_intersections(this, rect, colliders);
        }
    }
}