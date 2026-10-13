//
// Created by nikita on 13.10.2026.
//

#include "camera_wheel_zoom.h"
#include "../component_internal.h"
#include "mcge/devices/mouse.h"
#include "mcge/logging/logger.h"
#include "mcge/utils/action.h"
#include "mcge/utils/fields.h"


struct camera_wheel_zoom {
    struct component base;

    double min;
    double max;
    double speed;

    unsigned int on_mouse_wheel_scrolled_token;
};


const char* camera_wheel_zoom_component_key(void) {
    return "camera_wheel_zoom";
}


static void camera_wheel_zoom_on_create(struct component* base, struct fields *fields) {
    struct camera_wheel_zoom* this = (struct camera_wheel_zoom*)base;
    this->min = fields_get_double(fields, "min", 0.5);
    this->max = fields_get_double(fields, "max", 2.0);
    this->speed = fields_get_double(fields, "speed", 1.0);
}


static void handle_mouse_wheel_scrolled(void *listener, void *context);

static void camera_wheel_zoom_awake(struct component* base) {
    struct camera_wheel_zoom* this = (struct camera_wheel_zoom*)base;
    action_subscribe(mouse_get_on_wheel_scrolled(), this, handle_mouse_wheel_scrolled, &this->on_mouse_wheel_scrolled_token);
}
static void camera_wheel_zoom_on_disable(struct component* base) {
    const struct camera_wheel_zoom* this = (struct camera_wheel_zoom*)base;
    action_unsubscribe(mouse_get_on_wheel_scrolled(), this->on_mouse_wheel_scrolled_token);
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
    logger_info("Invoke");
}