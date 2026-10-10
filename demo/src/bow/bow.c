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
        dictionary_destroy(targets);
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

struct dictionary* bow_try_find_targets(struct bow* this) {
    const struct character *character = (struct character*)scene_try_get_component(this->character_id);
    if (character == NULL) {
        return NULL;
    }

    const struct vector2 position = component_get_rect((struct component*)character).position;
    const enum character_group group = character_get_group(character);

    const struct chunks *chunks = scene_get_chunks();
    struct rect rect = component_get_rect((struct component*)this);
    rect.size.x = 2 * this->stats.range;
    rect.size.y = 2 * this->stats.range;
    int x_start, x_end, y_start, y_end;
    chunks_get_rect_indexes(chunks, rect, &x_start, &x_end, &y_start, &y_end);
    struct character* result_character = NULL;
    const double sqr_radius = this->stats.range * this->stats.range;
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

    if (result_character == NULL) {
        return NULL;
    }

    dictionary_clear(this->targets);

    dictionary_try_add(this->targets, result_character, result_character);
    const struct vector2 result_character_position = component_get_rect((struct component*)result_character).position;

    for (int i = 0; i < this->stats.arrows - 1; i++) { // Number of arrows is usually small so probably it's the most efficient approach

        double distance_to_main_sqr_min = 1e+9;
        struct character* secondary_target = NULL;

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
                    if (group != target_character_group && dictionary_absent(this->targets, target_character)) {
                        const double distance_sqr = vector_sqr_distance(&position, &target_character_position);
                        if (sqr_radius > distance_sqr) {
                            const double distance_to_main_sqr = vector_sqr_distance(&position, &result_character_position);
                            if (distance_to_main_sqr_min > distance_to_main_sqr) {
                                distance_to_main_sqr_min = distance_to_main_sqr;
                                secondary_target = target_character;
                            }
                        }
                    }
                }
            }
        }

        if (secondary_target == NULL) {
            break;
        }
        dictionary_try_add(this->targets, secondary_target, secondary_target);
    }

    return this->targets;
}