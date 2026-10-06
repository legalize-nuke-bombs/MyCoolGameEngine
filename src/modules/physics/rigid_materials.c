#include "rigid_materials.h"

#include "rigid_material.h"
#include "../../assets/asset_storage.h"
#include "../../assets/asset_type.h"
#include "../../utils/fields.h"

static void rigid_materials_destroy_item(void *item) {
    rigid_material_destroy(item);
}

static struct asset_storage rigid_materials = {
    ._key = "rigid_material",
    ._destroy_item = rigid_materials_destroy_item
};

static void rigid_materials_on_add(struct fields *fields) {
    const double friction = fields_get_double(fields, "friction", rigid_material_get_friction(&rigid_material_default));
    const double restitution = fields_get_double(fields, "restitution", rigid_material_get_restitution(&rigid_material_default));
    asset_storage_add(&rigid_materials, fields_get_string(fields, "name", NULL), rigid_material_create(friction, restitution));
}
static void rigid_materials_on_clear(void) {
    asset_storage_clear(&rigid_materials);
}
static void rigid_materials_on_destroy(void) {
    asset_storage_destroy(&rigid_materials);
}

const struct asset_type rigid_materials_asset_type = {
    .key = "rigid_material",
    .on_add = rigid_materials_on_add,
    .on_clear = rigid_materials_on_clear,
    .on_destroy = rigid_materials_on_destroy
};

struct rigid_material* rigid_materials_get(const char *name) {
    return asset_storage_get(&rigid_materials, name);
}
