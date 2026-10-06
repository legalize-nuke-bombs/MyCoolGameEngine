#include "collision_rules.h"

#include <stdint.h>
#include <string.h>

#include "collision_layer.h"
#include "collision_layers.h"
#include "../../assets/asset_type.h"
#include "../../logging/logger.h"
#include "../../utils/fields.h"

// The id of a rule is an unordered pair of layers, so a rule lives in two cells: [layer1][layer2] and [layer2][layer1]
static struct {
    // 0 - the pair has no rule, otherwise the response of the rule + 1
    uint8_t cells[COLLISION_LAYERS_MAX][COLLISION_LAYERS_MAX];
} collision_rules;

static void collision_rules_on_add(struct fields *fields) {
    const struct fields_list *between = fields_get_list(fields, "between");
    const char *response_name = fields_get_string(fields, "response", NULL);

    if (fields_list_count(between) != 2) {
        logger_warn("Collision rule expected two layers in field `between`, got %d", fields_list_count(between));
        return;
    }
    const char *name1 = fields_key(fields_list_get(between, 0));
    const char *name2 = fields_key(fields_list_get(between, 1));

    enum collision_response response;
    if (!collision_response_try_parse(response_name, &response)) {
        logger_warn("Collision rule between %s and %s got unexpected response `%s`", name1, name2, response_name ? response_name : "<null>");
        return;
    }
    const struct collision_layer *layer1 = collision_layers_get(name1);
    const struct collision_layer *layer2 = collision_layers_get(name2);
    if (layer1 == NULL || layer2 == NULL) {
        return;
    }

    const uint8_t index1 = collision_layer_get_index(layer1);
    const uint8_t index2 = collision_layer_get_index(layer2);
    if (collision_rules.cells[index1][index2] != 0) {
        logger_warn("Collision rule between %s and %s is already there", name1, name2);
        return;
    }
    collision_rules.cells[index1][index2] = (uint8_t)(response + 1);
    collision_rules.cells[index2][index1] = (uint8_t)(response + 1);
    logger_debug("Collision rule between %s and %s is %s", name1, name2, response_name);
}
static void collision_rules_on_clear(void) {
    memset(collision_rules.cells, 0, sizeof(collision_rules.cells));
}

const struct asset_type collision_rules_asset_type = {
    .key = "collision_rule",
    .on_add = collision_rules_on_add,
    .on_clear = collision_rules_on_clear
};

enum collision_response collision_rules_get_response(const struct collision_layer *layer1, const struct collision_layer *layer2) {
    const uint8_t cell = collision_rules.cells[collision_layer_get_index(layer1)][collision_layer_get_index(layer2)];
    if (cell != 0) {
        return (enum collision_response)(cell - 1);
    }
    const enum collision_response default1 = collision_layer_get_default_response(layer1);
    const enum collision_response default2 = collision_layer_get_default_response(layer2);
    return default1 < default2 ? default1 : default2;
}
