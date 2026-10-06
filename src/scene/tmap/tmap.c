//
// Created by nikita on 22.09.2026.
//

#include "tmap.h"

#include <stdlib.h>

#include "../scene.h"
#include "../../utils/action.h"
#include "../../utils/dictionary.h"
#include "../../utils/pointer_dictionary.h"
#include "../../utils/string_dictionary.h"
#include "../../logging/logger.h"


struct tmap {
    struct dictionary *types;

    struct action* on_component_captured;
    unsigned int on_component_captured_subscription_token;

    struct action* on_component_marked_destroyed;
    unsigned int on_component_marked_destroyed_subscription_token;
};

static void handle_component_captured(void *listener, void *context);
static void handle_component_marked_destroyed(void *listener, void *context);

struct tmap* tmap_create(void) {
    logger_info("TMap is creating...");
    struct tmap *this = calloc(1, sizeof(struct tmap));
    this->types = string_dictionary_build(4);

    this->on_component_captured = scene_get_on_component_captured();
    action_subscribe(this->on_component_captured, this, handle_component_captured, &this->on_component_captured_subscription_token);

    this->on_component_marked_destroyed = scene_get_on_component_marked_destroyed();
    action_subscribe(this->on_component_marked_destroyed, this, handle_component_marked_destroyed, &this->on_component_marked_destroyed_subscription_token);

    return this;
}
void tmap_destroy(struct tmap *this) {
    logger_info("TMap is destroying...");
    action_unsubscribe(this->on_component_marked_destroyed, this->on_component_marked_destroyed_subscription_token);
    action_unsubscribe(this->on_component_captured, this->on_component_captured_subscription_token);
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
            if (!component_is_awake(component)) {
                continue;
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

static void handle_component_captured(void *listener, void *context) {
    struct tmap *this = listener;
    struct component *component = context;

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
        logger_debug("TMap registered sample of %s", component_key);
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

const struct dictionary* tmap_try_get_components(const struct tmap *this, const char *component_key) {
    return dictionary_get(this->types, (void*) component_key);
}
