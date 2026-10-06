//
// Created by nikita on 01.10.2026.
//

#include "rigid_material.h"

#include <stdlib.h>

#include "../../assets/asset_storage.h"
#include "../../assets/asset_type.h"
#include "../../utils/parser.h"



static void rigid_material_destroy_item(void *item) {
    free(item);
}

static struct asset_storage rigid_materials = {
    ._key = "rigid_material",
    ._destroy_item = rigid_material_destroy_item
};

static void rigid_material_asset_on_add(struct parser *parser) {
    char *name = asset_storage_parse_name(&rigid_materials, parser);
    if (name == NULL) {
        return;
    }
    struct rigid_material *this = calloc(1, sizeof(struct rigid_material));
    parser_next_double(parser, &this->_friction);
    parser_next_double(parser, &this->_restitution);
    asset_storage_add(&rigid_materials, name, this);
}
static void rigid_material_asset_on_clear(void) {
    asset_storage_clear(&rigid_materials);
}
static void rigid_material_asset_on_destroy(void) {
    asset_storage_destroy(&rigid_materials);
}

const struct asset_type rigid_material_asset_type = {
    .key = "rigid_material",
    .on_add = rigid_material_asset_on_add,
    .on_clear = rigid_material_asset_on_clear,
    .on_destroy = rigid_material_asset_on_destroy
};

struct rigid_material* rigid_material_asset_get(const char *name) {
    return asset_storage_get(&rigid_materials, name);
}

double rigid_material_get_friction(const struct rigid_material *this) {
    return this->_friction;
}
double rigid_material_get_restitution(const struct rigid_material *this) {
    return this->_restitution;
}

const struct rigid_material rigid_material_default = {
    ._friction = 0.5f,
    ._restitution = 0.5f,
};
