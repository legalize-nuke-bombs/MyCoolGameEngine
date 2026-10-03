//
// Created by nikita on 03.10.2026.
//

#include "printer.h"

#include <stdlib.h>

#include "../skill_internal.h"
#include "../../../../../logging/logger.h"

struct printer {
    struct skill base;
};

static bool printer_invoke(struct skill* base);

static const struct skill_vtable printer_vtable = {
    .key = "printer",
    .on_invoke = printer_invoke
};

struct skill* printer_create(struct entity *self) {
    struct printer* this = calloc(1, sizeof(struct printer));
    struct skill* base = (struct skill*)this;
    skill_base_create(base, &printer_vtable, 100, 2.5, self);
    return base;
}

static bool printer_invoke(struct skill* base) {
    logger_info("1000 - 7 = ?");
    return true;
}