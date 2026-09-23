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
        src/scene/components/custom/printer.h
        src/scene/components/custom/printer.c
        src/scene/update_context.h
        src/scene/scene.h
        src/scene/scene.c
        version.h
        src/scene/components/transform.h
        src/scene/components/transform.c
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
        src/scene/components/custom/camera.h
        src/scene/components/custom/camera.c
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
        src/engine/engine_context.h
        src/scene/components/custom/box_renderer.h
        src/scene/components/custom/box_renderer.c
        src/utils/color.h
        src/utils/colors.h
        src/utils/colors.c
        src/rendering/renderer_layers.h
        src/rendering/renderer_layers.c
)

target_link_libraries(MyCoolGameEngine PRIVATE SDL3::SDL3)
