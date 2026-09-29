#include "forest.h"

#include <stdlib.h>
#include <string.h>

#include "../component_internal.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../logging/logger.h"
#include "../../../random/random.h"
#include "../../../rendering/renderer_pipeline.h"
#include "../../../utils/list.h"
#include "../../../utils/parser.h"
#include "../../prefabs/prefab_manager.h"
#include "../../../subsystems/subsystem_collection.h"
#include "../../../utils/vector2_math.h"
#include "../../prefabs/prefab.h"
#include "../core/transform.h"


struct forest {
    struct component base;

    int trees_number;
    struct list* prefabIds;
};

static struct component* forest_clone(struct component base, const struct component *component);
static void forest_awake(struct component *base);
static void forest_on_destroy(struct component *base);

static const struct component_vtable forest_vtable = {
    .component_key = forest_component_key,
    .on_clone = forest_clone,
    .on_awake = forest_awake,
    .on_update = NULL,
    .on_disable = forest_on_destroy
};

const char* forest_component_key(void) {
    return "forest";
}

struct component* forest_create(struct parser *parser, struct entity *parent) {
    struct forest *this = calloc(1, sizeof(struct forest));
    struct component *base = (struct component *) this;
    component_base_create(base, &forest_vtable, parser, parent);

    parser_next_int(parser, &this->trees_number);

    this->prefabIds = list_create(1);
    for (; ;) {
        const char *word = parser_next(parser);
        if (word == NULL || strcmp(word, "end") == 0) {
            break;
        }
        list_add(this->prefabIds, strdup(word));
    }

    return base;
}

static struct component* forest_clone(struct component base, const struct component *component) {
    struct forest *forest = (struct forest *) component;

    struct forest* this = calloc(1, sizeof(struct forest));
    this->base = base;
    this->trees_number = forest->trees_number;
    this->prefabIds = list_create(list_count(forest->prefabIds));
    for (int i = 0; i < list_count(forest->prefabIds); i++) {
        list_add(this->prefabIds, strdup(list_get(forest->prefabIds, i)));
    }
    return (struct component*)this;
}

static void forest_awake(struct component *base) {
    const struct forest *this = (struct forest *) base;

    struct scene *scene = entity_get_parent(component_get_parent(base));
    const struct prefab_manager* prefab_manager = scene_get_prefab_manager(scene);
    struct random* random = (struct random*)subsystem_collection_get(scene_get_subsystems(scene), "random");

    struct list* prefabs = list_create(list_count(this->prefabIds));
    for (int i = 0; i < list_count(this->prefabIds); i++) {
        const char* prefab_id = list_get(this->prefabIds, i);
        struct prefab *prefab = prefab_manager_try_get(prefab_manager, prefab_id);
        if (prefab == NULL) {
            continue;
        }
        list_add(prefabs, prefab);
    }

    if (list_count(prefabs) == 0) {
        logger_warn("Forest found no valid prefabs. Nothing will be spawned!");
        list_destroy(prefabs);
        return;
    }

    logger_debug("Forest is spawning %d trees (%d different tree specs)...", this->trees_number, list_count(prefabs));

    const struct rect rect = component_get_rect(base);
    const struct vector2 half_size = vector_multiply_scalar(rect.size, 0.5);

    for (int i = 0; i < this->trees_number; i++) {
        const int tree_spec = random_next_int(random, 0, list_count(prefabs));
        struct prefab* prefab = list_get(prefabs, tree_spec);
        struct entity* tree = prefab_instantiate(prefab);
        struct transform* tree_transform = entity_get_transform(tree);

        struct rect tree_rect = transform_get_rect(tree_transform);
        const struct vector2 tree_half_size = vector_multiply_scalar(tree_rect.size, 0.5);

        tree_rect.position.x = (float)random_next_double(random, rect.position.x - half_size.x + tree_half_size.x, rect.position.x + half_size.x - tree_half_size.x);
        tree_rect.position.y = (float)random_next_double(random, rect.position.y - half_size.y + tree_half_size.y, rect.position.y + half_size.y - tree_half_size.y);
        transform_set_rect(tree_transform, tree_rect);

        scene_capture_entity(scene, tree);
    }

    list_destroy(prefabs);
}

static void forest_on_destroy(struct component *base) {
    const struct forest *this = (struct forest *) base;
    for (int i = 0; i < list_count(this->prefabIds); i++) {
        free(list_get(this->prefabIds, i));
    }
    list_destroy(this->prefabIds);
}
