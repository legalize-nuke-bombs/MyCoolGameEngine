//
// Created by Nikita on 03.10.2026.
//

#include "bt_node_factory.h"

#include <stdlib.h>

#include "../../demo/bt/player_chase.h"
#include "../../logging/logger.h"
#include "../../utils/dictionary.h"
#include "../../utils/string_dictionary.h"
#include "core/bt_selector.h"
#include "core/bt_sequence.h"
#include "../../demo/bt/player_is_near.h"
#include "../../demo/bt/player_chase.h"

struct bt_node_factory {
    struct dictionary* dict;
};

static void bt_node_factory_register(const struct bt_node_factory *this, const char* key, struct bt_node* (*constructor)(struct parser *parser, const struct bt_node_factory *factory)) {
    if (dictionary_try_add(this->dict, (void*)key, constructor)) {
        logger_debug("Bt node factory registered %s", key);
    }
    else {
        logger_error("Bt node factory failed to register %s", key);
    }
}

static void bt_node_factory_register_all(const struct bt_node_factory *this) {
    // Core
    bt_node_factory_register(this, BT_NODE_SEQUENCE, bt_sequence_parse);
    bt_node_factory_register(this, BT_NODE_SELECTOR, bt_selector_parse);

    // Demo
    bt_node_factory_register(this, BT_NODE_PLAYER_IS_NEAR, player_is_near_parse);
    bt_node_factory_register(this, BT_NODE_PLAYER_CHASE, player_chase_parse);

    logger_info("Bt node factory knows %d nodes", dictionary_count(this->dict));
}

struct bt_node_factory* bt_node_factory_create() {
    struct bt_node_factory* this = calloc(1, sizeof(struct bt_node_factory));
    this->dict = string_dictionary_build(3);
    bt_node_factory_register_all(this);
    return this;
}
void bt_node_factory_destroy(struct bt_node_factory *this) {
    dictionary_destroy(this->dict);
    free(this);
}

void bt_node_factory_clear(const struct bt_node_factory *this) {
    dictionary_clear(this->dict);
}

struct bt_node* bt_node_factory_try_produce_node(const struct bt_node_factory *this, const char *key, struct parser *parser) {
    struct bt_node* (*constructor)(struct parser *parser, const struct bt_node_factory *factory) = dictionary_get(this->dict, (void*)key);
    if (constructor == NULL) {
        logger_warn("Behaviour tree node factory does not know node %s", key);
        return NULL;
    }
    return constructor(parser, this);
}