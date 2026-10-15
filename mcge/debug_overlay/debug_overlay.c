//
// Created by nikita on 15.10.2026.
//

#include "debug_overlay.h"
#include "mcge/engine/msystem.h"


static struct {

} debug_overlay;


static void debug_overlay_on_enable(struct engine_arguments args) {

}
static void debug_overlay_on_disable(void) {

}


const struct msystem debug_overlay_msystem = {
    .name = "debug_overlay",
    .on_enable = debug_overlay_on_enable,
    .on_disable = debug_overlay_on_disable
};