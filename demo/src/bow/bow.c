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

    struct bowman_stats stats;
    struct prefab* arrow;

    double attack_timer;

    struct dictionary* targets; // Targets dictionary is pre allocated to avoid allocations on hot path

    uint128_t character_id;
    uint128_t hands_id;
};

const char* bow_component_key(void) {
    return "bow";
}

static void bow_on_create(struct component *base, struct fields *fields) {
    struct bow *this = (struct bow*)base;
    this->arrow = prefabs_get(fields_get_string(fields, "arrow", "default"));
    this->targets = pointer_dictionary_build(8);
}

static void bow_on_destroy(struct component *base) {
    struct bow *this = (struct bow*)base;
    dictionary_destroy(this->targets);
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
    if (this->attack_timer >= this->stats.interval){
        if (bow_try_schedule_attack(this)) {
            this->attack_timer -= this->stats.interval;
        }
        else {
            this->attack_timer = this->stats.interval;
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
    struct dictionary* targets = context;

    if (this->arrow == NULL) {
        return;
    }

    if (!prefab_contains_component(this->arrow, "arrow", entity_query_local)) {
        return;
    }

    struct dictionary_node node;
    struct dictionary_iterator iterator = dictionary_begin(targets);
    while (dictionary_next(targets, &iterator, &node)) {
        struct entity* arrow_entity = prefab_instantiate(this->arrow);

        struct rect arrow_rect = entity_get_local_rect(arrow_entity);
        arrow_rect.position = component_get_rect((struct component*)this).position;
        entity_set_local_rect(arrow_entity, arrow_rect);

        struct arrow* arrow = (struct arrow*)entity_get_component(arrow_entity, "arrow", entity_query_local);
        arrow_launch(arrow, node.value, this->stats);

        scene_capture_entity(arrow_entity);
    }
}

static bool bow_try_schedule_attack(struct bow *this) {
    struct hands *hands = (struct hands*)scene_try_get_component(this->hands_id);
    if (hands == NULL) {
        return false;
    }

    struct dictionary *targets = bow_try_find_targets(this);
    if (targets == NULL) {
        return false;
    }
    if (dictionary_count(targets) == 0) {
        return false;
    }

    const struct hands_action hands_action = {
        .name = "bow_attack",
        .duration = 0,
        .priority = HANDS_ACTION_PRIORITY_PHYS_ATTACK,
        .method.executor = this,
        .method.context = targets,
        .method.func = bow_execute_attack,
    };
    const bool result = hands_try_put(hands, hands_action);
    return result;
}

void bow_set_stats(struct bow* this, struct bowman_stats stats) {
    this->stats = stats;
}

struct bow_find_most_injured_enemy_context {
    struct vector2 self_position;
    enum character_group self_group;
    double sqr_radius;
    struct character* result_character;
    double min_health;
};
static void bow_find_most_injured_enemy_step(struct component *component, void *context) {
    struct character *character = (struct character*)component;
    struct bow_find_most_injured_enemy_context *cnt = context;

    const enum character_group group = character_get_group(character);
    const struct vector2 position = component_get_rect(component).position;
    const double distance_sqr = vector_sqr_distance(&cnt->self_position, &position);

    if (cnt->self_group == group || distance_sqr > cnt->sqr_radius) {
        return;
    }

    const struct health *health = (struct health*)entity_try_get_component(component_get_parent(component), "health", entity_query_local);
    if (health == NULL) {
        return;
    }
    const double h_amount = health_amount(health);

    if (cnt->min_health > h_amount) {
        cnt->min_health = h_amount;
        cnt->result_character = character;
    }
}
struct bow_find_closest_enemy_context {
    struct vector2 self_position;
    enum character_group self_group;
    double sqr_radius;
    struct vector2 most_injured_position;
    struct dictionary* exclude_dict;
    struct character* result_character;
    double min_distance_sqr;
};
static void bow_find_closest_enemy_step(struct component *component, void *context) {
    struct character *character = (struct character*)component;
    struct bow_find_closest_enemy_context *cnt = context;

    const enum character_group group = character_get_group(character);
    const struct vector2 position = component_get_rect(component).position;
    const double self_distance_sqr = vector_sqr_distance(&cnt->self_position, &position);

    if (cnt->self_group == group || self_distance_sqr > cnt->sqr_radius) {
        return;
    }

    const double most_injured_distance_sqr = vector_sqr_distance(&cnt->most_injured_position, &position);
    if (most_injured_distance_sqr > cnt->min_distance_sqr) {
        return;
    }

    if (dictionary_present(cnt->exclude_dict, character)) {
        return;
    }

    cnt->min_distance_sqr = most_injured_distance_sqr;
    cnt->result_character = character;
}
struct dictionary* bow_try_find_targets(struct bow* this) {
    const struct character *character = (struct character*)scene_try_get_component(this->character_id);
    if (character == NULL) {
        return NULL;
    }

    struct rect rect = component_get_rect((struct component*)this);
    rect.size.x = 2 * this->stats.range;
    rect.size.y = 2 * this->stats.range;

    struct bow_find_most_injured_enemy_context find_most_injured_enemy_context = {
        .self_position = component_get_rect((struct component*)character).position,
        .self_group = character_get_group(character),
        .sqr_radius = this->stats.range * this->stats.range,
        .result_character = NULL,
        .min_health = 1e+9
    };
    chunks_algorithms_for_each_typed(rect, "character", bow_find_most_injured_enemy_step, &find_most_injured_enemy_context);
    struct character* most_injured_enemy = find_most_injured_enemy_context.result_character;
    if (most_injured_enemy == NULL) {
        return NULL;
    }

    dictionary_clear(this->targets);
    dictionary_try_add(this->targets, most_injured_enemy, most_injured_enemy);
    // Number of arrows is usually very small (1 - 3) so probably it's the fastest approach
    for (int i = 0; i < this->stats.arrows - 1; i++) {
        struct bow_find_closest_enemy_context find_closest_enemy_context = {
            .self_position = find_most_injured_enemy_context.self_position,
            .self_group = find_most_injured_enemy_context.self_group,
            .sqr_radius = find_most_injured_enemy_context.sqr_radius,
            .most_injured_position = component_get_rect((struct component*)most_injured_enemy).position,
            .exclude_dict = this->targets,
            .result_character = NULL,
            .min_distance_sqr = 1e+9
        };

        chunks_algorithms_for_each_typed(rect, "character", bow_find_closest_enemy_step, &find_closest_enemy_context);
        if (find_closest_enemy_context.result_character == NULL) {
            break;
        }
        dictionary_try_add(this->targets, this->targets, find_closest_enemy_context.result_character);
    }
    return this->targets;
}