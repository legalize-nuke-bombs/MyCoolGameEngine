//
// Created by nikita on 03.10.2026.
//

#include "printer.h"

#include <stdlib.h>

#include "../skill_internal.h"
#include "../../../../../logging/logger.h"
#include "../../../../../utils/parser.h"

struct printer {
    struct skill base;
    char *string;
};

static void printer_on_destroy(struct skill *base);
static bool printer_invoke(struct skill* base);

static const struct skill_vtable printer_vtable = {
    .key = "printer",
    .on_destroy = printer_on_destroy,
    .on_invoke = printer_invoke
};


struct skill* printer_parse(struct parser *parser, struct entity *self) {
    struct printer* this = calloc(1, sizeof(struct printer));
    struct skill* base = (struct skill*)this;
    skill_base_create(base, &printer_vtable, 100, 2.5, self);
    this->string = parser_next_dup(parser);
    return base;
}

static void printer_on_destroy(struct skill *base) {
    const struct printer* this = (struct printer*)base;
    free(this->string);
}

static bool printer_invoke(struct skill* base) {
    const struct printer* this = (struct printer*)base;
    logger_info("%s", this->string);
    return true;
}