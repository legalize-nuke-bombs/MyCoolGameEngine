//
// Created by nikita on 13.10.2026.
//

#include "steal_wallet_on_kill.h"
#include <mcge/mcge.h>

#include "wallet.h"
#include "demo/src/health/damager.h"


struct steal_wallet_on_kill {
    struct component base;

    double efficiency;

    uint128_t wallet_id;
    uint128_t damager_id;
    unsigned int damager_on_kill_token;
};


const char* steal_wallet_on_kill_component_key(void) {
    return "steal_wallet_on_kill";
}


static void steal_wallet_on_kill_on_create(struct component* base, struct fields *fields) {
    struct steal_wallet_on_kill* this = (struct steal_wallet_on_kill*)base;
    this->efficiency = fields_get_double(fields, "efficiency", 1);
    if (this->efficiency < 0 || this->efficiency > 1) {
        logger_warn("Entity %s steal_wallet_on_kill bad efficiency (%f)", component_get_global_parent_name(base), this->efficiency);
        this->efficiency = 1;
    }
}


static void handle_kill(void *listener, void *context);

static void steal_wallet_on_kill_awake(struct component* base) {
    struct steal_wallet_on_kill* this = (struct steal_wallet_on_kill*)base;
    struct entity* entity = component_get_parent(base);

    struct wallet* wallet = (struct wallet*)entity_get_component(entity, "wallet", entity_query_local);
    struct damager* damager = (struct damager*)entity_get_component(entity, "damager", entity_query_local);
    if (wallet == NULL || damager == NULL) {
        entity_mark_destroyed(entity);
        return;
    }
    this->wallet_id = component_get_id((struct component*)wallet);
    this->damager_id = component_get_id((struct component*)damager);
    action_subscribe(damager_on_kill(damager), this, handle_kill, &this->damager_on_kill_token);
}
static void steal_wallet_on_kill_on_disable(struct component* base) {
    struct steal_wallet_on_kill* this = (struct steal_wallet_on_kill*)base;
    struct damager* damager = (struct damager*)scene_try_get_component(this->damager_id);
    if (damager) {
        action_unsubscribe(damager_on_kill(damager), this->damager_on_kill_token);
    }
}


const struct component_vtable steal_wallet_on_kill_vtable = {
    .component_key = steal_wallet_on_kill_component_key,
    .size = sizeof(struct steal_wallet_on_kill),
    .on_create = steal_wallet_on_kill_on_create,
    .on_awake = steal_wallet_on_kill_awake,
    .on_disable = steal_wallet_on_kill_on_disable
};



static void handle_kill(void *listener, void *context) {
    struct steal_wallet_on_kill* this = listener;
    struct entity* target = context;

    struct wallet* wallet = (struct wallet*)scene_try_get_component(this->wallet_id);
    if (wallet == NULL) {
        return;
    }

    struct wallet *target_wallet = (struct wallet*)entity_try_get_component(target, "wallet", entity_query_local);
    if (target_wallet == NULL) {
        return;
    }

    const int amount = wallet_amount(target_wallet);
    const int to_steal = (int)(this->efficiency * amount);
    wallet_try_take(target_wallet, to_steal);
    wallet_add(wallet, to_steal);
    logger_debug("Entity %s stealed %d / %d coins from entity %s. Now: %d", component_get_global_parent_name((struct component*)this), to_steal, amount, entity_get_name(target), wallet_amount(wallet));
}
