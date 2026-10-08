//
// Created by Nikita on 08.10.2026.
//

#include "bow.h"
#include <mcge/mcge.h>

#include "demo/src/characters/character.h"
#include "demo/src/characters/character_group.h"


struct bow {
    struct component base;

    double radius;

    double speed;
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
    this->speed = fields_get_double(fields, "speed", 1);
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
    this->hands_id = component_get_id(character);
}

static bool bow_try_attack(struct bow *this);

static void bow_simulation_chunk_update(struct component *base, const struct update_context *context) {
    struct bow *this = (struct bow*)base;

    this->attack_timer += context->dt;
    if (this->attack_timer >= this->speed){
        if (bow_try_attack(this)) {
            this->attack_timer -= this->speed;
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

static bool bow_try_attack(struct bow *this) {
    if (this->arrow == NULL) {
        return false;
    }

    const struct character *character = (struct character*)scene_try_get_component(this->character_id);
    const struct hands *hands = (struct hands*)scene_try_get_component(this->hands_id);
    if (character == NULL || hands == NULL) {
        return false;
    }

    struct character *target = bow_try_find_target(this);
    if (target == NULL) {
        return false;
    }

    logger_info("Attack");

    struct entity* arrow_entity = prefab_instantiate(this->arrow);

    struct rect arrow_rect = entity_get_local_rect(arrow_entity);
    arrow_rect.position = component_get_rect((struct component*)this).position;
    entity_set_local_rect(arrow_entity, arrow_rect);

    scene_capture_entity(arrow_entity);

    return true;
}

struct character* bow_try_find_target(struct bow* this) {
    const struct character *character = (struct character*)scene_try_get_component(this->character_id);
    if (character == NULL) {
        return NULL;
    }

    const struct vector2 position = component_get_rect((struct component*)character).position;
    const enum character_group group = character_get_group(character);

    const struct chunks *chunks = scene_get_chunks();
    const struct rect rect = component_get_rect((struct component*)this);
    int x_start, x_end, y_start, y_end;
    chunks_get_rect_indexes(chunks, rect, &x_start, &x_end, &y_start, &y_end);
    struct character* result_character = NULL;
    double min_sqr_distance = 1e+9;
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
                    if (min_sqr_distance > distance_sqr) {
                        min_sqr_distance = distance_sqr;
                        result_character = target_character;
                    }
                }
            }
        }
    }

    return result_character;
}