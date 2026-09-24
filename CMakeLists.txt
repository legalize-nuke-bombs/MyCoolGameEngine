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
FetchContent_MakeAvailable(SDL3)

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
        src/scene/components/demo/printer.h
        src/scene/components/demo/printer.c
        src/scene/update_context.h
        src/scene/scene.h
        src/scene/scene.c
        version.h
        src/scene/components/core/transform.h
        src/scene/components/core/transform.c
        src/utils/list.h
        src/utils/list.c
        src/utils/dictionary.h
        src/utils/dictionary.c
        src/utils/string_dictionary.h
        src/utils/string_hasher.h
        src/utils/string_hasher.c
        src/utils/string_dictionary.c
        src/scene/tmap.h
        src/scene/tmap.c
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
        src/rendering/renderer_primitive.h
        src/rendering/renderer_layer.h
        src/rendering/renderer_primitive_internal.h
        src/rendering/renderer_primitive.c
        src/rendering/custom/renderer_square.h
        src/rendering/custom/renderer_square.c
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
        src/interpreter/custom/interpreter_print.h
        src/interpreter/custom/interpreter_print.c
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
        src/interpreter/window/interpreter_window_new.h
        src/interpreter/window/interpreter_window_new.c
        src/rendering/renderer_layer_manager.h
        src/rendering/renderer_layer_manager.c
        src/rendering/renderer_layer.c
        src/interpreter/renderer_layer_manager/interpreter_renderer_layer_manager.h
        src/interpreter/renderer_layer_manager/interpreter_renderer_layer_manager.c
        src/interpreter/renderer_layer_manager/interpreter_renderer_layer_manager_new_layer.h
        src/interpreter/renderer_layer_manager/interpreter_renderer_layer_manager_new_layer.c
        src/interpreter/scene/interpreter_scene.h
        src/interpreter/scene/interpreter_scene.c
        src/interpreter/scene/interpreter_scene_new.h
        src/interpreter/scene/interpreter_scene_new.c
        src/scene/components/component_fabric.h
        src/scene/components/component_fabric.c
        src/interpreter/scene/interpreter_scene_new_entity.h
        src/interpreter/scene/interpreter_scene_new_entity.c
        src/scene/components/demo/suicidal.h
        src/scene/components/demo/suicidal.c
        src/scene/components/demo/armageddon.h
        src/scene/components/demo/armageddon.c
        src/devices/keyboard.h
        src/devices/keyboard.c
        src/devices/devices.h
        src/devices/devices.c
        src/profiler/profiler.h
        src/profiler/profiler.c
        src/profiler/time_estimator.h
        src/profiler/time_estimator.c
)

target_link_libraries(MyCoolGameEngine PRIVATE SDL3::SDL3)
target_link_options(MyCoolGameEngine PRIVATE $<$<CONFIG:Release>:-s -Wl,--gc-sections>)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error LANGUAGES C)
if(ipo_supported)
    set_property(TARGET MyCoolGameEngine PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
else()
    message(WARNING "LTO is off: ${ipo_error}")
endif()
