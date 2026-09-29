//
// Created by nikita on 29.09.2026.
//

#include "chunks.h"

#include <stdlib.h>

#include "../../logging/logger.h"
#include "../../utils/action.h"
#include "../components/component.h"

struct chunks {

};

struct chunks *chunks_create() {
    logger_info("Chunks are creating....");
    struct chunks *this = calloc(1, sizeof(struct chunks));
    return this;
}
void chunks_destroy(struct chunks *this) {
    logger_info("Chunks are destroying...");
    free(this);
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