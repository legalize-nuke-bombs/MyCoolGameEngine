cmake_minimum_required(VERSION 4.0)
project(MyCoolGameEngine C)

set(CMAKE_C_STANDARD 11)

include(FetchContent)

set(SDL_SHARED OFF)
set(SDL_STATIC ON)
set(SDL_TEST_LIBRARY OFF)

FetchContent_Declare(
        SDL3
        URL https://github.com/libsdl-org/SDL/releases/download/release-3.4.16/SDL3-3.4.16.tar.gz
        URL_HASH SHA256=7322236cd12090c3eb40b9728be4d49c76f66ad17d04369584d4ecad5cf77c68
        EXCLUDE_FROM_ALL
        SYSTEM
)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffunction-sections -fdata-sections")

set(SDL_GPU OFF)
set(SDL_VULKAN OFF)
set(SDL_OPENGL OFF)
set(SDL_OPENGLES OFF)
set(SDL_RENDER_GPU OFF)
set(SDL_RENDER_VULKAN OFF)
set(SDL_RENDER_D3D12 OFF)
set(SDL_RENDER_D3D OFF)
set(SDL_JOYSTICK OFF)
set(SDL_HAPTIC OFF)
set(SDL_HIDAPI OFF)
set(SDL_CAMERA OFF)
set(SDL_SENSOR OFF)
set(SDL_POWER OFF)
set(SDL_DIALOG OFF)
set(SDL_TRAY OFF)

set(BUILD_SHARED_LIBS OFF)
set(SDLIMAGE_VENDORED OFF)
set(SDLIMAGE_DEPS_SHARED OFF)
set(SDLIMAGE_SAMPLES OFF)
set(SDLIMAGE_TESTS OFF)
set(SDLIMAGE_PNG_LIBPNG OFF)
set(SDLIMAGE_PNG_SAVE OFF)
foreach(fmt ANI AVIF BMP GIF JPG JXL LBM PCX PNM QOI SVG TGA TIF WEBP XCF XPM XV)
    set(SDLIMAGE_${fmt} OFF)
endforeach()

FetchContent_Declare(
        SDL3_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-3.4.6
        GIT_SHALLOW TRUE
        EXCLUDE_FROM_ALL
        SYSTEM
)

FetchContent_MakeAvailable(SDL3 SDL3_image)

add_executable(MyCoolGameEngine main.c
        src/scene/entity.h
        src/scene/entity.c
        src/utils/vector2.h
        src/utils/vector2.c
        src/utils/vector2_math.h
        src/utils/vector2_math.c
        src/scene/components/component.h
        src/scene/components/component.c
        src/scene/components/component_internal.h
        src/logging/logger.h
        src/logging/logger.c
        src/engine/update_context.h
        src/scene/scene.h
        src/scene/scene.c
        src/engine/version.h
        src/utils/list.h
        src/utils/list.c
        src/utils/typed_list.h
        src/utils/dictionary.h
        src/utils/dictionary.c
        src/utils/string_dictionary.h
        src/utils/string_hasher.h
        src/utils/string_hasher.c
        src/utils/string_dictionary.c
        src/utils/pointer_dictionary.h
        src/utils/pointer_dictionary.c
        src/scene/tmap/tmap.h
        src/scene/tmap/tmap.c
        src/utils/action.h
        src/utils/action.c
        src/scene/components/rendering/camera.h
        src/scene/components/rendering/camera.c
        src/scene/entity_collection.h
        src/scene/entity_collection.c
        example.c
        src/rendering/renderer_pipeline.h
        src/rendering/renderer_pipeline.c
        src/utils/rect.h
        src/rendering/primitives/renderer_primitive.h
        src/rendering/layers/renderer_layer.h
        src/rendering/primitives/renderer_primitive_internal.h
        src/rendering/primitives/renderer_primitive.c
        src/rendering/primitives/custom/renderer_square.h
        src/rendering/primitives/custom/renderer_square.c
        src/utils/rect_math.h
        src/utils/rect_math.c
        src/scene/components/rendering/box_renderer.h
        src/scene/components/rendering/box_renderer.c
        src/utils/color.h
        src/utils/parser.h
        src/utils/parser.c
        src/interpreter/interpreter.h
        src/interpreter/interpreter.c
        src/interpreter/interpreter_command.h
        src/interpreter/core/interpreter_print.h
        src/interpreter/core/interpreter_print.c
        src/interpreter/interpreter_command_register.h
        src/interpreter/interpreter_command_register.c
        src/engine/engine.h
        src/engine/engine.c
        src/interpreter/interpreter_command_internal.h
        src/interpreter/interpreter_command.c
        src/interpreter/interpreter_command_parent.h
        src/interpreter/interpreter_command_parent.c
        src/interpreter/window/interpreter_window.h
        src/interpreter/window/interpreter_window.c
        src/logging/ansi.h
        src/rendering/layers/renderer_layer_manager.h
        src/rendering/layers/renderer_layer_manager.c
        src/rendering/layers/renderer_layer.c
        src/interpreter/renderer_layer_manager/interpreter_renderer_layer_manager.h
        src/interpreter/renderer_layer_manager/interpreter_renderer_layer_manager.c
        src/interpreter/renderer_layer_manager/interpreter_renderer_layer_manager_new_layer.h
        src/interpreter/renderer_layer_manager/interpreter_renderer_layer_manager_new_layer.c
        src/interpreter/scene/interpreter_scene.h
        src/interpreter/scene/interpreter_scene.c
        src/scene/components/component_fabric.h
        src/scene/components/component_fabric.c
        src/interpreter/scene/interpreter_scene_add.h
        src/interpreter/scene/interpreter_scene_add.c
        src/devices/keyboard.h
        src/devices/keyboard.c
        src/profiler/profiler.h
        src/profiler/profiler.c
        src/profiler/time_estimator.h
        src/profiler/time_estimator.c
        src/scene/components/core/idle.h
        src/scene/components/core/idle.c
        src/utils/file_listener.h
        src/utils/file_listener.c
        src/engine/events/engine_events.h
        src/engine/events/engine_events.c
        src/rendering/renderer.h
        src/rendering/renderer.c
        src/engine/utils/engine_closer.h
        src/engine/utils/engine_closer.c
        src/engine/utils/engine_restarter.h
        src/engine/utils/engine_restarter.c
        src/subsystems/subsystem.h
        src/subsystems/subsystem.c
        src/subsystems/subsystem_internal.h
        src/subsystems/subsystem_collection.h
        src/subsystems/subsystem_collection.c
        src/engine/lifecycle/engine_lifecycle.h
        src/engine/lifecycle/engine_lifecycle.c
        src/engine/engine_arguments.h
        src/scene/components/core/controller.h
        src/scene/components/core/controller.c
        src/scene/components/core/keyboard_controller.h
        src/scene/components/core/keyboard_controller.c
        src/scene/components/rendering/box_light.h
        src/scene/components/rendering/box_light.c
        src/scene/components/rendering/renderer_settings.h
        src/scene/components/rendering/renderer_settings.c
        src/interpreter/core/interpreter_ignore.h
        src/interpreter/core/interpreter_ignore.c
        src/rendering/primitives/custom/light_map.h
        src/rendering/primitives/custom/light_map.c
        src/utils/rect.c
        src/rendering/textures/texture_manager.h
        src/rendering/textures/texture_manager.c
        src/rendering/textures/texture.h
        src/rendering/textures/texture.c
        src/rendering/textures/texture_loading_mode.h
        src/interpreter/texture_manager/interpreter_texture_manager.h
        src/interpreter/texture_manager/interpreter_texture_manager.c
        src/interpreter/texture_manager/interpreter_texture_manager_new_texture.h
        src/interpreter/texture_manager/interpreter_texture_manager_new_texture.c
        src/utils/color.c
        src/scene/components/rendering/box_renderer_animated.h
        src/scene/components/rendering/box_renderer_animated.c
        src/scene/prefabs/prefab.h
        src/scene/prefabs/prefab.c
        src/scene/prefabs/prefab_manager.h
        src/scene/prefabs/prefab_manager.c
        src/scene/components/demo/forest.h
        src/scene/components/demo/forest.c
        src/random/random.h
        src/random/random.c
        src/scene/components/core/pulsator.h
        src/scene/components/core/pulsator.c
        src/utils/rect_pair.h
        src/scene/chunks/chunks.h
        src/scene/chunks/chunks.c
        src/scene/chunks/chunk.h
        src/scene/chunks/chunk.c
        src/scene/components/demo/scene_switcher.h
        src/scene/components/demo/scene_switcher.c
        src/utils/path.h
        src/utils/path.c
        src/interpreter/core/interpreter_eval.h
        src/interpreter/core/interpreter_eval.c
        src/interpreter/core/interpreter_once.h
        src/interpreter/core/interpreter_once.c
        src/scene/components/demo/game_closer.h
        src/scene/components/demo/game_closer.c
        src/scene/components/core/clock.h
        src/scene/components/core/clock.c
        src/scene/components/core/sky.h
        src/scene/components/core/sky.c
        src/scene/components/physics/rigid_body.h
        src/scene/components/physics/rigid_body.c
        src/scene/physics/rigid_material.h
        src/scene/physics/rigid_material.c
        src/scene/physics/rigid_layer.h
        src/scene/physics/rigid_layer.c
        src/scene/physics/rigid_layers.h
        src/scene/physics/rigid_layers.c
        src/scene/physics/rigid_materials.h
        src/scene/physics/rigid_materials.c
)

target_link_libraries(MyCoolGameEngine PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
target_link_options(MyCoolGameEngine PRIVATE $<$<CONFIG:Release>:-s -Wl,--gc-sections>)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error LANGUAGES C)
if(ipo_supported)
    set_property(TARGET MyCoolGameEngine PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
else()
    message(WARNING "LTO is off: ${ipo_error}")
endif()