//
// Created by nikita on 29.09.2026.
//

#include "chunks.h"
#include "chunk.h"

#include <math.h>
#include <stdlib.h>

#include "../../logging/logger.h"
#include "../../utils/action.h"
#include "../../utils/pointer_dictionary.h"
#include "../components/component.h"

#define CHUNKS_SIZE 512
#define CHUNKS_START_CHUNK_SIZE 256.f


struct chunks {
    struct chunk chunks[CHUNKS_SIZE][CHUNKS_SIZE];
    float chunk_size;
};


struct chunks *chunks_create() {
    logger_info("Chunks (%d x %d, chunk size %f) are creating...", CHUNKS_SIZE, CHUNKS_SIZE, CHUNKS_START_CHUNK_SIZE);
    struct chunks *this = calloc(1, sizeof(struct chunks));
    this->chunk_size = CHUNKS_START_CHUNK_SIZE;
    return this;
}

static void chunks_destroy_chunks(struct chunks *this) {
    for (int i = 0; i < CHUNKS_SIZE; i++) {
        for (int j = 0; j < CHUNKS_SIZE; j++) {
            chunk_destroy(&this->chunks[i][j]);
        }
    }
}

void chunks_destroy(struct chunks *this) {
    logger_info("Chunks are destroying...");
    chunks_destroy_chunks(this);
    free(this);
}

void chunks_clear(struct chunks *this) {
    chunks_destroy_chunks(this);
}

static void chunks_resize(struct chunks *this);

static int chunks_get_position_index(const struct chunks* this, const double position) {
    const int offset = floor(position / this->chunk_size);
    return (CHUNKS_SIZE / 2) + offset;
}

void chunks_get_rect_indexes(const struct chunks *this, const struct rect rect, int *x_start, int *x_end, int *y_start, int *y_end) {
    const double min_x = rect.position.x - rect.size.x / 2;
    const double max_x = rect.position.x + rect.size.x / 2;
    const double min_y = rect.position.y - rect.size.y / 2;
    const double max_y = rect.position.y + rect.size.y / 2;

    *x_start = chunks_get_position_index(this, min_x);
    *x_end = chunks_get_position_index(this, max_x);
    *y_start = chunks_get_position_index(this, min_y);
    *y_end = chunks_get_position_index(this, max_y);
}

struct dictionary* chunks_chunk_get_types(const struct chunks *this, const int index_x, const int index_y) {
    if (index_x < 0 || index_y < 0 || index_x >= CHUNKS_SIZE || index_y >= CHUNKS_SIZE) {
        return NULL;
    }
    return chunk_get_types(&this->chunks[index_x][index_y]);
}

struct dictionary* chunks_chunk_get_components_by_type(const struct chunks *this, const int index_x, const int index_y, const char *component_type) {
    if (index_x < 0 || index_y < 0 || index_x >= CHUNKS_SIZE || index_y >= CHUNKS_SIZE) {
        return NULL;
    }
    return chunk_get_components_by_type(&this->chunks[index_x][index_y], component_type);
}

static void chunks_remove_component(const struct chunks *this, const struct rect rect, const struct component *component) {
    int start_x, end_x, start_y, end_y;
    chunks_get_rect_indexes(this, rect, &start_x, &end_x, &start_y, &end_y);
    for (int x = start_x; x <= end_x; x++) {
        for (int y = start_y; y <= end_y; y++) {
            chunk_try_remove_component(&this->chunks[x][y], component);
        }
    }
}

static void chunks_add_component_without_resize(struct chunks *this, struct component *component) {
    int start_x, end_x, start_y, end_y;
    chunks_get_rect_indexes(this, component_get_rect(component), &start_x, &end_x, &start_y, &end_y);
    for (int x = start_x; x <= end_x; x++) {
        for (int y = start_y; y <= end_y; y++) {
            chunk_try_add_component(&this->chunks[x][y], component);
        }
    }
}

static void chunks_add_component_with_resize(struct chunks* this, struct component* component) {
    int start_x, end_x, start_y, end_y;
    while (1) {
        chunks_get_rect_indexes(this, component_get_rect(component), &start_x, &end_x, &start_y, &end_y);
        if (start_x >= 0 && end_x < CHUNKS_SIZE && start_y >= 0 && end_y < CHUNKS_SIZE) {
            break;
        }
        chunks_resize(this);
    }
    chunks_add_component_without_resize(this, component);
}

static void chunks_resize(struct chunks *this) {
    const float target_chunk_size = this->chunk_size * 2;
    logger_info("Chunks are updating chunk size from %f to %f...", this->chunk_size, target_chunk_size);

    struct dictionary *all_components = pointer_dictionary_build(10);

    for (int i = 0; i < CHUNKS_SIZE; i++) {
        for (int j = 0; j < CHUNKS_SIZE; j++) {
            const struct dictionary *chunk_types = chunk_get_types(&this->chunks[i][j]);
            if (chunk_types == NULL) {
                continue;
            }
            struct dictionary_iterator chunk_types_iterator = dictionary_begin(chunk_types);
            struct dictionary_node node;
            while (dictionary_next(chunk_types, &chunk_types_iterator, &node)) {
                const struct dictionary* chunk_typed_components = node.value;

                struct dictionary_iterator chunk_typed_components_iterator = dictionary_begin(chunk_typed_components);
                while (dictionary_next(chunk_typed_components, &chunk_typed_components_iterator, &node)) {
                    dictionary_try_add(all_components, node.key, node.value);
                }
            }

            chunk_clear(&this->chunks[i][j]);
        }
    }

    this->chunk_size = target_chunk_size;

    struct dictionary_iterator iterator = dictionary_begin(all_components);
    struct dictionary_node node;
    while (dictionary_next(all_components, &iterator, &node)) {
        chunks_add_component_without_resize(this, node.value);
    }

    dictionary_destroy(all_components);
}

static void handle_component_rect_changed(void *listener, void *context);
static void handle_component_marked_destroyed(void *listener, void *context);

void chunks_register_component(struct chunks *this, struct component *component) {
    if (!component_is_chunkable(component) || !component_is_alive(component)) {
        return;
    }
    // We do not unsubscribe because scene infrastructure live longer than components
    unsigned int on_rect_changed_subscription_token;
    action_subscribe(component_get_on_rect_changed(component), this, handle_component_rect_changed, &on_rect_changed_subscription_token);
    unsigned int on_marked_destroyed_subscription_token;
    action_subscribe(component_get_on_marked_destroyed(component), this, handle_component_marked_destroyed, &on_marked_destroyed_subscription_token);
    chunks_add_component_with_resize(this, component);
}

static void handle_component_rect_changed(void *listener, void *context) {
    struct chunks *this = listener;
    const struct component_on_rect_changed_callback_data *data = context;
    struct component *component = data->component;
    if (!component_is_alive(component)) {
        return;
    }

    int old_x_start, old_x_end, old_y_start, old_y_end;
    chunks_get_rect_indexes(this, data->rect_pair.rect1, &old_x_start, &old_x_end, &old_y_start, &old_y_end);
    int new_x_start, new_x_end, new_y_start, new_y_end;
    chunks_get_rect_indexes(this, data->rect_pair.rect2, &new_x_start, &new_x_end, &new_y_start, &new_y_end);
    if (old_x_start == new_x_start && old_x_end == new_x_end && old_y_start == new_y_start && old_y_end == new_y_end) {
        return;
    }

    chunks_remove_component(this, data->rect_pair.rect1, component);
    chunks_add_component_with_resize(this, component);
}

static void handle_component_marked_destroyed(void *listener, void *context) {
    const struct chunks *this = listener;
    const struct component *component = context;
    chunks_remove_component(this, component_get_rect(component), component);
}