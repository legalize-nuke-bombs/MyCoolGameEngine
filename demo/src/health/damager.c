//
// Created by nikita on 12.10.2026.
//

#include "damager.h"
#include <mcge/mcge.h>

#include "health.h"


struct damager {
    struct component base;
    struct action on_damage_dealt;
    struct action on_kill;
};

const char* damager_component_key(void) {
    return "damager";
}

static void damager_on_create(struct component *base, struct fields *fields) {
    struct damager *this = (struct damager *) base;
    this->on_damage_dealt = action_create();
    this->on_kill = action_create();
}
static void damager_on_destroy(struct component *base) {
    struct damager *this = (struct damager *) base;
    action_destroy(&this->on_damage_dealt);
    action_destroy(&this->on_kill);
}

const struct component_vtable damager_vtable = {
    .component_key = damager_component_key,
    .size = sizeof(struct damager),
    .on_create = damager_on_create,
    .on_destroy = damager_on_destroy
};

void damager_deal_damage(const struct damager *this, struct health *target_health, const struct damage damage) {
    if (!health_is_alive(target_health)) {
        return;
    }
    struct entity* target_entity = component_get_parent((struct component*)target_health);

    double delta_hp;
    bool kill;
    health_check_damage(target_health, damage, &delta_hp, &kill);

    // Callbacks must be called before entity actually take damage because entity can be marked destroyed after taking damage
    if (delta_hp > 0) {
        action_invoke(&this->on_damage_dealt, target_entity);
    }
    if (kill) {
        action_invoke(&this->on_kill, target_entity);
    }

    health_take_damage(target_health, damage);
}

struct action* damager_on_damage_dealt(struct damager *this) {
    return &this->on_damage_dealt;
}
struct action* damager_on_kill(struct damager *this) {
    return &this->on_kill;
}