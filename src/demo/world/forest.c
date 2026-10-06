#include "forest.h"

#include <stdlib.h>
#include <string.h>

#include "../../scene/components/component_internal.h"
#include "../../scene/entity.h"
#include "../../scene/scene.h"
#include "../../logging/logger.h"
#include "../../random/random.h"
#include "../../utils/list.h"
#include "../../utils/fields.h"
#include "../../utils/vector2_math.h"
#include "../../scene/prefabs/prefab.h"
#include "../../scene/prefabs/prefabs.h"


struct forest {
    struct component base;

    int trees_number;
    struct list prefabIds;
};

static void forest_on_create(struct component *base, struct fields *fields);
static void forest_awake(struct component *base);
static void forest_on_destroy(struct component *base);

const struct component_vtable forest_vtable = {
    .component_key = forest_component_key,
    .size = sizeof(struct forest),
    .on_create = forest_on_create,
    .on_awake = forest_awake,
    .on_update = NULL,
    .on_destroy = forest_on_destroy
};

const char* forest_component_key(void) {
    return "forest";
}

static void forest_on_create(struct component *base, struct fields *fields) {
    struct forest *this = (struct forest *) base;

    this->trees_number = fields_get_int(fields, "count", 0);

    this->prefabIds = list_create(1);
    const struct fields_list *prefabs = fields_get_list(fields, "prefabs");
    for (int i = 0; i < fields_list_count(prefabs); i++) {
        list_add(&this->prefabIds, strdup(fields_key(fields_list_get(prefabs, i))));
    }
}

static void forest_awake(struct component *base) {
    const struct forest *this = (struct forest *) base;

    struct list prefabs = list_create(list_count(&this->prefabIds));
    for (int i = 0; i < list_count(&this->prefabIds); i++) {
        const char* prefab_id = list_get(&this->prefabIds, i);
        struct prefab *prefab = prefabs_get(prefab_id);
        if (prefab == NULL) {
            continue;
        }
        list_add(&prefabs, prefab);
    }

    if (list_count(&prefabs) == 0) {
        logger_warn("Forest found no valid prefabs. Nothing will be spawned!");
        list_destroy(&prefabs);
        return;
    }

    logger_debug("Forest is spawning %d trees (%d different tree specs)...", this->trees_number, list_count(&prefabs));

    const struct rect rect = component_get_rect(base);
    const struct vector2 half_size = vector_multiply_scalar(rect.size, 0.5);

    for (int i = 0; i < this->trees_number; i++) {
        const int tree_spec = random_next_int(0, list_count(&prefabs));
        struct prefab* prefab = list_get(&prefabs, tree_spec);
        struct entity* tree = prefab_instantiate(prefab);

        struct rect tree_rect = entity_get_local_rect(tree);
        const struct vector2 tree_half_size = vector_multiply_scalar(tree_rect.size, 0.5);

        tree_rect.position.x = (float)random_next_double(rect.position.x - half_size.x + tree_half_size.x, rect.position.x + half_size.x - tree_half_size.x);
        tree_rect.position.y = (float)random_next_double(rect.position.y - half_size.y + tree_half_size.y, rect.position.y + half_size.y - tree_half_size.y);
        entity_set_local_rect(tree, tree_rect);

        scene_capture_entity(tree);
    }

    list_destroy(&prefabs);
}

static void forest_on_destroy(struct component *base) {
    struct forest *this = (struct forest *) base;
    for (int i = 0; i < list_count(&this->prefabIds); i++) {
        free(list_get(&this->prefabIds, i));
    }
    list_destroy(&this->prefabIds);
}
