//
// Created by nikita on 01.10.2026.
//

#include "rigid_materials.h"

#include <stdlib.h>

#include "../../utils/dictionary.h"
#include "../../utils/string_dictionary.h"
#include "rigid_material.h"
#include "../../logging/logger.h"

struct rigid_materials {
    struct dictionary* dict;
};


struct rigid_materials* rigid_materials_create() {
    struct rigid_materials* this = calloc(1, sizeof(struct rigid_materials));
    this->dict = string_dictionary_build(3);
    return this;
}
void rigid_materials_destroy(struct rigid_materials* this) {
    rigid_materials_clear(this);
    dictionary_destroy(this->dict);
    free(this);
}

void rigid_materials_clear(const struct rigid_materials* this) {
    struct dictionary_iterator iterator;
    struct dictionary_node node;
    while (dictionary_next(this->dict, &iterator, &node)) {
        struct rigid_material* material = node.value;
        rigid_material_destroy(material);
    }
    dictionary_clear(this->dict);
}

void rigid_materials_capture(const struct rigid_materials* this, struct rigid_material* material) {
    const char* material_name = rigid_material_get_name(material);
    if (dictionary_try_add(this->dict, (void*)material_name, material)) {
        logger_debug("Rigid materials captured material %s", material_name);
    }
    else {
        logger_warn("Rigid materials failed to capture material %s", material_name);
        rigid_material_destroy(material);
    }
}
struct rigid_material* rigid_materials_get(const struct rigid_materials* this, const char *name) {
    return dictionary_get(this->dict, (void*)name);
}