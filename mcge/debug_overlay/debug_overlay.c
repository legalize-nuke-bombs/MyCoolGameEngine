//
// Created by nikita on 15.10.2026.
//

#include "debug_overlay.h"

#include "mcge/devices/keyboard.h"
#include "mcge/engine/msystem.h"
#include "mcge/rendering/renderer.h"
#include "mcge/utils/action.h"
#include "mcge/utils/color.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include "mcge/engine/version.h"
#include "mcge/profiler/profiler.h"
#include "mcge/profiler/time_estimator.h"
#include "mcge/rendering/renderer_pipeline.h"
#include "mcge/scene/scene.h"
#include "mcge/scene/chunks/chunks.h"


#define LINES_NUMBER 20
#define LINE_LENGTH 256
#define DEBUG_CHAR_SIZE 8
#define DEBUG_TEXT_Y_MARGIN 2


static struct {
    struct engine_arguments args;
    bool visible;
    char lines[LINES_NUMBER][LINE_LENGTH];
    unsigned int renderer_post_pipeline_token;
    unsigned int keyboard_on_hotkey_pressed_token;
} debug_overlay;






// This is kinda ugly
static void debug_overlay_fill_debug_info() {
    snprintf(debug_overlay.lines[0], sizeof(char) * LINE_LENGTH, "MyCoolGameEngine v%d.%d.%d", ENGINE_V_MAJOR, ENGINE_V_MINOR, ENGINE_V_PATCH);
    snprintf(debug_overlay.lines[1], sizeof(char) * LINE_LENGTH, "Program: %s. Script: %s. Dev mode: %d", debug_overlay.args.program_name, debug_overlay.args.script_path, debug_overlay.args.dev_mode);
    snprintf(debug_overlay.lines[2], sizeof(char) * LINE_LENGTH, "");

    const double frame_ms = time_estimator_average_block_ms(profiler_get_frame());
    const double update_ms = time_estimator_average_block_ms(profiler_get_update());
    const double render_ms = time_estimator_average_block_ms(profiler_get_rendering());
    const double unknown_ms = frame_ms - update_ms - render_ms;
    const double update_ms_percent = update_ms / frame_ms * 100;
    const double render_ms_percent = render_ms / frame_ms * 100;
    const double unknown_ms_percent = unknown_ms / frame_ms * 100;
    const double fps = 1000 / frame_ms;
    snprintf(debug_overlay.lines[3], sizeof(char) * LINE_LENGTH, "FPS: %f. Frame: %f ms", fps, frame_ms);
    snprintf(debug_overlay.lines[4], sizeof(char) * LINE_LENGTH, "Update: %f ms (%f%%)", update_ms, update_ms_percent);
    snprintf(debug_overlay.lines[5], sizeof(char) * LINE_LENGTH, "Render: %f ms (%f%%)", render_ms, render_ms_percent);
    snprintf(debug_overlay.lines[6], sizeof(char) * LINE_LENGTH, "Unknown: %f ms (%f%%)", unknown_ms, unknown_ms_percent);
    snprintf(debug_overlay.lines[7], sizeof(char) * LINE_LENGTH, "");

    snprintf(debug_overlay.lines[8], sizeof(char) * LINE_LENGTH, "Root entities: %d", scene_get_root_entities_count());
    snprintf(debug_overlay.lines[9], sizeof(char) * LINE_LENGTH, "Total entities: %d", scene_get_total_entities_count());
    snprintf(debug_overlay.lines[10], sizeof(char) * LINE_LENGTH, "Components: %d", scene_get_components_count());
    snprintf(debug_overlay.lines[11], sizeof(char) * LINE_LENGTH, "");

    snprintf(debug_overlay.lines[12], sizeof(char) * LINE_LENGTH, "Chunks number: %d x %d", chunks_get_number(scene_get_chunks()), chunks_get_number(scene_get_chunks()));
    snprintf(debug_overlay.lines[13], sizeof(char) * LINE_LENGTH, "Chunk size: %f x %f", chunks_get_chunk_size(scene_get_chunks()), chunks_get_chunk_size(scene_get_chunks()));
    snprintf(debug_overlay.lines[14], sizeof(char) * LINE_LENGTH, "");

    const struct renderer_pipeline *pipeline = renderer_get_pipeline();
    const struct rect viewport = renderer_pipeline_get_viewport(pipeline);
    snprintf(debug_overlay.lines[15], sizeof(char) * LINE_LENGTH, "Renderer pipeline last frame draw calls count: %d", renderer_pipeline_get_prev_draw_call_count(pipeline));
    snprintf(debug_overlay.lines[16], sizeof(char) * LINE_LENGTH, "Renderer pipeline viewport: x %f y %f w %f h %f", viewport.position.x, viewport.position.y, viewport.size.x, viewport.size.y);
    snprintf(debug_overlay.lines[17], sizeof(char) * LINE_LENGTH, "");


}














static void handle_post_pipeline(void *listener, void *context) {
    if (!debug_overlay.visible) {
        return;
    }
    debug_overlay_fill_debug_info();
    SDL_SetRenderDrawColor(renderer_get_native_renderer(), color_white.r, color_white.g, color_white.b, color_white.a);
    for (int i = 0; i < LINES_NUMBER; i++) {
        SDL_RenderDebugText(renderer_get_native_renderer(), 0, (float)(i * (DEBUG_CHAR_SIZE + DEBUG_TEXT_Y_MARGIN)), debug_overlay.lines[i]);
    }
}

static void handle_hotkey_pressed(void *listener, void *context) {
    debug_overlay.visible = !debug_overlay.visible;
}


static void debug_overlay_on_enable(const struct engine_arguments args) {
    debug_overlay.args = args;
    if (debug_overlay.args.dev_mode) {
        debug_overlay.visible = false;
        action_subscribe(renderer_get_post_pipeline(), NULL, handle_post_pipeline, &debug_overlay.renderer_post_pipeline_token);
        action_subscribe(keyboard_require_action_on_key_pressed("F3"), NULL, handle_hotkey_pressed, &debug_overlay.keyboard_on_hotkey_pressed_token);
    }
}
static void debug_overlay_on_disable(void) {
    if (debug_overlay.args.dev_mode) {
        action_unsubscribe(renderer_get_post_pipeline(), debug_overlay.renderer_post_pipeline_token);
        action_unsubscribe(keyboard_require_action_on_key_pressed("F3"), debug_overlay.keyboard_on_hotkey_pressed_token);
    }
}


const struct msystem debug_overlay_msystem = {
    .name = "debug_overlay",
    .on_enable = debug_overlay_on_enable,
    .on_disable = debug_overlay_on_disable
};