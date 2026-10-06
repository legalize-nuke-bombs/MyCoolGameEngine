#include "entity_parser.h"

#include <string.h>

#include "entity.h"
#include "scene.h"
#include "../factories/factories.h"
#include "components/component_factory.h"
#include "../utils/parser.h"


struct entity* entity_parse(struct parser *parser, char *name) {
    struct entity *entity = entity_create(name, NULL);
    entity_set_in_scene(entity, true);

    struct rect rect;
    parser_next_double(parser, &rect.position.x);
    parser_next_double(parser, &rect.position.y);
    parser_next_double(parser, &rect.size.x);
    parser_next_double(parser, &rect.size.y);
    entity_set_local_rect(entity, rect);

    const struct component_factory *component_factory = (struct component_factory*)factories_get("component_factory");

    for (;;) {
        const char* word = parser_next(parser);
        if (word == NULL || strcmp(word, "end") == 0) {
            break;
        }

        if (strcmp(word, "child") == 0 || strcmp(word, "entity") == 0) {
            struct entity *child = entity_parse(parser, parser_next_dup(parser));
            if (child != NULL) {
                entity_capture_entity(entity, child);
            }
            continue;
        }

        struct component* component = component_factory_produce(component_factory, word, parser, entity);
        if (component != NULL) {
            entity_capture_component(entity, component);
        }
    }

    return entity;
}
