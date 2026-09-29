//
// Created by nikita on 29.09.2026.
//

#include "chunks.h"

#include <math.h>
#include <stdlib.h>

#include "../../logging/logger.h"
#include "../../utils/action.h"
#include "../../utils/list.h"
#include "../components/component.h"

#define CHUNKS_SIZE 256
#define CHUNKS_START_CHUNK_SIZE 1.f

struct chunks {
    struct list* components[CHUNKS_SIZE][CHUNKS_SIZE];
    float chunk_size;
};


struct chunks *chunks_create() {
    logger_info("Chunks (%d x %d, chunk size %f) are creating...", CHUNKS_SIZE, CHUNKS_SIZE, CHUNKS_START_CHUNK_SIZE);
    struct chunks *this = calloc(1, sizeof(struct chunks));
    for (int i = 0; i < CHUNKS_SIZE; i++) {
        for (int j = 0; j < CHUNKS_SIZE; j++) {
            this->components[i][j] = list_create(1);
        }
    }
    this->chunk_size = CHUNKS_START_CHUNK_SIZE;
    return this;
}
void chunks_destroy(struct chunks *this) {
    logger_info("Chunks are destroying...");
    for (int i = 0; i < CHUNKS_SIZE; i++) {
        for (int j = 0; j < CHUNKS_SIZE; j++) {
            list_destroy(this->components[i][j]);
        }
    }
    free(this);
}

void chunks_clear(const struct chunks *this) {
    logger_info("Chunks are clearing...");
    for (int i = 0; i < CHUNKS_SIZE; i++) {
        for (int j = 0; j < CHUNKS_SIZE; j++) {
            list_clear(this->components[i][j]);
        }
    }
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

static void chunks_chunk_add_component_if_absent(const struct chunks *this, struct component* component, const int index_x, const int index_y) {
    struct list* list = this->components[index_x][index_y];
    for (int i = 0; i < list_count(list); i++) {
        const struct component* c = list_get(list, i);
        if (c == component) {
            return;
        }
    }
    list_add(list, component);
}

static void chunks_chunk_remove_component(const struct chunks *this, const struct component* component, const int index_x, const int index_y) {
    struct list* list = this->components[index_x][index_y];
    for (int i = 0; i < list_count(list); i++) {
        const struct component* c = list_get(list, i);
        if (c == component) {
            if (i != list_count(list) - 1) {
                list_swap(list, i, list_count(list) - 1);
            }
            list_pop_back(list);
            break;
        }
    }
}

struct list* chunks_chunk_get_components(const struct chunks *this, const int index_x, const int index_y) {
    return this->components[index_x][index_y];
}

static void chunks_remove_component(const struct chunks *this, const struct rect rect, const struct component *component) {
    int start_x, end_x, start_y, end_y;
    chunks_get_rect_indexes(this, rect, &start_x, &end_x, &start_y, &end_y);
    for (int x = start_x; x <= end_x; x++) {
        for (int y = start_y; y <= end_y; y++) {
            chunks_chunk_remove_component(this, component, x, y);
        }
    }
}

static void chunks_add_component_without_resize(struct chunks *this, struct component *component) {
    int start_x, end_x, start_y, end_y;
    chunks_get_rect_indexes(this, component_get_rect(component), &start_x, &end_x, &start_y, &end_y);
    for (int x = start_x; x <= end_x; x++) {
        for (int y = start_y; y <= end_y; y++) {
            chunks_chunk_add_component_if_absent(this, component, x, y);
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

    struct list *all_components = list_create(10240);

    for (int i = 0; i < CHUNKS_SIZE; i++) {
        for (int j = 0; j < CHUNKS_SIZE; j++) {
            struct list *list = this->components[i][j];
            for (int k = 0; k < list_count(list); k++) {
                struct component *component = list_get(list, k);
                list_add(all_components, component);
            }
            list_clear(list);
        }
    }

    this->chunk_size = target_chunk_size;

    for (int i = 0; i < list_count(all_components); i++) {
        struct component *comp = list_get(all_components, i);
        chunks_add_component_without_resize(this, comp);
    }

    list_destroy(all_components);
}

static void handle_component_rect_changed(void *listener, void *context);

void chunks_register_component(struct chunks *this, struct component *component) {
    if (!component_is_chunkable(component)) {
        return;
    }
    const struct action* on_rect_changed = component_get_on_rect_changed(component);
    unsigned int on_rect_changed_subscription_token; // We do not unsubscribe because scene infrastructure live longer than components
    action_subscribe(on_rect_changed, this, handle_component_rect_changed, &on_rect_changed_subscription_token);
    chunks_add_component_with_resize(this, component);
}

static void handle_component_rect_changed(void *listener, void *context) {
    struct chunks *this = listener;
    const struct component_on_rect_changed_callback_data *data = context;
    struct component *component = data->component;
    chunks_remove_component(this, data->rect_pair.rect1, component);
    chunks_add_component_with_resize(this, component);
}