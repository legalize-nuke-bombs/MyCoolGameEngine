#include "component_ids.h"

#include <stdlib.h>

#include "../scene.h"
#include "../components/component_internal.h"
#include "../../utils/action.h"
#include "../../utils/dictionary.h"
#include "../../utils/uint128_dictionary.h"
#include "../../logging/logger.h"


struct component_ids {
    struct dictionary *components;

    struct action* on_component_captured;
    unsigned int on_component_captured_subscription_token;

    struct action* on_component_marked_destroyed;
    unsigned int on_component_marked_destroyed_subscription_token;
};

static void handle_component_captured(void *listener, void *context);
static void handle_component_marked_destroyed(void *listener, void *context);

struct component_ids* component_ids_create(void) {
    logger_info("Component_ids are creating...");
    struct component_ids *this = calloc(1, sizeof(struct component_ids));
    this->components = uint128_dictionary_build(4);

    this->on_component_captured = scene_get_on_component_captured();
    action_subscribe(this->on_component_captured, this, handle_component_captured, &this->on_component_captured_subscription_token);

    this->on_component_marked_destroyed = scene_get_on_component_marked_destroyed();
    action_subscribe(this->on_component_marked_destroyed, this, handle_component_marked_destroyed, &this->on_component_marked_destroyed_subscription_token);

    return this;
}
void component_ids_destroy(struct component_ids *this) {
    logger_info("Component_ids are destroying...");
    action_unsubscribe(this->on_component_marked_destroyed, this->on_component_marked_destroyed_subscription_token);
    action_unsubscribe(this->on_component_captured, this->on_component_captured_subscription_token);
    dictionary_destroy(this->components);
    free(this);
}

void component_ids_clear(const struct component_ids *this) {
    dictionary_clear(this->components);
}

static void handle_component_captured(void *listener, void *context) {
    const struct component_ids *this = listener;
    struct component *component = context;

    if (!component_is_alive(component)) {
        return;
    }
    if (!dictionary_try_add(this->components, &component->id, component) && dictionary_get(this->components, &component->id) != component) {
        logger_warn("Component %s of entity %s has an id that is already taken, it will not be found by its id", component_get_key(component), component_get_parent_name(component));
    }
}

static void handle_component_marked_destroyed(void *listener, void *context) {
    const struct component_ids *this = listener;
    struct component *component = context;

    if (dictionary_get(this->components, &component->id) == component) {
        dictionary_remove(this->components, &component->id);
    }
}

struct component* component_ids_try_get(const struct component_ids *this, uint128_t id) {
    return dictionary_get(this->components, &id);
}
