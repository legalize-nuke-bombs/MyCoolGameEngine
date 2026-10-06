//
// Created by nikita on 03.10.2026.
//

#include "mana.h"



#include <stdlib.h>
#include <mcge/scene/components/component_internal.h>
#include <mcge/utils/fields.h>

struct mana {
    struct component base;
    double amount;
    double max_amount;
};

static void mana_on_create(struct component *base, struct fields *fields);

const struct component_vtable mana_vtable = {
    .component_key = mana_component_key,
    .size = sizeof(struct mana),
    .on_create = mana_on_create
};

const char* mana_component_key(void) {
    return "mana";
}

static void mana_on_create(struct component *base, struct fields *fields) {
    struct mana *this = (struct mana *) base;
    this->max_amount = fields_get_double(fields, "max", 0);
    this->amount = this->max_amount;
}

bool mana_try_take(struct mana *this, double amount) {
    if (amount < 0) {
        amount = 0;
    }
    if (this->amount >= amount) {
        this->amount -= amount;
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