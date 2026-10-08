//
// Created by Nikita on 08.10.2026.
//

#include "bow.h"
#include <mcge/mcge.h>


struct bow {
    struct component base;

    double radius;
    double speed;
    double attack_timer;
};

const char* bow_component_key(void) {
    return "bow";
}

static void bow_on_create(struct component *base, struct fields *fields) {
    struct bow *this = (struct bow*)base;
    this->radius = fields_get_double(fields, "radius", 10);
    this->speed = fields_get_double(fields, "speed", 1);
}

static void bow_on_destroy(struct component *base) {
    struct bow *this = (struct bow*)base;
}

static void bow_attack(struct bow *this);

static void bow_simulation_chunk_update(struct component *base, const struct update_context *context) {
    struct bow *this = (struct bow*)base;

    this->attack_timer += context->dt;
    if (this->attack_timer >= this->speed){
        this->attack_timer -= this->speed;
        bow_attack(this);
    }
}

const struct component_vtable bow_vtable = {
    .component_key = bow_component_key,
    .size = sizeof(struct bow),
    .on_create = bow_on_create,
    .on_destroy = bow_on_destroy,
    .on_simulation_chunk_update = bow_simulation_chunk_update
};

static void bow_attack(struct bow *this) {
    logger_info("Bow attack"); // TODO
}