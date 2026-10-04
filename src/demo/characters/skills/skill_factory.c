//
// Created by nikita on 03.10.2026.
//

#include "skill_factory.h"

#include <stdlib.h>

#include "../../../factories/factory_internal.h"
#include "custom/fear_balls.h"
#include "custom/printer.h"

struct skill_factory {
    struct factory base;
};

static const struct factory_vtable skill_factory_vtable = {
    .key = "skill_factory"
};

static void skill_factory_register_all(const struct factory *base) {
    factory_register(base, SKILL_PRINTER_KEY, printer_parse);
    factory_register(base, SKILL_FEAR_BALLS_KEY, fear_balls_parse);
}

struct factory* skill_factory_create() {
    struct skill_factory* this = calloc(1, sizeof(struct skill_factory));
    struct factory* base = (struct factory*)this;
    factory_base_create(base, &skill_factory_vtable);
    skill_factory_register_all(base);
    return base;
}

struct skill* skill_factory_produce(const struct skill_factory *this, const char* key, struct parser *parser, struct entity *parent) {
    struct skill* (*constructor)(struct parser *parser, struct entity *parent) = factory_get_constructor((struct factory*)this, key);
    if (constructor == NULL) {
        return NULL;
    }
    return constructor(parser, parent);
}