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
#include "../utils/time_estimator.h"
#include "mcge/allocator/allocator.h"
#include "mcge/rendering/renderer_pipeline.h"
#include "mcge/scene/scene.h"
#include "mcge/scene/chunks/chunks.h"


#define LINES_NUMBER 20
#define LINE_LENGTH 128
#define DEBUG_CHAR_SIZE 8.f
#define DEBUG_TEXT_Y_MARGIN 2.f


static struct {
    struct engine_arguments args;
    bool visible;
    char lines[LINES_NUMBER][LINE_LENGTH];
    unsigned int renderer_post_pipeline_token;
    unsigned int keyboard_on_hotkey_pressed_token;
} debug_overlay;






static void debug_overlay_println(int *index, const char *format, ...) {
    if (*index >= LINES_NUMBER) {
        return;
    }
    va_list args;
    va_start(args, format);
    vsnprintf(debug_overlay.lines[*index], LINE_LENGTH, format, args);
    va_end(args);
    (*index)++;
}

static void debug_overlay_fill_debug_info() {
    int line_idx = 0;

    debug_overlay_println(&line_idx, "MyCoolGameEngine v%d.%d.%d", ENGINE_V_MAJOR, ENGINE_V_MINOR, ENGINE_V_PATCH);
    debug_overlay_println(&line_idx, "Program: %s. Script: %s. Dev mode: %d",
               debug_overlay.args.program_name, debug_overlay.args.script_path, debug_overlay.args.dev_mode);
    debug_overlay_println(&line_idx, "");

    const double frame_ms   = time_estimator_average_block_ms(profiler_get_frame());
    const double update_ms  = time_estimator_average_block_ms(profiler_get_update());
    const double render_ms  = time_estimator_average_block_ms(profiler_get_rendering());
    const double unknown_ms = frame_ms - update_ms - render_ms;
    const double update_ms_percent  = (update_ms / frame_ms) * 100;
    const double render_ms_percent  = (render_ms / frame_ms) * 100;
    const double unknown_ms_percent = (unknown_ms / frame_ms) * 100;
    const double fps                = 1000.0 / frame_ms;
    debug_overlay_println(&line_idx, "FPS: %.1f. Frame: %.2f ms", fps, frame_ms);
    debug_overlay_println(&line_idx, "Update: %.2f ms (%.1f%%)", update_ms, update_ms_percent);
    debug_overlay_println(&line_idx, "Render: %.2f ms (%.1f%%)", render_ms, render_ms_percent);
    debug_overlay_println(&line_idx, "Unknown: %.2f ms (%.1f%%)", unknown_ms, unknown_ms_percent);
    debug_overlay_println(&line_idx, "");

    const double active_memory_mb = (double)allocator_get_active_allocations_size() / 1024 / 1024;
    const unsigned long long active_allocations_num = allocator_get_active_allocations_num();
    const double total_memory_mb = (double)allocator_get_total_allocations_size() / 1024 / 1024;
    const unsigned long long total_allocations_num = allocator_get_total_allocations_num();
    debug_overlay_println(&line_idx, "Memory allocated: %.1f MB. Active allocations: %llu", active_memory_mb, active_allocations_num);
    debug_overlay_println(&line_idx, "Memory allocations sum: %.1f MB. Total allocations: %llu", total_memory_mb, total_allocations_num);

    debug_overlay_println(&line_idx, "Root entities: %d", scene_get_root_entities_count());
    debug_overlay_println(&line_idx, "Total entities: %d", scene_get_total_entities_count());
    debug_overlay_println(&line_idx, "Components: %d", scene_get_components_count());
    debug_overlay_println(&line_idx, "");

    const unsigned int chunk_num = chunks_get_number(scene_get_chunks());
    const double chunk_size = chunks_get_chunk_size(scene_get_chunks());
    debug_overlay_println(&line_idx, "Chunks number: %d x %d", chunk_num, chunk_num);
    debug_overlay_println(&line_idx, "Chunk size: %.2f x %.2f", chunk_size, chunk_size);
    debug_overlay_println(&line_idx, "");

    const struct renderer_pipeline *pipeline = renderer_get_pipeline();
    const struct rect viewport = renderer_pipeline_get_viewport(pipeline);

    debug_overlay_println(&line_idx, "Renderer pipeline last frame draw calls count: %d",
               renderer_pipeline_get_prev_draw_call_count(pipeline));
    debug_overlay_println(&line_idx, "Renderer pipeline viewport: x %.2f y %.2f w %.2f h %.2f",
               viewport.position.x, viewport.position.y, viewport.size.x, viewport.size.y);
    debug_overlay_println(&line_idx, "");
}



static unsigned int debug_overlay_longest_line_length() {
    unsigned int result = 0;
    for (int i = 0; i < LINES_NUMBER; i++) {
        const unsigned int len = strlen(debug_overlay.lines[i]);
        if (len > result) {
            result = len;
        }
    }
    return result;
}
static unsigned int debug_overlay_lines_number() {
    unsigned int result = 0;
    for (int i = 0; i < LINES_NUMBER; i++) {
        if (debug_overlay.lines[i][0] != '\0') {
            result = i + 1;
        }
    }
    return result;
}













static void handle_post_pipeline(void *listener, void *context) {
    if (!debug_overlay.visible) {
        return;
    }
    debug_overlay_fill_debug_info();
    const SDL_FRect rect = {
        .w = DEBUG_CHAR_SIZE * (float)debug_overlay_longest_line_length(),
        .h = (DEBUG_CHAR_SIZE + DEBUG_TEXT_Y_MARGIN) * (float)(debug_overlay_lines_number())
    };
    SDL_SetRenderDrawColor(renderer_get_native_renderer(), 0, 0, 0, 200);
    SDL_RenderFillRect(renderer_get_native_renderer(), &rect);
    SDL_SetRenderDrawColor(renderer_get_native_renderer(), color_white.r, color_white.g, color_white.b, color_white.a);
    for (int i = 0; i < LINES_NUMBER; i++) {
        SDL_RenderDebugText(renderer_get_native_renderer(), 0, (float)i * (DEBUG_CHAR_SIZE + DEBUG_TEXT_Y_MARGIN), debug_overlay.lines[i]);
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
