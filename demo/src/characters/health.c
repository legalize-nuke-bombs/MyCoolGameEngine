//
// Created by Nikita on 07.10.2026.
//

#include "health.h"

#include <mcge/mcge.h>


struct health {
    struct component base;
    double amount;
    double max;
    struct action on_changed;
    struct action on_take_damage;
    struct action on_restore;
    struct action on_death;
    struct action on_resurrect;
};


#define DEATH_THRESHOLD 1e-3



const char* health_component_key(void) {
    return "health";
}

static void health_on_create(struct component *base, struct fields *fields) {
    struct health* this = (struct health*)base;
    this->max = fields_get_double(fields, "max", 100);
    this->amount = fields_get_double(fields, "amount", this->max);
    this->on_changed = action_create();
    this->on_take_damage = action_create();
    this->on_restore = action_create();
    this->on_death = action_create();
    this->on_resurrect = action_create();
}
static void health_on_destroy(struct component* base) {
    struct health* this = (struct health*)base;
    action_destroy(&this->on_resurrect);
    action_destroy(&this->on_death);
    action_destroy(&this->on_restore);
    action_destroy(&this->on_take_damage);
    action_destroy(&this->on_changed);
}

const struct component_vtable health_vtable = {
    .component_key = health_component_key,
    .size = sizeof(struct health),
    .on_create = health_on_create,
    .on_destroy = health_on_destroy
};

double health_amount(const struct health *this) {
    return this->amount;
}
double health_max_amount(const struct health *this) {
    return this->max;
}
double health_scale(const struct health *this) {
    return this->amount / this->max;
}
bool health_is_alive(const struct health* this) {
    return this->amount > DEATH_THRESHOLD;
}

void health_take_damage(struct health* this, struct damage damage) {
    if (!health_is_alive(this) || damage.amount <= 0) {
        return;
    }
    if (damage.amount > this->amount) {
        damage.amount = this->amount;
    }
    logger_debug("Entity %s hp %f got %f damage", component_get_global_parent_name((struct component*)this), this->amount, damage.amount);
    this->amount -= damage.amount;
    action_invoke(&this->on_changed, NULL);
    action_invoke(&this->on_take_damage, &damage);
    if (!health_is_alive(this)) {
        action_invoke(&this->on_death, &damage);
    }
}
void health_restore(struct health* this, double amount) {
    if (!health_is_alive(this) || amount <= 0) {
        return;
    }
    if (amount > this->max - this->amount) {
        amount = this->max - this->amount;
    }
    this->amount += amount;
    action_invoke(&this->on_changed, NULL);
    action_invoke(&this->on_restore, &amount);
}

void health_resurrect(struct health* this, double amount) {
    if (health_is_alive(this) || amount <= DEATH_THRESHOLD) {
        return;
    }
    if (amount > this->max) {
        amount = this->max;
    }
    this->amount = amount;
    action_invoke(&this->on_changed, NULL);
    action_invoke(&this->on_resurrect, NULL);
}

void health_set_max_amount(struct health* this, const double max_amount) {
    if (max_amount < 0) {
        return;
    }
    if (this->amount > max_amount) {
        this->amount = max_amount;
    }
    this->max = max_amount;
    action_invoke(&this->on_changed, NULL);
}

struct action* health_get_on_changed(struct health *this) {
    return &this->on_changed;
}
struct action* health_get_on_take_damage(struct health *this) {
    return &this->on_take_damage;
}
struct action* health_get_on_restore(struct health *this) {
    return &this->on_restore;
}
struct action* health_get_on_death(struct health *this) {
    return &this->on_death;
}
struct action* health_get_on_resurrect(struct health *this) {
    return &this->on_resurrect;
}