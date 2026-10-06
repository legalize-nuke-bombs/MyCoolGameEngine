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

add_library(mcge STATIC
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
        src/scene/components/core/camera.h
        src/scene/components/core/camera.c
        src/scene/entity_collection.h
        src/scene/entity_collection.c
        src/rendering/renderer_pipeline.h
        src/rendering/renderer_pipeline.c
        src/utils/rect.h
        src/rendering/primitives/renderer_primitive.h
        src/rendering/renderer_layer.h
        src/rendering/primitives/renderer_primitive_internal.h
        src/rendering/primitives/renderer_primitive.c
        src/rendering/primitives/custom/renderer_square.h
        src/rendering/primitives/custom/renderer_square.c
        src/utils/rect_math.h
        src/utils/rect_math.c
        src/scene/components/rendering/box_renderer.h
        src/scene/components/rendering/box_renderer.c
        src/utils/color.h
        src/interpreter/interpreter.h
        src/interpreter/interpreter.c
        src/engine/engine.h
        src/engine/engine.c
        src/logging/ansi.h
        src/rendering/renderer_layer.c
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
        src/msystems/msystem.h
        src/engine/lifecycle/engine_lifecycle.h
        src/engine/lifecycle/engine_lifecycle.c
        src/engine/engine_arguments.h
        src/scene/components/movement/controller.h
        src/scene/components/movement/controller.c
        src/scene/components/movement/keyboard_controller.h
        src/scene/components/movement/keyboard_controller.c
        src/scene/components/rendering/box_light.h
        src/scene/components/rendering/box_light.c
        src/scene/components/rendering/renderer_settings.h
        src/scene/components/rendering/renderer_settings.c
        src/rendering/primitives/custom/light_map.h
        src/rendering/primitives/custom/light_map.c
        src/utils/rect.c
        src/rendering/texture.h
        src/rendering/texture.c
        src/utils/color.c
        src/scene/components/rendering/box_renderer_animated.h
        src/scene/components/rendering/box_renderer_animated.c
        src/scene/prefabs/prefab.h
        src/scene/prefabs/prefab.c
        src/random/random.h
        src/random/random.c
        src/utils/rect_pair.h
        src/scene/chunks/chunks.h
        src/scene/chunks/chunks.c
        src/scene/chunks/chunk.h
        src/scene/chunks/chunk.c
        src/scene/components/physics/rigid_body.h
        src/scene/components/physics/rigid_body.c
        src/modules/physics/rigid_material.h
        src/modules/physics/rigid_material.c
        src/modules/physics/rigid_layer.h
        src/modules/physics/rigid_layer.c
        src/scene/components/physics/rigid_surface.h
        src/scene/components/physics/rigid_surface.c
        src/scene/components/physics/keyboard_rigid_controller.h
        src/scene/components/physics/keyboard_rigid_controller.c
        src/assets/asset_type.h
        src/assets/assets.h
        src/assets/assets.c
        src/assets/asset_storage.h
        src/assets/asset_storage.c
        src/scene/components/core/simulator.h
        src/scene/components/core/simulator.c
        src/scene/components/physics/collider.h
        src/scene/components/physics/collider.c
        src/modules/bt/bt_node.h
        src/modules/bt/bt_status.h
        src/modules/bt/bt_node_internal.h
        src/modules/bt/bt_node.c
        src/modules/bt/core/bt_sequence.h
        src/modules/bt/core/bt_sequence.c
        src/modules/bt/core/bt_selector.h
        src/modules/bt/core/bt_selector.c
        src/modules/bt/bt_node_factory.h
        src/modules/bt/bt_node_factory.c
        src/modules/bt/bt_graph.h
        src/modules/bt/bt_graph.c
        src/utils/factory.h
        src/utils/factory.c
        src/scene/components/component_factory.h
        src/scene/components/component_factory.c
        src/utils/system_message_box.h
        src/utils/system_message_box.c
        src/utils/fields.h
        src/utils/fields.c
        src/modules/bt/bt_module.h
        src/modules/bt/bt_module.c
        src/scene/entity_factory.h
        src/scene/entity_factory.c
        src/modules/bt/bt_graphs.h
        src/modules/bt/bt_graphs.c
        src/modules/physics/rigid_layers.h
        src/modules/physics/rigid_materials.h
        src/modules/physics/rigid_layers.c
        src/modules/physics/rigid_materials.c
        src/rendering/renderer_layers.h
        src/rendering/textures.h
        src/rendering/renderer_layers.c
        src/rendering/textures.c
        src/scene/prefabs/prefabs.h
        src/scene/prefabs/prefabs.c
        src/modules/physics/collision_layer.h
        src/modules/physics/collision_layers.h
        src/modules/physics/collision_response.h
        src/modules/physics/collision_rules.h
        src/modules/physics/collision_layer.c
        src/modules/physics/collision_layers.c
        src/modules/physics/collision_response.c
        src/modules/physics/collision_rules.c
        src/utils/memory/control_block.h
        src/utils/memory/holder.h
        src/utils/memory/ref.h
        src/utils/memory/holder.c
        src/utils/memory/ref.c
)
target_include_directories(mcge PUBLIC src)
target_link_libraries(mcge PUBLIC SDL3::SDL3 SDL3_image::SDL3_image)

add_executable(MyCoolGameEngine demo/main.c
        demo/example.c
        demo/src/world/forest.h
        demo/src/world/forest.c
        demo/src/special/pulsator.h
        demo/src/special/pulsator.c
        demo/src/scenes/scene_switcher.h
        demo/src/scenes/scene_switcher.c
        demo/src/scenes/game_closer.h
        demo/src/scenes/game_closer.c
        demo/src/enviornment/clock.h
        demo/src/enviornment/clock.c
        demo/src/enviornment/sky.h
        demo/src/enviornment/sky.c
        demo/src/characters/player.h
        demo/src/characters/player.c
        demo/src/characters/bt/blackboard.h
        demo/src/characters/bt/player_is_near.h
        demo/src/characters/bt/player_is_near.c
        demo/src/characters/bt/behaviour_agent.h
        demo/src/characters/bt/behaviour_agent.c
        demo/src/characters/bt/player_chase.h
        demo/src/characters/bt/player_chase.c
        demo/src/version.h
        demo/src/characters/mana.h
        demo/src/characters/mana.c
        demo/src/characters/skills/skill.h
        demo/src/characters/skills/skill.c
        demo/src/characters/skills/skill_internal.h
        demo/src/characters/skills/custom/printer.h
        demo/src/characters/skills/custom/printer.c
        demo/src/characters/skills/skilled.h
        demo/src/characters/skills/skilled.c
        demo/src/characters/skills/skill_factory.h
        demo/src/characters/skills/skill_factory.c
        demo/src/characters/skills/custom/fear_balls/fear_balls.h
        demo/src/characters/skills/custom/fear_balls/fear_balls.c
        demo/src/characters/skills/custom/fear_balls/fear_ball.h
        demo/src/characters/skills/custom/fear_balls/fear_ball.c
        demo/src/characters/effects.h
        demo/src/characters/effects.c
        demo/src/characters/bt/player_run_away.h
        demo/src/characters/bt/player_run_away.c
        demo/src/characters/bt/is_scared.h
        demo/src/characters/bt/is_scared.c
        demo/src/demo.h
        demo/src/demo.c
)

target_link_libraries(MyCoolGameEngine PRIVATE mcge)
target_link_options(MyCoolGameEngine PRIVATE $<$<CONFIG:Release>:-s -Wl,--gc-sections>)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error LANGUAGES C)
if(ipo_supported)
    set_property(TARGET mcge MyCoolGameEngine PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
else()
    message(WARNING "LTO is off: ${ipo_error}")
endif()