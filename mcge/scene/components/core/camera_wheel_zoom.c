//
// Created by nikita on 13.10.2026.
//

#include "camera_wheel_zoom.h"

#include "camera.h"
#include "../component_internal.h"
#include "mcge/devices/mouse.h"
#include "mcge/logging/logger.h"
#include "mcge/scene/entity.h"
#include "mcge/scene/scene.h"
#include "mcge/utils/action.h"
#include "mcge/utils/fields.h"


struct camera_wheel_zoom {
    struct component base;

    double min;
    double max;
    double speed;

    uint128_t camera_id;

    struct action* on_mouse_wheel_scrolled;
    unsigned int on_mouse_wheel_scrolled_token;
};


const char* camera_wheel_zoom_component_key(void) {
    return "camera_wheel_zoom";
}


static void camera_wheel_zoom_on_create(struct component* base, struct fields *fields) {
    struct camera_wheel_zoom* this = (struct camera_wheel_zoom*)base;
    this->min = fields_get_double(fields, "min", 0.75);
    this->max = fields_get_double(fields, "max", 1.25);
    this->speed = fields_get_double(fields, "speed", 0.025);
}


static void handle_mouse_wheel_scrolled(void *listener, void *context);

static void camera_wheel_zoom_awake(struct component* base) {
    struct camera_wheel_zoom* this = (struct camera_wheel_zoom*)base;
    struct entity* entity = component_get_parent(base);

    const struct component* camera = entity_get_component(entity, "camera", entity_query_local);
    if (camera == NULL) {
        entity_mark_destroyed(entity);
        return;
    }
    this->camera_id = component_get_id(camera);

    this->on_mouse_wheel_scrolled = mouse_get_on_wheel_scrolled();
    action_subscribe(this->on_mouse_wheel_scrolled, this, handle_mouse_wheel_scrolled, &this->on_mouse_wheel_scrolled_token);
}
static void camera_wheel_zoom_on_disable(struct component* base) {
    const struct camera_wheel_zoom* this = (struct camera_wheel_zoom*)base;
    if (this->on_mouse_wheel_scrolled == NULL) {
        return;
    }
    action_unsubscribe(this->on_mouse_wheel_scrolled, this->on_mouse_wheel_scrolled_token);
}


const struct component_vtable camera_wheel_zoom_vtable = {
    .component_key = camera_wheel_zoom_component_key,
    .size = sizeof(struct camera_wheel_zoom),
    .on_create = camera_wheel_zoom_on_create,
    .on_awake = camera_wheel_zoom_awake,
    .on_disable = camera_wheel_zoom_on_disable
};



static void handle_mouse_wheel_scrolled(void *listener, void *context) {
    struct camera_wheel_zoom* this = listener;
    struct mouse_on_wheel_scrolled_callback_context* cnt = context;

    struct camera* camera = (struct camera*)scene_try_get_component(this->camera_id);
    if (camera == NULL) {
        return;
    }

    const double offset = this->speed * cnt->dy;

    double current = camera_get_visible_height_multiplier(camera);
    current += offset;
    if (current < this->min) {
        current = this->min;
    }
    if (current > this->max) {
        current = this->max;
    }
    camera_set_visible_height_multiplier(camera, current);
}