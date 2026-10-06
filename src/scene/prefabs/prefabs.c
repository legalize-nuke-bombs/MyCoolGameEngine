#include "prefabs.h"

#include "prefab.h"
#include "../../assets/asset_storage.h"
#include "../../assets/asset_type.h"
#include "../../utils/fields.h"

static void prefabs_destroy_item(void *item) {
    prefab_destroy(item);
}

static struct asset_storage prefabs = {
    ._key = "prefab",
    ._destroy_item = prefabs_destroy_item
};

static void prefabs_on_add(struct fields *fields) {
    asset_storage_add(&prefabs, fields_get_string(fields, "name", NULL), prefab_create(fields));
}
static void prefabs_on_clear(void) {
    asset_storage_clear(&prefabs);
}
static void prefabs_on_destroy(void) {
    asset_storage_destroy(&prefabs);
}

const struct asset_type prefabs_asset_type = {
    .key = "prefab",
    .on_add = prefabs_on_add,
    .on_clear = prefabs_on_clear,
    .on_destroy = prefabs_on_destroy
};

struct prefab* prefabs_get(const char *name) {
    return asset_storage_get(&prefabs, name);
}
