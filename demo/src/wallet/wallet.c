//
// Created by nikita on 13.10.2026.
//

#include "wallet.h"
#include <mcge/mcge.h>


struct wallet {
    struct component base;

    int amount;

    struct action on_changed;
};


const char* wallet_component_key(void) {
    return "wallet";
}

static void wallet_on_create(struct component* base, struct fields *fields) {
    struct wallet *this = (struct wallet*)base;
    int start_min = fields_get_int(fields, "start_min", 10);
    int start_max = fields_get_int(fields, "start_max", 20);
    if (start_min < 0 || start_max < 0 || start_min > start_max) {
        logger_warn("Entity %s wallet bad start_min start_max (%d %d)", component_get_global_parent_name(base), start_min, start_max);
    }
    if (start_min < 0) {
        start_min = 0;
    }
    if (start_max < 0) {
        start_max = 0;
    }
    if (start_min > start_max) {
        start_max = start_min;
    }
    this->amount = random_next_int(start_min, start_max + 1);
    this->on_changed = action_create();
}
static void wallet_on_destroy(struct component* base) {
    struct wallet *this = (struct wallet*)base;
    action_destroy(&this->on_changed);
}

const struct component_vtable wallet_vtable = {
    .component_key = wallet_component_key,
    .size = sizeof(struct wallet),
    .on_create = wallet_on_create,
    .on_destroy = wallet_on_destroy
};

int wallet_amount(const struct wallet *this) {
    return this->amount;
}
bool wallet_try_take(struct wallet *this, const int amount) {
    if (amount <= 0) {
        return true;
    }
    if (amount > this->amount) {
        return false;
    }
    this->amount -= amount;
    action_invoke(&this->on_changed, NULL);
    return true;
}
void wallet_add(struct wallet *this, const int amount) {
    if (amount <= 0) {
        return;
    }
    this->amount += amount;
    action_invoke(&this->on_changed, NULL);
}

struct action* wallet_get_on_changed(struct wallet *this) {
    return &this->on_changed;
}