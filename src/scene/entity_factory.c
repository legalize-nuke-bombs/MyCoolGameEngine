#include "entity_factory.h"

#include <string.h>

#include "entity.h"
#include "components/component_factory.h"
#include "../logging/logger.h"
#include "../utils/fields.h"


struct entity* entity_factory_produce(struct fields *fields) {
    struct entity *entity = entity_create(fields_dup_string(fields, "name", "Entity"), NULL);

    const struct rect rect = {
        .position = fields_get_vector2(fields, "position", vector2_zero),
        .size = fields_get_vector2(fields, "size", vector2_one)
    };
    entity_set_local_rect(entity, rect);

    const struct fields_list *components = fields_get_list(fields, "components");
    for (int i = 0; i < fields_list_count(components); i++) {
        struct fields *component_fields = fields_list_get(components, i);
        struct component *component = component_factory_produce(fields_key(component_fields), component_fields, entity);
        if (component != NULL) {
            entity_capture_component(entity, component);
        }
    }

    const struct fields_list *children = fields_get_list(fields, "children");
    for (int i = 0; i < fields_list_count(children); i++) {
        struct fields *child_fields = fields_list_get(children, i);
        if (strcmp(fields_key(child_fields), "entity") != 0) {
            logger_warn("Entity %s (line %d) expected entity in field `children`, got %s", entity_get_name(entity), fields_line(child_fields), fields_key(child_fields));
            continue;
        }
        entity_capture_entity(entity, entity_factory_produce(child_fields));
    }

    fields_warn_unknown(fields);
    return entity;
}
