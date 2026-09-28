//
// Created by nikita on 28.09.2026.
//

#include "prefab_manager.h"

#include <stdlib.h>

#include "../../utils/dictionary.h"
#include "../../utils/string_dictionary.h"
#include "prefab.h"
#include "../../logging/logger.h"


struct prefab_manager {
    struct dictionary* dict;
};


struct prefab_manager* prefab_manager_create() {
    logger_info("Prefab manager is creating...");
    struct prefab_manager* this = calloc(1, sizeof(struct prefab_manager));
    this->dict = string_dictionary_build(10);
    return this;
}
void prefab_manager_destroy(struct prefab_manager* this) {
    logger_info("Prefab manager is destroying...");
    prefab_manager_clear(this);
    dictionary_destroy(this->dict);
    free(this);
}

void prefab_manager_capture_prefab(const struct prefab_manager* this, struct prefab* prefab) {
    const char* prefab_key = prefab_get_name(prefab);
    if (!dictionary_try_add(this->dict, (void*)prefab_key, prefab)) {
        logger_error("Prefab manager failed to capture prefab %s", prefab_key);
        prefab_destroy(prefab);
    }
}
struct entity* prefab_manager_instantiate(const struct prefab_manager* this, const char* key) {
    struct prefab* prefab = dictionary_get(this->dict, (void*)key);
    if (prefab == NULL) {
        logger_error("Prefab manager failed to find prefab %s", key);
        return NULL;
    }
    return prefab_instantiate(prefab);
}

void prefab_manager_clear(const struct prefab_manager* this) {
    logger_info("Prefab manager is clearing...");
    for (int i = 0; i < dictionary_capacity(this->dict); i++) {
        const struct dictionary_node node = dictionary_get_node(this->dict, i);
        if (node.value != NULL) {
            prefab_destroy(node.value);
        }
    }
    dictionary_clear(this->dict);
}