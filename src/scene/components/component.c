#include "component_internal.h"

#include <stdlib.h>
#include <string.h>

#include "../entity.h"
#include "../../logging/logger.h"
#include "../../utils/action.h"
#include "../../utils/vector2_math.h"
#include "core/transform.h"
#include "../../utils/parser.h"
#include "../../utils/rect_pair.h"

void component_base_create(struct component *this, const struct component_vtable *vtable, struct parser *parser, struct entity *parent) {
    this->vtable = vtable;
    logger_debug("Component %s is creating...", component_get_key(this));
    this->awake = false;
    this->alive = true;

    const char* word = parser_next(parser);
    if (strcmp(word, "default") == 0) {
        this->local_rect.position = vector2_zero;
        this->local_rect.size = vector2_one;
    }
    else if (strcmp(word, "custom") == 0) {
        parser_next_double(parser, &this->local_rect.position.x);
        parser_next_double(parser, &this->local_rect.position.y);
        parser_next_double(parser, &this->local_rect.size.x);
        parser_next_double(parser, &this->local_rect.size.y);
    }
    else {
        logger_warn("Unexpected component local transform start token `%s`", word);
    }

    this->on_rect_changed = action_create();
    this->on_marked_destroyed = action_create();
    this->transform_on_rect_changed = NULL;
    this->transform_on_rect_changed_subscription_token = 0;

    this->last_chunked_update_frame_number = 0;

    this->parent = parent;
}
struct component* component_clone(const struct component *component) {
    struct component this = {0};
    this.vtable = component->vtable;
    logger_debug("Component %s is cloning...", component_get_key(component));
    this.awake = false;
    this.alive = true;
    this.local_rect = component->local_rect;
    this.on_rect_changed = action_create();
    this.on_marked_destroyed = action_create();
    return component->vtable->on_clone(this, component);
}

static void handle_transform_rect_changed(void *listener, void *context);

void component_awake(struct component *this) {
    logger_debug("Entity %s is awaking component %s...", component_get_parent_name(this), component_get_key(this));
    this->awake = true;

    struct transform* transform = entity_get_transform(component_get_parent(this));
    this->transform_on_rect_changed = transform_get_on_rect_changed(transform);
    action_subscribe(this->transform_on_rect_changed, this, handle_transform_rect_changed, &this->transform_on_rect_changed_subscription_token);

    if (this->vtable->on_awake) {
        this->vtable->on_awake(this);
    }
}

static void component_unsubscribe_from_transform(struct component *this) {
    if (this->transform_on_rect_changed == NULL) {
        return;
    }
    action_unsubscribe(this->transform_on_rect_changed, this->transform_on_rect_changed_subscription_token);
    this->transform_on_rect_changed = NULL;
}

void component_destroy(struct component *this) {
    component_mark_destroyed(this);
    logger_debug("Entity %s is destroying component %s...", component_get_parent_name(this), component_get_key(this));
    component_unsubscribe_from_transform(this);
    action_destroy(this->on_marked_destroyed);
    action_destroy(this->on_rect_changed);
    if (this->vtable->on_destroy) {
        this->vtable->on_destroy(this);
    }
    free(this);
}
void component_mark_destroyed(struct component *this) {
    if (!this->alive) {
        return;
    }
    logger_debug("Entity %s is marking destroyed component %s...", component_get_parent_name(this), component_get_key(this));
    this->alive = false;
    component_unsubscribe_from_transform(this);
    if (this->awake && this->vtable->on_disable) {
        this->vtable->on_disable(this);
    }
    action_invoke(this->on_marked_destroyed, this);
}

bool component_is_awake(const struct component *this) {
    return this->awake;
}
bool component_is_alive(const struct component *this) {
    return this->alive;
}

void component_set_parent(struct component *this, struct entity *parent) {
    this->parent = parent;
}
struct entity* component_get_parent(const struct component *this) {
    return this->parent;
}
const char* component_get_parent_name(const struct component *this) {
    return this->parent != NULL ? entity_get_name(this->parent) : "<none>";
}

struct rect component_get_local_rect(const struct component *this) {
    return this->local_rect;
}


void component_set_local_rect(struct component *this, const struct rect new_local_rect) {
    if (rects_equal(this->local_rect, new_local_rect)) {
        return;
    }
    struct component_on_rect_changed_callback_data data = {
        .component = this,
        .rect_pair.rect1 = component_get_rect(this)
    };
    this->local_rect = new_local_rect;
    data.rect_pair.rect2 = component_get_rect(this);
    action_invoke(this->on_rect_changed, &data);
}

static struct rect component_compute_rect(const struct rect parent_rect, const struct rect local_rect) {
    const struct rect result = {
        .position = vector_sum(parent_rect.position, vector_multiply_vector(local_rect.position, parent_rect.size)),
        .size = vector_multiply_vector(parent_rect.size, local_rect.size)
    };
    return result;
}

static void handle_transform_rect_changed(void *listener, void *context) {
    struct component *this = listener;
    const struct component_on_rect_changed_callback_data *transform_callback_data = context;
    struct component_on_rect_changed_callback_data data = {
        .component = this,
        .rect_pair.rect1 = component_compute_rect(transform_callback_data->rect_pair.rect1, this->local_rect),
        .rect_pair.rect2 = component_compute_rect(transform_callback_data->rect_pair.rect2, this->local_rect),
    };
    action_invoke(this->on_rect_changed, &data);
}

struct rect component_get_rect(const struct component *this) {
    if (this->parent == NULL) {
        return this->local_rect;
    }
    const struct rect parent_rect = transform_get_rect(entity_get_transform(this->parent));
    return component_compute_rect(parent_rect, this->local_rect);
}
struct action* component_get_on_rect_changed(const struct component *this) {
    return this->on_rect_changed;
}
struct action* component_get_on_marked_destroyed(const struct component *this) {
    return this->on_marked_destroyed;
}

const char* component_get_key(const struct component *this) {
    return this->vtable->component_key();
}

void component_update(struct component *this, const struct update_context *context) {
    if (!component_is_alive(this)) {
        return;
    }
    if (this->vtable->on_update != NULL) {
        this->vtable->on_update(this, context);
    }
}
bool component_is_updateable(const struct component *this) {
    return this->vtable->on_update;
}

void component_chunked_update(struct component *this, const struct update_context *context) {
    if (!component_is_alive(this)) {
        return;
    }
    if (this->vtable->on_chunked_update == NULL) {
        return;
    }
    if (this->last_chunked_update_frame_number == context->frame_number) {
        return;
    }
    this->last_chunked_update_frame_number++;
    this->vtable->on_chunked_update(this, context);
}
bool component_is_chunkable(const struct component *this) {
    return this->vtable->on_chunked_update || this->vtable->is_chunkable;
}