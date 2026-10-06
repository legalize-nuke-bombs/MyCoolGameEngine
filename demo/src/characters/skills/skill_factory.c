//
// Created by nikita on 03.10.2026.
//

#include "skill_factory.h"

#include <stdlib.h>
#include <string.h>

#include "skill_internal.h"
#include "logging/logger.h"
#include "utils/fields.h"
#include "custom/fear_balls/fear_balls.h"
#include "custom/printer.h"

static const struct skill_vtable *skills[] = {
    &printer_vtable,
    &fear_balls_vtable,
};

struct skill* skill_factory_produce(struct fields *fields, struct entity *self) {
    const char *key = fields_key(fields);
    for (size_t i = 0; i < sizeof(skills) / sizeof(skills[0]); i++) {
        const struct skill_vtable *vtable = skills[i];
        if (strcmp(vtable->key, key) != 0) {
            continue;
        }
        struct skill *skill = calloc(1, vtable->size);
        skill_base_create(skill, vtable, fields_get_double(fields, "manacost", 0), fields_get_double(fields, "cooldown", 0), self);
        if (vtable->on_create) {
            vtable->on_create(skill, fields);
        }
        fields_warn_unknown(fields);
        return skill;
    }
    logger_warn("Skill factory does not know %s", key);
    return NULL;
}
