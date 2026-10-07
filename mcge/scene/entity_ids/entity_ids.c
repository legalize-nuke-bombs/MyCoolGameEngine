#include "entity_ids.h"

#include <stdlib.h>

#include "../entity.h"
#include "../scene.h"
#include "../../utils/action.h"
#include "../../utils/dictionary.h"
#include "../../utils/uint128_dictionary.h"
#include "../../logging/logger.h"


// Every living entity of the scene by its id, the roots and the children alike.
// It owns nothing: a child belongs to its parent and a root to entity_collection
struct entity_ids {
    struct dictionary *entities;

    struct action* on_entity_captured;
    unsigned int on_entity_captured_subscription_token;

    struct action* on_entity_marked_destroyed;
    unsigned int on_entity_marked_destroyed_subscription_token;
};

static void handle_entity_captured(void *listener, void *context);
static void handle_entity_marked_destroyed(void *listener, void *context);

struct entity_ids* entity_ids_create(void) {
    logger_info("Entity_ids are creating...");
    struct entity_ids *this = calloc(1, sizeof(struct entity_ids));
    this->entities = uint128_dictionary_build(4);

    this->on_entity_captured = scene_get_on_entity_captured();
    action_subscribe(this->on_entity_captured, this, handle_entity_captured, &this->on_entity_captured_subscription_token);

    this->on_entity_marked_destroyed = scene_get_on_entity_marked_destroyed();
    action_subscribe(this->on_entity_marked_destroyed, this, handle_entity_marked_destroyed, &this->on_entity_marked_destroyed_subscription_token);

    return this;
}
void entity_ids_destroy(struct entity_ids *this) {
    logger_info("Entity_ids are destroying...");
    action_unsubscribe(this->on_entity_marked_destroyed, this->on_entity_marked_destroyed_subscription_token);
    action_unsubscribe(this->on_entity_captured, this->on_entity_captured_subscription_token);
    dictionary_destroy(this->entities);
    free(this);
}

void entity_ids_clear(const struct entity_ids *this) {
    dictionary_clear(this->entities);
}

static void handle_entity_captured(void *listener, void *context) {
    const struct entity_ids *this = listener;
    struct entity *entity = context;

    if (!entity_is_alive(entity)) {
        return;
    }
    // The key is the id inside the entity itself: it is valid until the entity is marked destroyed, and that is when it leaves
    if (!dictionary_try_add(this->entities, &entity->_id, entity) && dictionary_get(this->entities, &entity->_id) != entity) {
        logger_warn("Entity %s has an id that is already taken, it will not be found by its id", entity_get_name(entity));
    }
}

static void handle_entity_marked_destroyed(void *listener, void *context) {
    const struct entity_ids *this = listener;
    struct entity *entity = context;

    // Only the entity itself leaves: under a repeated id there is another entity, and it stays
    if (dictionary_get(this->entities, &entity->_id) == entity) {
        dictionary_remove(this->entities, &entity->_id);
    }
}

struct entity* entity_ids_try_get(const struct entity_ids *this, uint128_t id) {
    return dictionary_get(this->entities, &id);
}
