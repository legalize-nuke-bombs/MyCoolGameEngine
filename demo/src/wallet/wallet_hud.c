//
// Created by nikita on 14.10.2026.
//

#include "wallet_hud.h"
#include <mcge/mcge.h>


struct wallet_hud {
    struct component base;

    uint128_t wallet_id;
    unsigned int wallet_on_changed_token;
};


const char* wallet_hud_component_key(void) {
    return "wallet_hud";
}


const struct component_vtable wallet_hud_vtable = {
    .component_key = wallet_hud_component_key,
    .size = sizeof(struct wallet_hud)
};