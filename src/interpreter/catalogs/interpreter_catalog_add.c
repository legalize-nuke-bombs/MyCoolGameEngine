#include "interpreter_catalog_add.h"

#include <stdlib.h>

#include "../interpreter_command_internal.h"
#include "../../catalogs/catalog.h"
#include "../../catalogs/catalogs.h"
#include "../../subsystems/subsystem_collection.h"
#include "../../utils/parser.h"


struct interpreter_catalog_add {
    struct interpreter_command base;
};

static const char* interpreter_catalog_add_get_key(const struct interpreter_command *this) {
    return "add";
}

static void interpreter_catalog_add_execute(const struct interpreter_command *this, struct parser *parser, const struct subsystem_collection *subsystems) {
    const struct catalogs *catalogs = (struct catalogs*)subsystem_collection_get(subsystems, "catalogs");
    struct catalog *catalog = catalogs_get(catalogs, parser_next(parser));
    if (catalog == NULL) {
        return;
    }
    catalog_add(catalog, parser, subsystems);
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
