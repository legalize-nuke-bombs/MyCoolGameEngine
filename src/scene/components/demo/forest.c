#include "forest.h"

#include <stdlib.h>
#include <string.h>

#include "../component_internal.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../rendering/renderer_pipeline.h"
#include "../../../utils/list.h"
#include "../../../utils/parser.h"
#include "../../prefabs/prefab_manager.h"


struct forest {
    struct component base;

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
    return (struct component*)this;
}

static void forest_awake(struct component *base) {
    const struct forest *this = (struct forest *) base;

    struct scene *scene = entity_get_parent(component_get_parent(base));
    const struct prefab_manager* prefabs = scene_get_prefab_manager(scene);

    for (int i = 0; i < list_count(this->prefabIds); i++) {
        const char* prefab_id = list_get(this->prefabIds, i);
        struct entity* entity = prefab_manager_instantiate(prefabs, prefab_id);
        scene_capture_entity(scene, entity);
    }
}

static void forest_on_destroy(struct component *base) {
    const struct forest *this = (struct forest *) base;
    for (int i = 0; i < list_count(this->prefabIds); i++) {
        free(list_get(this->prefabIds, i));
    }
    list_destroy(this->prefabIds);
}
