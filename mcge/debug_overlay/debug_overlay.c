//
// Created by nikita on 15.10.2026.
//

#include "debug_overlay.h"

#include "mcge/devices/keyboard.h"
#include "mcge/engine/msystem.h"
#include "mcge/rendering/renderer.h"
#include "mcge/utils/action.h"
#include "mcge/utils/color.h"
#include "SDL3/SDL_render.h"



#define BUFFER_SIZE 2048


static struct {
    bool enabled;
    bool visible;
    char *buffer;
    unsigned int renderer_post_pipeline_token;
    unsigned int keyboard_on_hotkey_pressed_token;
} debug_overlay;


static void handle_post_pipeline(void *listener, void *context) {
    if (!debug_overlay.visible) {
        return;
    }
    SDL_SetRenderDrawColor(renderer_get_native_renderer(), color_white.r, color_white.g, color_white.b, color_white.a);
    SDL_RenderDebugText(renderer_get_native_renderer(), 0, 0, debug_overlay.buffer);
}

static void handle_hotkey_pressed(void *listener, void *context) {
    debug_overlay.visible = !debug_overlay.visible;
}


static void debug_overlay_on_enable(const struct engine_arguments args) {
    debug_overlay.enabled = args.dev_mode;
    if (debug_overlay.enabled) {
        debug_overlay.visible = false;
        debug_overlay.buffer = calloc(BUFFER_SIZE, sizeof(char));
        action_subscribe(renderer_get_post_pipeline(), NULL, handle_post_pipeline, &debug_overlay.renderer_post_pipeline_token);
        action_subscribe(keyboard_require_action_on_key_pressed("F3"), NULL, handle_hotkey_pressed, &debug_overlay.keyboard_on_hotkey_pressed_token);
    }
}
static void debug_overlay_on_disable(void) {
    if (debug_overlay.enabled) {
        debug_overlay.enabled = false;
        free(debug_overlay.buffer);
        action_unsubscribe(renderer_get_post_pipeline(), debug_overlay.renderer_post_pipeline_token);
        action_unsubscribe(keyboard_require_action_on_key_pressed("F3"), debug_overlay.keyboard_on_hotkey_pressed_token);
    }
}


const struct msystem debug_overlay_msystem = {
    .name = "debug_overlay",
    .on_enable = debug_overlay_on_enable,
    .on_disable = debug_overlay_on_disable
};