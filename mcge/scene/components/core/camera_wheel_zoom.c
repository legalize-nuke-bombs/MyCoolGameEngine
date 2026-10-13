//
// Created by nikita on 13.10.2026.
//

#include "camera_wheel_zoom.h"
#include "../component_internal.h"


struct camera_wheel_zoom {
    struct component base;
    double min;
    double max;
    double speed;
};


const char* camera_wheel_zoom_component_key(void) {
    return "camera_wheel_zoom";
}


static void camera_wheel_zoom_on_create(struct component* base, struct fields *fields) {

}

static void camera_wheel_zoom_awake(struct component* base) {

}
static void camera_wheel_zoom_on_disable(struct component* base) {

}


const struct component_vtable camera_wheel_zoom_vtable = {
    .component_key = camera_wheel_zoom_component_key,
    .size = sizeof(struct camera_wheel_zoom),
    .on_create = camera_wheel_zoom_on_create,
    .on_awake = camera_wheel_zoom_awake,
    .on_disable = camera_wheel_zoom_on_disable
};