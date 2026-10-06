#include "interpreter_catalog_add.h"

#include <stdlib.h>

#include "../interpreter_command_internal.h"
#include "../../assets/assets.h"
#include "../../utils/parser.h"


struct interpreter_catalog_add {
    struct interpreter_command base;
};

static const char* interpreter_catalog_add_get_key(const struct interpreter_command *this) {
    return "add";
}

static void interpreter_catalog_add_execute(const struct interpreter_command *this, struct parser *parser) {
    assets_add(parser_next(parser), parser);
}

static const struct interpreter_command_vtable catalog_add_vtable = {
    .key = interpreter_catalog_add_get_key,
    .execute = interpreter_catalog_add_execute
};

struct interpreter_command* interpreter_catalog_add_create() {
    struct interpreter_catalog_add *this = calloc(1, sizeof(struct interpreter_catalog_add));
    this->base.vtable = &catalog_add_vtable;
    return (struct interpreter_command*)this;
}
