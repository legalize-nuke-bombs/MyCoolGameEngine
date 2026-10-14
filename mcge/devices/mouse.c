//
// Created by nikita on 13.10.2026.
//

#include "mouse.h"
#include "../engine/msystem.h"
#include "mcge/engine/events/engine_events.h"
#include "mcge/utils/action.h"
#include <SDL3/SDL.h>

#include "mcge/logging/logger.h"
#include "mcge/rendering/renderer.h"
#include "mcge/rendering/renderer_pipeline.h"
#include "mcge/utils/vector2_math.h"


static struct {
    struct action on_wheel_scrolled;
    struct action on_button_pressed;
    struct action on_button_released;

    struct action* on_native_event;
    unsigned int on_native_event_token;
} mouse;



static void mouse_on_create(void) {
    mouse.on_wheel_scrolled = action_create();
    mouse.on_button_pressed = action_create();
    mouse.on_button_released = action_create();
}
static void mouse_on_destroy(void) {
    action_destroy(&mouse.on_wheel_scrolled);
    action_destroy(&mouse.on_button_pressed);
    action_destroy(&mouse.on_button_released);
}


static void handle_native_event(void *listener, void *context);

static void mouse_on_enable(struct engine_arguments args) {
    mouse.on_native_event = engine_events_on_native_event();
    action_subscribe(mouse.on_native_event, NULL, handle_native_event, &mouse.on_native_event_token);
}
static void mouse_on_disable(void) {
    action_unsubscribe(mouse.on_native_event, mouse.on_native_event_token);
}


const struct msystem mouse_msystem = {
    .name = "mouse",
    .on_create = mouse_on_create,
    .on_destroy = mouse_on_destroy,
    .on_enable = mouse_on_enable,
    .on_disable = mouse_on_disable
};




static void handle_native_event(void *listener, void *context) {
    const SDL_Event *event = context;
    switch (event->type) {
        case SDL_EVENT_MOUSE_WHEEL: {
            struct mouse_on_wheel_scrolled_callback_context cnt = {
                .dx = -event->wheel.x,
                .dy = -event->wheel.y
            };
            action_invoke(&mouse.on_wheel_scrolled, &cnt);
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            struct mouse_on_button_callback_context cnt = {
                .button = event->button.button,
                .position = mouse_get_position()
            };
            action_invoke(&mouse.on_button_pressed, &cnt);
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            struct mouse_on_button_callback_context cnt = {
                .button = event->button.button,
                .position = mouse_get_position()
            };
            action_invoke(&mouse.on_button_released, &cnt);
            break;
        }
        default:
            break;
    }
}



struct action* mouse_get_on_wheel_scrolled() {
    return &mouse.on_wheel_scrolled;
}
struct action* mouse_get_on_button_pressed() {
    return &mouse.on_button_pressed;
}
struct action* mouse_get_on_button_released() {
    return &mouse.on_button_released;
}


struct vector2 mouse_get_position() {
    float mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    const struct vector2 screen_size = renderer_pipeline_get_output_size(renderer_get_pipeline());
    const struct rect viewport = renderer_pipeline_get_viewport(renderer_get_pipeline());

    const struct vector2 mouse_pos = { .x = mouse_x, .y = mouse_y };
    const struct vector2 half_screen = vector_multiply_scalar(screen_size, 0.5f);

    const struct vector2 pixel_offset = vector_sub(mouse_pos, half_screen);

    const struct vector2 invert_y = { .x = 1.0f, .y = -1.0f };
    const struct vector2 corrected_pixel_offset = vector_multiply_vector(pixel_offset, invert_y);

    const struct vector2 pixels_per_meter = {
        .x = screen_size.x / viewport.size.x,
        .y = screen_size.y / viewport.size.y
    };

    const struct vector2 inv_ppm = { .x = 1.0f / pixels_per_meter.x, .y = 1.0f / pixels_per_meter.y };
    const struct vector2 meter_offset = vector_multiply_vector(corrected_pixel_offset, inv_ppm);

    const struct vector2 result = vector_sum(viewport.position, meter_offset);

    return result;
}



