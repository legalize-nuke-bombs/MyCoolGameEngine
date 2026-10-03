//
// Created by nikita on 03.10.2026.
//

#include "mana.h"



#include <stdlib.h>
#include "../../scene/components/component_internal.h"
#include "../../utils/parser.h"

struct mana {
    struct component base;
    double amount;
    double max_amount;
};

static struct component* mana_clone(struct component base, const struct component *component);

static const struct component_vtable mana_vtable = {
    .component_key = mana_component_key,
    .on_clone = mana_clone
};

const char* mana_component_key(void) {
    return "mana";
}

struct component* mana_create(struct parser *parser, struct entity *parent) {
    struct mana *this = calloc(1, sizeof(struct mana));
    struct component *base = (struct component *) this;
    component_base_create(base, &mana_vtable, parent);
    parser_next_double(parser, &this->max_amount);
    this->amount = this->max_amount;
    return base;
}

static struct component* mana_clone(struct component base, const struct component *component) {
    const struct mana *mana = (const struct mana *) component;

    struct mana* this = calloc(1, sizeof(struct mana));
    this->base = base;
    return (struct component*)this;
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