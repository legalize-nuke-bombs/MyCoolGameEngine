//
// Created by Nikita on 03.10.2026.
//

#include "player.h"



#include <stdlib.h>
#include <mcge/mcge.h>

struct player {
    struct component base;
    double timer;
};


// TODO Remove this shit after the fix

static void player_update(struct component *base, const struct update_context *context) {
    struct player *this = (struct player*)base;
    this->timer += context->dt;
    if (this->timer >= 5) {
        logger_info("Marking player destroyed... If souls were launched the process will probably die");
        entity_mark_destroyed(component_get_parent(base));
    }
}

const struct component_vtable player_vtable = {
    .component_key = player_component_key,
    .size = sizeof(struct player),
    .on_update = player_update
};

const char* player_component_key(void) {
    return "player";
}


