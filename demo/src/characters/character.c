//
// Created by Nikita on 08.10.2026.
//

#include "character.h"

#include <string.h>
#include <mcge/mcge.h>

struct character {
    struct component base;
    enum character_group group;
};

const char* character_component_key(void) {
    return "character";
}

static void character_on_create(struct component* base, struct fields *fields) {
    struct character* this = (struct character*)base;
    const char* group_name = fields_get_string(fields, "group", "creep");
    if (strcmp(group_name, "player") == 0) {
        this->group = character_group_player;
    }
    else if (strcmp(group_name, "creep") == 0) {
        this->group = character_group_creep;
    }
    else {
        logger_warn("Entity %s has unexpected character group %s", component_get_global_parent_name(base), group_name);
        this->group = character_group_creep;
    }
}

const struct component_vtable character_vtable = {
    .component_key = character_component_key,
    .size = sizeof(struct character),
    .on_create = character_on_create
};

enum character_group character_get_group(const struct character *this) {
    return this->group;
}