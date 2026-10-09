//
// Created by Nikita on 08.10.2026.
//

#include "bow.h"
#include <mcge/mcge.h>

#include "arrow.h"
#include "demo/src/character/character.h"
#include "demo/src/character/character_group.h"
#include "demo/src/hands/hands.h"
#include "demo/src/health/health.h"


struct bow {
    struct component base;

    double radius;

    double attack_interval;
    double attack_timer;

    struct prefab* arrow;

    uint128_t character_id;
    uint128_t hands_id;
};

const char* bow_component_key(void) {
    return "bow";
}

static void bow_on_create(struct component *base, struct fields *fields) {
    struct bow *this = (struct bow*)base;
    this->radius = fields_get_double(fields, "radius", 10);
    this->attack_interval = fields_get_double(fields, "interval", 1);
    this->arrow = prefabs_get(fields_get_string(fields, "arrow", "default"));
}

static void bow_on_destroy(struct component *base) {
    struct bow *this = (struct bow*)base;
}

static void bow_awake(struct component *base) {
    struct bow* this = (struct bow*)base;
    struct entity* entity = component_get_parent(base);

    const struct component* character = entity_get_component(entity, "character", entity_query_local);
    const struct component* hands = entity_get_component(entity, "hands", entity_query_local);
    if (character == NULL || hands == NULL) {
        entity_mark_destroyed(entity);
        return;
    }

    this->character_id = component_get_id(character);
    this->hands_id = component_get_id(hands);
}

static bool bow_try_schedule_attack(struct bow *this);

static void bow_simulation_chunk_update(struct component *base, const struct update_context *context) {
    struct bow *this = (struct bow*)base;

    this->attack_timer += context->dt;
    if (this->attack_timer >= this->attack_interval){
        if (bow_try_schedule_attack(this)) {
            this->attack_timer -= this->attack_interval;
        }
        else {
            this->attack_timer = this->attack_interval;
        }
    }
}

const struct component_vtable bow_vtable = {
    .component_key = bow_component_key,
    .size = sizeof(struct bow),
    .on_create = bow_on_create,
    .on_destroy = bow_on_destroy,
    .on_awake = bow_awake,
    .on_simulation_chunk_update = bow_simulation_chunk_update
};

static void bow_execute_attack(void *executor, void *context) {
    struct bow* this = executor;
    struct character* target = context;

    if (this->arrow == NULL) {
        return;
    }

    if (!prefab_contains_component(this->arrow, "arrow", entity_query_local)) {
        return;
    }

    struct entity* arrow_entity = prefab_instantiate(this->arrow);

    struct rect arrow_rect = entity_get_local_rect(arrow_entity);
    arrow_rect.position = component_get_rect((struct component*)this).position;
    entity_set_local_rect(arrow_entity, arrow_rect);

    struct arrow* arrow = (struct arrow*)entity_get_component(arrow_entity, "arrow", entity_query_local);
    arrow_launch(arrow, target);

    scene_capture_entity(arrow_entity);
}

static bool bow_try_schedule_attack(struct bow *this) {
    struct character *target = bow_try_find_target(this);
    if (target == NULL) {
        return false;
    }

    struct hands *hands = (struct hands*)scene_try_get_component(this->hands_id);
    if (hands == NULL) {
        return false;
    }

    const struct hands_action hands_action = {
        .name = "bow_attack",
        .duration = 0,
        .priority = HANDS_ACTION_PRIORITY_PHYS_ATTACK,
        .method.executor = this,
        .method.context = target,
        .method.func = bow_execute_attack,
    };
    return hands_try_put(hands, hands_action);
}

struct character* bow_try_find_target(struct bow* this) {
    const struct character *character = (struct character*)scene_try_get_component(this->character_id);
    if (character == NULL) {
        return NULL;
    }

    const struct vector2 position = component_get_rect((struct component*)character).position;
    const enum character_group group = character_get_group(character);

    const struct chunks *chunks = scene_get_chunks();
    struct rect rect = component_get_rect((struct component*)this);
    rect.size.x = 2 * this->radius;
    rect.size.y = 2 * this->radius;
    int x_start, x_end, y_start, y_end;
    chunks_get_rect_indexes(chunks, rect, &x_start, &x_end, &y_start, &y_end);
    struct character* result_character = NULL;
    const double sqr_radius = this->radius * this->radius;
    double min_health = 1e+9;
    for (int x = x_start; x <= x_end; x++) {
        for (int y = y_start; y <= y_end; y++) {
            const struct dictionary* characters = chunks_chunk_get_components_by_type(chunks, x, y, "character");
            if (characters == NULL) {
                continue;
            }
            struct dictionary_iterator iterator = dictionary_begin(characters);
            struct dictionary_node node;
            while (dictionary_next(characters, &iterator, &node)) {
                struct character *target_character = node.value;
                const enum character_group target_character_group = character_get_group(target_character);
                struct vector2 target_character_position = component_get_rect((struct component*)target_character).position;
                if (group != target_character_group) {
                    const double distance_sqr = vector_sqr_distance(&position, &target_character_position);
                    if (sqr_radius > distance_sqr) {
                        const struct health* target_health = (struct health*)entity_try_get_component(component_get_parent((struct component*)target_character), "health", entity_query_local);
                        const double target_health_amount = target_health ? health_amount(target_health) : min_health - 1;
                        if (min_health > target_health_amount) {
                            result_character = target_character;
                            min_health = target_health_amount;
                        }
                    }
                }
            }
        }
    }

    return result_character;
}