//
// Created by nikita on 03.10.2026.
//

#include "factories.h"

#include <stdlib.h>

#include "../utils/dictionary.h"
#include "../utils/string_dictionary.h"
#include "../subsystems/subsystem_internal.h"
#include "factory.h"
#include "../logging/logger.h"
#include "../modules/bt/bt_node_factory.h"
#include "../scene/components/component_factory.h"
#include "../demo/characters/skills/skill_factory.h"

struct factories {
    struct subsystem base;

    struct dictionary *dict;
};

static const char* factories_get_name() {
    return "factories";
}
static void factories_on_destroy(struct subsystem *base);

static const struct subsystem_vtable factories_vtable = {
    .name = factories_get_name,
    .on_destroy = factories_on_destroy
};

static void factories_register_factory(const struct factories *this, struct factory *factory) {
    const char* factory_key = factory_get_key(factory);
    if (dictionary_try_add(this->dict, (void*)factory_key, factory)) {
        logger_debug("Factories registered factory %s", factory_key);
    }
    else {
        logger_error("Factories failed to register factory %s", factory_key);
    }
}

static void factories_register_all(const struct factories *this) {
    // Core factories
    factories_register_factory(this, component_factory_create());

    // Bt module factories
    factories_register_factory(this, bt_node_factory_create());

    // Demo factories
    factories_register_factory(this, skill_factory_create());
}

struct subsystem* factories_create(const struct subsystem_collection *subsystems) {
    struct factories *this = calloc(1, sizeof(struct factories));
    struct subsystem *base = (struct subsystem*)this;
    subsystem_create(base, &factories_vtable, subsystems);
    this->dict = string_dictionary_build(4);
    factories_register_all(this);
    return base;
}

static void factories_on_destroy(struct subsystem *base) {
    const struct factories *this = (struct factories*)base;
    struct dictionary_iterator iterator = dictionary_begin(this->dict);
    struct dictionary_node node;
    while (dictionary_next(this->dict, &iterator, &node)) {
        factory_destroy(node.value);
    }
    dictionary_destroy(this->dict);
}

struct factory* factories_get(const struct factories *this, const char *key) {
    struct factory* factory = dictionary_get(this->dict, (void*)key);
    if (factory == NULL) {
        logger_error("Factories do not know factory %s", key);
        return NULL;
    }
    return factory;
}