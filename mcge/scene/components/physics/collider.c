//
// Created by nikita on 02.10.2026.
//

#include "collider.h"

#include <stdlib.h>
#include <string.h>

#include "../component_internal.h"
#include "../../../utils/dictionary.h"
#include "../../../utils/pointer_dictionary.h"
#include "../../scene.h"
#include "../../../modules/physics/collision_layer.h"
#include "../../../modules/physics/collision_layers.h"
#include "../../../modules/physics/collision_rules.h"
#include "../../../modules/physics/rigid_material.h"
#include "../../../modules/physics/rigid_materials.h"
#include "../../../logging/logger.h"
#include "../../../utils/action.h"
#include "../../../utils/fields.h"
#include "../../chunks/chunks.h"


struct collider {
    struct component base;

    char *material_name;
    char *layer_name;
    struct rigid_material *material;
    const struct collision_layer *layer;

    struct dictionary* intersections;

    struct action on_enter;
    struct action on_exit;

    const struct chunks* chunks;
};

const char* collider_component_key(void) {
    return "collider";
}

static void collider_on_create(struct component *base, struct fields *fields);
static bool collider_is_chunkable() {
    return true;
}

static void collider_on_awake(struct component *base);
static void collider_on_movement(struct component *base);
static void collider_on_disable(struct component *base);
static void collider_on_destroy(struct component *base);

const struct component_vtable collider_vtable = {
    .component_key = collider_component_key,
    .size = sizeof(struct collider),
    .on_create = collider_on_create,
    .on_destroy = collider_on_destroy,
    .on_awake = collider_on_awake,
    .is_chunkable = collider_is_chunkable,
    .on_movement = collider_on_movement,
    .on_disable = collider_on_disable
};

static void collider_on_create(struct component *base, struct fields *fields) {
    struct collider *this = (struct collider*)base;
    this->material_name = fields_dup_string(fields, "material", NULL);
    this->layer_name = fields_dup_string(fields, "layer", NULL);
    this->on_enter = action_create();
    this->on_exit = action_create();
}

static void collider_on_destroy(struct component *base) {
    struct collider *this = (struct collider*)base;
    if (this->material_name) free(this->material_name);
    if (this->layer_name) free(this->layer_name);
    action_destroy(&this->on_exit);
    action_destroy(&this->on_enter);
    if (this->intersections) {
        dictionary_destroy(this->intersections);
        this->intersections = NULL;
    }
}

static void collider_on_awake(struct component *base) {
    struct collider *this = (struct collider*)base;

    if (this->material_name != NULL) {
        this->material = rigid_materials_get(this->material_name);
    }
    free(this->material_name);
    this->material_name = NULL;

    if (this->layer_name != NULL) {
        this->layer = collision_layers_get(this->layer_name);
    }
    free(this->layer_name);
    this->layer_name = NULL;

    this->chunks = scene_get_chunks();
}

static const struct collision_layer* collider_get_layer(const struct collider *this) {
    if (this->layer) {
        return this->layer;
    }
    return &collision_layer_default;
}

static enum collision_response collider_get_response(const struct collider *this, const struct collider *collider) {
    return collision_rules_get_response(collider_get_layer(this), collider_get_layer(collider));
}

static void collider_lazy_create_intersections(struct collider *this) {
    if (this->intersections == NULL) {
        this->intersections = pointer_dictionary_build(1);
    }
}

static void collider_handle_on_exit(struct collider *this, struct collider *collider, bool share);

static void collider_handle_on_enter(struct collider *this, struct collider *collider, const bool share) {
    // The dead do not enter anybody: their on_disable has already left everyone
    if (!component_is_alive((struct component*)this) || !component_is_alive((struct component*)collider)) {
        return;
    }
    collider_lazy_create_intersections(this);
    if (!dictionary_try_add(this->intersections, collider, collider)) {
        return;
    }
    logger_debug("Entity %s on enter %s!", component_get_global_parent_name((struct component*)this), component_get_global_parent_name((struct component*)collider));
    action_invoke(&this->on_enter, component_get_global_parent((struct component*)collider));
    if (!share) {
        return;
    }
    // A subscriber could have destroyed the other side, and its on_disable did not know about this one yet
    if (!component_is_alive((struct component*)collider)) {
        collider_handle_on_exit(this, collider, false);
        return;
    }
    collider_handle_on_enter(collider, this, false);
}

static void collider_handle_on_exit(struct collider *this, struct collider *collider, const bool share) {
    if (this->intersections == NULL || dictionary_remove(this->intersections, collider) == 0) {
        return;
    }
    logger_debug("Entity %s on exit %s!", component_get_global_parent_name((struct component*)this), component_get_global_parent_name((struct component*)collider));
    action_invoke(&this->on_exit, component_get_global_parent((struct component*)collider));
    if (share) {
        collider_handle_on_exit(collider, this, false);
    }
}

static void collider_check_new_intersections(struct collider *this, const struct rect rect, const struct dictionary *colliders) {
    struct dictionary_iterator iterator = dictionary_begin(colliders);
    struct dictionary_node node;
    while (dictionary_next(colliders, &iterator, &node)) {
        struct collider *collider = node.value;
        if (collider == this || collider_get_response(this, collider) == collision_response_ignore) {
            continue;
        }
        const struct rect collider_rect = component_get_rect((struct component*)collider);
        if (rects_intersection(rect, collider_rect)) {
            collider_handle_on_enter(this, collider, true);
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
            collider_handle_on_exit(this, collider, true);
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
        collider_handle_on_exit(this, node.value, true);
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
            const struct dictionary *colliders = chunks_chunk_get_components_by_type(this->chunks, x, y, collider_component_key());
            if (colliders == NULL) {
                continue;
            }

            collider_check_new_intersections(this, rect, colliders);
        }
    }
}

static struct entity* collider_try_get_obstacle_among(const struct collider *this, const struct rect rect, const struct dictionary *colliders) {
    const struct entity* current_parent = component_get_global_parent((const struct component*)this);
    const struct rect current_rect = component_get_rect((const struct component*)this);

    struct dictionary_iterator iterator = dictionary_begin(colliders);
    struct dictionary_node node;
    while (dictionary_next(colliders, &iterator, &node)) {
        const struct collider *collider = node.value;
        if (collider_get_response(this, collider) != collision_response_block || component_get_global_parent((const struct component*)collider) == current_parent) {
            continue;
        }
        const struct rect collider_rect = component_get_rect((const struct component*)collider);
        // A collider we already intersect does not block, otherwise there would be no way out of it
        if (rects_intersection(rect, collider_rect) && !rects_intersection(current_rect, collider_rect)) {
            return component_get_global_parent((const struct component*)collider);
        }
    }
    return NULL;
}

struct entity* collider_try_get_obstacle(const struct collider *this, const struct rect rect) {
    int x_start, x_end, y_start, y_end;
    chunks_get_rect_indexes(this->chunks, rect, &x_start, &x_end, &y_start, &y_end);

    for (int x = x_start; x <= x_end; x++) {
        for (int y = y_start; y <= y_end; y++) {
            const struct dictionary *colliders = chunks_chunk_get_components_by_type(this->chunks, x, y, collider_component_key());
            if (colliders == NULL) {
                continue;
            }

            struct entity *obstacle = collider_try_get_obstacle_among(this, rect, colliders);
            if (obstacle != NULL) {
                return obstacle;
            }
        }
    }
    return NULL;
}





const struct rigid_material* collider_get_rigid_material(const struct collider *this) {
    if (this->material) {
        return this->material;
    }
    return &rigid_material_default;
}
struct action* collider_on_enter(struct collider *this) {
    return &this->on_enter;
}
struct action* collider_on_exit(struct collider *this) {
    return &this->on_exit;
}