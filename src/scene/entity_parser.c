#include "entity_parser.h"

#include <string.h>

#include "entity.h"
#include "scene.h"
#include "components/component_fabric.h"
#include "../utils/parser.h"


struct entity* entity_parse(struct parser *parser, struct scene *scene, char *name) {
    struct entity *entity = entity_create(name, NULL);

    struct rect rect;
    parser_next_double(parser, &rect.position.x);
    parser_next_double(parser, &rect.position.y);
    parser_next_double(parser, &rect.size.x);
    parser_next_double(parser, &rect.size.y);
    entity_set_local_rect(entity, rect);

    const struct component_fabric *component_fabric = scene_get_component_fabric(scene);

    for (;;) {
        const char* word = parser_next(parser);
        if (word == NULL || strcmp(word, "end") == 0) {
            break;
        }

        if (strcmp(word, "child") == 0 || strcmp(word, "entity") == 0) {
            struct entity *child = entity_parse(parser, scene, parser_next_dup(parser));
            if (child != NULL) {
                entity_capture_entity(entity, child);
            }
            continue;
        }

        struct component* component = component_fabric_try_produce_component(component_fabric, word, parser, entity);
        if (component != NULL) {
            entity_capture_component(entity, component);
        }
    }

    return entity;
}
