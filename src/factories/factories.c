//
// Created by nikita on 03.10.2026.
//

#include "factories.h"

#include <stdlib.h>

#include "../utils/dictionary.h"
#include "../utils/string_dictionary.h"
#include "../msystems/msystem.h"
#include "factory.h"
#include "../logging/logger.h"
#include "../modules/bt/bt_node_factory.h"
#include "../scene/components/component_factory.h"
#include "../demo/characters/skills/skill_factory.h"

static struct {
    struct dictionary *dict;
} factories;

static void factories_register_factory(struct factory *factory) {
    const char* factory_key = factory_get_key(factory);
    if (dictionary_try_add(factories.dict, (void*)factory_key, factory)) {
        logger_debug("Factories registered factory %s", factory_key);
    }
    else {
        logger_error("Factories failed to register factory %s", factory_key);
    }
}

static void factories_register_all(void) {
    // Core factories
    factories_register_factory(component_factory_create());

    // Bt module factories
    factories_register_factory(bt_node_factory_create());

    // Demo factories
    factories_register_factory(skill_factory_create());
}

static void factories_on_create(void) {
    factories.dict = string_dictionary_build(4);
    factories_register_all();
}

static void factories_on_destroy(void) {
    struct dictionary_iterator iterator = dictionary_begin(factories.dict);
    struct dictionary_node node;
    while (dictionary_next(factories.dict, &iterator, &node)) {
        factory_destroy(node.value);
    }
    dictionary_destroy(factories.dict);
}

const struct msystem factories_msystem = {
    .name = "factories",
    .on_create = factories_on_create,
    .on_destroy = factories_on_destroy
};

struct factory* factories_get(const char *key) {
    struct factory* factory = dictionary_get(factories.dict, (void*)key);
    if (factory == NULL) {
        logger_error("Factories do not know factory %s", key);
        return NULL;
    }
    return factory;
}
