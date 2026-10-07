cmake_minimum_required(VERSION 4.0)
project(MyCoolGameEngine C)

set(CMAKE_C_STANDARD 11)

include(FetchContent)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(SDL_SHARED ON)
set(SDL_STATIC OFF)
set(SDL_TEST_LIBRARY OFF)

FetchContent_Declare(
        SDL3
        URL https://github.com/libsdl-org/SDL/releases/download/release-3.4.16/SDL3-3.4.16.tar.gz
        URL_HASH SHA256=7322236cd12090c3eb40b9728be4d49c76f66ad17d04369584d4ecad5cf77c68
        EXCLUDE_FROM_ALL
        SYSTEM
)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffunction-sections -fdata-sections")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -s")

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

set(BUILD_SHARED_LIBS ON)
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

add_library(mcge SHARED
        mcge/api.h
        mcge/mcge.h
        mcge/scene/entity.h
        mcge/scene/entity.c
        mcge/utils/vector2.h
        mcge/utils/vector2.c
        mcge/utils/vector2_math.h
        mcge/utils/vector2_math.c
        mcge/scene/components/component.h
        mcge/scene/components/component.c
        mcge/scene/components/component_internal.h
        mcge/logging/logger.h
        mcge/logging/logger.c
        mcge/engine/update_context.h
        mcge/scene/scene.h
        mcge/scene/scene.c
        mcge/engine/version.h
        mcge/utils/list.h
        mcge/utils/list.c
        mcge/utils/typed_list.h
        mcge/utils/dictionary.h
        mcge/utils/dictionary.c
        mcge/utils/string_dictionary.h
        mcge/utils/string_hasher.h
        mcge/utils/string_hasher.c
        mcge/utils/string_dictionary.c
        mcge/utils/pointer_dictionary.h
        mcge/utils/pointer_dictionary.c
        mcge/scene/tmap/tmap.h
        mcge/scene/tmap/tmap.c
        mcge/utils/action.h
        mcge/utils/action.c
        mcge/scene/components/core/camera.h
        mcge/scene/components/core/camera.c
        mcge/scene/entity_collection.h
        mcge/scene/entity_collection.c
        mcge/rendering/renderer_pipeline.h
        mcge/rendering/renderer_pipeline.c
        mcge/utils/rect.h
        mcge/rendering/primitives/renderer_primitive.h
        mcge/rendering/renderer_layer.h
        mcge/rendering/primitives/renderer_primitive_internal.h
        mcge/rendering/primitives/renderer_primitive.c
        mcge/rendering/primitives/custom/renderer_square.h
        mcge/rendering/primitives/custom/renderer_square.c
        mcge/utils/rect_math.h
        mcge/utils/rect_math.c
        mcge/scene/components/rendering/box_renderer.h
        mcge/scene/components/rendering/box_renderer.c
        mcge/utils/color.h
        mcge/interpreter/interpreter.h
        mcge/interpreter/interpreter.c
        mcge/engine/engine.h
        mcge/engine/engine.c
        mcge/logging/ansi.h
        mcge/rendering/renderer_layer.c
        mcge/devices/keyboard.h
        mcge/devices/keyboard.c
        mcge/profiler/profiler.h
        mcge/profiler/profiler.c
        mcge/profiler/time_estimator.h
        mcge/profiler/time_estimator.c
        mcge/scene/components/core/idle.h
        mcge/scene/components/core/idle.c
        mcge/utils/file_listener.h
        mcge/utils/file_listener.c
        mcge/engine/events/engine_events.h
        mcge/engine/events/engine_events.c
        mcge/rendering/renderer.h
        mcge/rendering/renderer.c
        mcge/engine/utils/engine_closer.h
        mcge/engine/utils/engine_closer.c
        mcge/engine/utils/engine_restarter.h
        mcge/engine/utils/engine_restarter.c
        mcge/msystems/msystem.h
        mcge/engine/lifecycle/engine_lifecycle.h
        mcge/engine/lifecycle/engine_lifecycle.c
        mcge/engine/engine_arguments.h
        mcge/scene/components/movement/controller.h
        mcge/scene/components/movement/controller.c
        mcge/scene/components/movement/keyboard_controller.h
        mcge/scene/components/movement/keyboard_controller.c
        mcge/scene/components/rendering/box_light.h
        mcge/scene/components/rendering/box_light.c
        mcge/scene/components/rendering/renderer_settings.h
        mcge/scene/components/rendering/renderer_settings.c
        mcge/rendering/primitives/custom/light_map.h
        mcge/rendering/primitives/custom/light_map.c
        mcge/utils/rect.c
        mcge/rendering/texture.h
        mcge/rendering/texture.c
        mcge/utils/color.c
        mcge/scene/components/rendering/box_renderer_animated.h
        mcge/scene/components/rendering/box_renderer_animated.c
        mcge/scene/prefabs/prefab.h
        mcge/scene/prefabs/prefab.c
        mcge/random/random.h
        mcge/random/random.c
        mcge/utils/rect_pair.h
        mcge/scene/chunks/chunks.h
        mcge/scene/chunks/chunks.c
        mcge/scene/chunks/chunk.h
        mcge/scene/chunks/chunk.c
        mcge/scene/components/physics/rigid_body.h
        mcge/scene/components/physics/rigid_body.c
        mcge/modules/physics/rigid_material.h
        mcge/modules/physics/rigid_material.c
        mcge/modules/physics/rigid_layer.h
        mcge/modules/physics/rigid_layer.c
        mcge/scene/components/physics/rigid_surface.h
        mcge/scene/components/physics/rigid_surface.c
        mcge/scene/components/physics/keyboard_rigid_controller.h
        mcge/scene/components/physics/keyboard_rigid_controller.c
        mcge/assets/asset_type.h
        mcge/assets/assets.h
        mcge/assets/assets.c
        mcge/assets/asset_storage.h
        mcge/assets/asset_storage.c
        mcge/scene/components/core/simulator.h
        mcge/scene/components/core/simulator.c
        mcge/scene/components/physics/collider.h
        mcge/scene/components/physics/collider.c
        mcge/modules/bt/bt_node.h
        mcge/modules/bt/bt_status.h
        mcge/modules/bt/bt_node_internal.h
        mcge/modules/bt/bt_node.c
        mcge/modules/bt/core/bt_sequence.h
        mcge/modules/bt/core/bt_sequence.c
        mcge/modules/bt/core/bt_selector.h
        mcge/modules/bt/core/bt_selector.c
        mcge/modules/bt/bt_node_factory.h
        mcge/modules/bt/bt_node_factory.c
        mcge/modules/bt/bt_graph.h
        mcge/modules/bt/bt_graph.c
        mcge/utils/factory.h
        mcge/utils/factory.c
        mcge/scene/components/component_factory.h
        mcge/scene/components/component_factory.c
        mcge/utils/system_message_box.h
        mcge/utils/system_message_box.c
        mcge/utils/fields.h
        mcge/utils/fields.c
        mcge/modules/bt/bt_module.h
        mcge/modules/bt/bt_module.c
        mcge/scene/entity_factory.h
        mcge/scene/entity_factory.c
        mcge/modules/bt/bt_graphs.h
        mcge/modules/bt/bt_graphs.c
        mcge/modules/physics/rigid_layers.h
        mcge/modules/physics/rigid_materials.h
        mcge/modules/physics/rigid_layers.c
        mcge/modules/physics/rigid_materials.c
        mcge/rendering/renderer_layers.h
        mcge/rendering/textures.h
        mcge/rendering/renderer_layers.c
        mcge/rendering/textures.c
        mcge/scene/prefabs/prefabs.h
        mcge/scene/prefabs/prefabs.c
        mcge/modules/physics/collision_layer.h
        mcge/modules/physics/collision_layers.h
        mcge/modules/physics/collision_response.h
        mcge/modules/physics/collision_rules.h
        mcge/modules/physics/collision_layer.c
        mcge/modules/physics/collision_layers.c
        mcge/modules/physics/collision_response.c
        mcge/modules/physics/collision_rules.c
        mcge/utils/uint128_t.h
        mcge/utils/uint128_t.c
        mcge/utils/pointer_hasher.h
        mcge/utils/pointer_hasher.c
        mcge/scene/entity_ids/entity_ids.h
        mcge/scene/entity_ids/entity_ids.c
        mcge/utils/uint128_dictionary.h
        mcge/utils/uint128_dictionary.c
        mcge/scene/component_ids/component_ids.h
        mcge/scene/component_ids/component_ids.c
)
set_target_properties(mcge PROPERTIES DEFINE_SYMBOL MCGE_EXPORT)
if(WIN32)
    set_target_properties(mcge PROPERTIES PREFIX "")
endif()
target_include_directories(mcge PUBLIC .)
target_link_libraries(mcge PUBLIC SDL3::SDL3 SDL3_image::SDL3_image)

add_executable(MyCoolGameEngine demo/src/main.c
        demo/src/world/forest.h
        demo/src/world/forest.c
        demo/src/special/pulsator.h
        demo/src/special/pulsator.c
        demo/src/scenes/scene_switcher.h
        demo/src/scenes/scene_switcher.c
        demo/src/scenes/game_closer.h
        demo/src/scenes/game_closer.c
        demo/src/environment/clock.h
        demo/src/environment/clock.c
        demo/src/environment/sky.h
        demo/src/environment/sky.c
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
        demo/src/skills/skill.h
        demo/src/skills/skill.c
        demo/src/skills/skill_internal.h
        demo/src/skills/custom/printer.h
        demo/src/skills/custom/printer.c
        demo/src/skills/skilled.h
        demo/src/skills/skilled.c
        demo/src/skills/skill_factory.h
        demo/src/skills/skill_factory.c
        demo/src/skills/custom/fear_balls/fear_balls.h
        demo/src/skills/custom/fear_balls/fear_balls.c
        demo/src/skills/custom/fear_balls/fear_ball.h
        demo/src/skills/custom/fear_balls/fear_ball.c
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

if(WIN32)
    target_link_libraries(mcge PRIVATE bcrypt)
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error LANGUAGES C)
if(ipo_supported)
    set_property(TARGET mcge MyCoolGameEngine PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
else()
    message(WARNING "LTO is off: ${ipo_error}")
endif()