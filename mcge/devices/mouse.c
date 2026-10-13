//
// Created by nikita on 13.10.2026.
//

#include "mouse.h"
#include "../msystems/msystem.h"


static struct {
    struct action* on_native_event;
    unsigned int on_native_event_token;
} mouse;



static void mouse_on_create(void) {

}
static void mouse_on_destroy(void) {

}


static void mouse_on_enable(struct engine_arguments args) {

}
static void mouse_on_disable(void) {

}


const struct msystem mouse_msystem = {
    .name = "mouse",
    .on_create = mouse_on_create,
    .on_destroy = mouse_on_destroy,
    .on_enable = mouse_on_enable,
    .on_disable = mouse_on_disable
};