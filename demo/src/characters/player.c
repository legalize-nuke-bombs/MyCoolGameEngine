//
// Created by Nikita on 03.10.2026.
//

#include "player.h"



#include <stdlib.h>
#include "scene/components/component_internal.h"

struct player {
    struct component base;
};


const struct component_vtable player_vtable = {
    .component_key = player_component_key,
    .size = sizeof(struct player)
};

const char* player_component_key(void) {
    return "player";
}


