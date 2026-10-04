//
// Created by Nikita on 04.10.2026.
//

#include "fear_balls.h"

#include <stdlib.h>

#include "../skill_internal.h"
#include "../../../../utils/parser.h"
#include "../../../../scene/prefabs/prefab.h"
#include "../../../../subsystems/subsystem_collection.h"
#include "../../../../catalogs/catalogs.h"
#include "../../../../logging/logger.h"
#include "../../../../scene/entity.h"
#include "../../../../scene/scene.h"

struct fear_balls {
    struct skill base;
    struct prefab* prefab;
};

static bool fear_balls_invoke(struct skill* base);

static const struct skill_vtable fear_balls_vtable = {
    .key = SKILL_FEAR_BALLS_KEY,
    .on_invoke = fear_balls_invoke
};


struct skill* fear_balls_parse(struct parser *parser, struct entity *self) {
    struct fear_balls* this = calloc(1, sizeof(struct fear_balls));
    struct skill* base = (struct skill*)this;
    skill_base_parse(base, &fear_balls_vtable, parser, self);
    const char* prefab_name = parser_next(parser);
    this->prefab = catalogs_get_item((struct catalogs*)subsystem_collection_get(scene_get_subsystems(entity_get_scene(self)), "catalogs"), "prefab", prefab_name);
    return base;
}

static bool fear_balls_invoke(struct skill* base) {
    const struct fear_balls* this = (struct fear_balls*)base;
    if (this->prefab == NULL) {
        logger_warn("Failed to invoke fear balls, prefab is null");
        return false;
    }
    return true;
}