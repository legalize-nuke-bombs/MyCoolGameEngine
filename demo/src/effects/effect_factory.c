//
// Created by nikita on 09.10.2026.
//

#include "effect_factory.h"

#include <stdlib.h>
#include <string.h>

#include "effect_internal.h"
#include <mcge/mcge.h>
#include "custom/fear.h"

static const struct effect_vtable *effects[] = {
    &fear_vtable,
};

struct effect* effect_factory_produce(struct fields *fields, struct entity *self) {
    const char *key = fields_key(fields);
    for (size_t i = 0; i < sizeof(effects) / sizeof(effects[0]); i++) {
        const struct effect_vtable *vtable = effects[i];
        if (strcmp(vtable->key, key) != 0) {
            continue;
        }
        struct effect *effect = calloc(1, vtable->size);
        effect_base_create(
            effect,
            vtable,
            fields_get_double(fields, "duration", 0),
            self
            );
        if (vtable->on_create) {
            vtable->on_create(effect, fields);
        }
        fields_warn_unknown(fields);
        return effect;
    }
    logger_warn("Effect factory does not know %s", key);
    return NULL;
}
