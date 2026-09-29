//
// Created by nikita on 29.09.2026.
//

#include "chunks.h"

#include <stdlib.h>

#include "../../logging/logger.h"
#include "../../utils/action.h"
#include "../../utils/list.h"
#include "../components/component.h"

#define CHUNKS_WIDTH 1024
#define CHUNKS_HEIGHT 1024

struct chunks {
    struct list* components[CHUNKS_WIDTH][CHUNKS_WIDTH];
};

struct chunks *chunks_create() {
    logger_info("Chunks (%d x %d) are creating...", CHUNKS_WIDTH, CHUNKS_HEIGHT);
    struct chunks *this = calloc(1, sizeof(struct chunks));
    for (int i = 0; i < CHUNKS_WIDTH; i++) {
        for (int j = 0; j < CHUNKS_HEIGHT; j++) {
            this->components[i][j] = list_create(1);
        }
    }
    return this;
}
void chunks_destroy(struct chunks *this) {
    logger_info("Chunks are destroying...");
    for (int i = 0; i < CHUNKS_WIDTH; i++) {
        for (int j = 0; j < CHUNKS_HEIGHT; j++) {
            list_destroy(this->components[i][j]);
        }
    }
    free(this);
}

void chunks_clear(const struct chunks *this) {
    logger_info("Chunks are clearing...");
    for (int i = 0; i < CHUNKS_WIDTH; i++) {
        for (int j = 0; j < CHUNKS_HEIGHT; j++) {
            list_clear(this->components[i][j]);
        }
    }
}

static void handle_component_rect_changed(void *listener, void *context);

void chunks_register_component(struct chunks *this, const struct component *component) {
    const struct action* on_rect_changed = component_get_on_rect_changed(component);
    unsigned int on_rect_changed_subscription_token; // We do not unsubscribe because scene infrastructure live longer than components
    action_subscribe(on_rect_changed, this, handle_component_rect_changed, &on_rect_changed_subscription_token);
}

static void handle_component_rect_changed(void *listener, void *context) {
    struct chunks *this = listener;
    const struct component_on_rect_changed_callback_data *data = context;
    const struct component *component = data->component;
    logger_debug("Entity %s component %s rect update", component_get_parent_name(component), component_get_key(component));
}