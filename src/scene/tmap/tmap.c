//
// Created by nikita on 22.09.2026.
//

#include "tmap.h"

#include <stdlib.h>

#include "../../utils/action.h"
#include "../../utils/dictionary.h"
#include "../../utils/pointer_dictionary.h"
#include "../../utils/string_dictionary.h"
#include "../../logging/logger.h"


struct tmap {
    struct dictionary *types;
};

struct tmap* tmap_create(void) {
    logger_info("TMap is creating...");
    struct tmap *this = malloc(sizeof(struct tmap));
    this->types = string_dictionary_build(4);
    return this;
}
void tmap_destroy(struct tmap *this) {
    logger_info("TMap is destroying...");
    struct dictionary_iterator iterator = dictionary_begin(this->types);
    struct dictionary_node node;
    while (dictionary_next(this->types, &iterator, &node)) {
        dictionary_destroy(node.value);
    }
    dictionary_destroy(this->types);
    free(this);
}

void tmap_update(const struct tmap *this, const struct update_context *context) {
    struct dictionary_iterator types_iterator = dictionary_begin(this->types);
    struct dictionary_node type_node;
    while (dictionary_next(this->types, &types_iterator, &type_node)) {
        const struct dictionary *components = type_node.value;
        struct dictionary_iterator iterator = dictionary_begin(components);
        struct dictionary_node node;
        while (dictionary_next(components, &iterator, &node)) {
            struct component *component = node.value;
            if (!component_is_updateable(component)) {
                break;
            }
            component_update(component, context);
        }
    }
}

void tmap_clear(const struct tmap *this) {
    struct dictionary_iterator iterator = dictionary_begin(this->types);
    struct dictionary_node node;
    while (dictionary_next(this->types, &iterator, &node)) {
        dictionary_clear(node.value);
    }
}

static void handle_component_marked_destroyed(void *listener, void *context) {
    const struct tmap *this = listener;
    struct component *component = context;
    struct dictionary *components = dictionary_get(this->types, (void*) component_get_key(component));
    if (components != NULL) {
        dictionary_remove(components, component);
    }
}

void tmap_register_component(const struct tmap *this, struct component *component) {
    if (!component_is_alive(component)) {
        return;
    }
    const char* component_key = component_get_key(component);

    struct dictionary *components = dictionary_get(this->types, (void*) component_key);
    if (components == NULL) {
        components = pointer_dictionary_build(0);
        if (!dictionary_try_add(this->types, (void*) component_key, components)) {
            dictionary_destroy(components);
            logger_error("Failed to populate tmap for component key %s", component_key);
            return;
        }
        logger_debug("TMap now knows component key %s", component_key);
    }

    if (dictionary_try_add(components, component, component)) {
        unsigned int subscription_token; // We do not unsubscribe because tmap always lives longer than components
        action_subscribe(component_get_on_marked_destroyed(component), (void*) this, handle_component_marked_destroyed, &subscription_token);
        logger_debug("TMap registered sample of %s", component_key);
    }
}

const struct dictionary* tmap_try_get_components(const struct tmap *this, const char *component_key) {
    return dictionary_get(this->types, (void*) component_key);
}
