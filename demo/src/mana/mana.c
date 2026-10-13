//
// Created by nikita on 03.10.2026.
//

#include "mana.h"



#include <stdlib.h>
#include <mcge/mcge.h>

struct mana {
    struct component base;
    double amount;
    double max_amount;

    struct action on_changed;
};

static void mana_on_create(struct component *base, struct fields *fields);
static void mana_on_destroy(struct component *base);

const struct component_vtable mana_vtable = {
    .component_key = mana_component_key,
    .size = sizeof(struct mana),
    .on_create = mana_on_create,
    .on_destroy = mana_on_destroy
};

const char* mana_component_key(void) {
    return "mana";
}

static void mana_on_create(struct component *base, struct fields *fields) {
    struct mana *this = (struct mana *) base;
    this->max_amount = fields_get_double(fields, "max", 0);
    this->amount = this->max_amount;
    this->on_changed = action_create();
}
static void mana_on_destroy(struct component *base) {
    struct mana* this = (struct mana*)base;
    action_destroy(&this->on_changed);
}

bool mana_try_take(struct mana *this, double amount) {
    if (amount < 0) {
        amount = 0;
    }
    if (this->amount >= amount) {
        this->amount -= amount;
        action_invoke(&this->on_changed, NULL);
        return true;
    }
    return false;
}
double mana_amount(const struct mana *this) {
    return this->amount;
}
double mana_max_amount(const struct mana *this) {
    return this->max_amount;
}
double mana_percent(const struct mana *this) {
    return this->amount / this->max_amount;
}

struct action* mana_get_on_changed(struct mana *this) {
    return &this->on_changed;
}