//
// Created by nikita on 01.10.2026.
//

#include "rigid_material.h"

#include <stdlib.h>

#include "../catalog.h"
#include "../../utils/parser.h"



static const char* rigid_material_catalog_key(void) {
    return "rigid_material";
}

static void* rigid_material_on_create_item(const char *name, struct parser *parser, const struct subsystem_collection *subsystems) {
    struct rigid_material *this = calloc(1, sizeof(struct rigid_material));
    parser_next_double(parser, &this->_friction);
    parser_next_double(parser, &this->_restitution);
    return this;
}
static void rigid_material_on_destroy_item(void *item) {
    free(item);
}

const struct catalog_vtable rigid_material_catalog_vtable = {
    .key = rigid_material_catalog_key,
    .on_create_item = rigid_material_on_create_item,
    .on_destroy_item = rigid_material_on_destroy_item
};

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
