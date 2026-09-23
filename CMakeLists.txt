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
        src/core/entity.h
        src/core/entity.c
        src/utils/vector2.h
        src/utils/vector2.c
        src/utils/vector2_math.h
        src/utils/vector2_math.c
        src/core/components/component.h
        src/core/components/component.c
        src/core/components/component_internal.h
        src/logging/logger.h
        src/logging/logger.c
        src/core/components/custom/printer.h
        src/core/components/custom/printer.c
        src/core/update_context.h
        src/core/scene.h
        src/core/scene.c
        version.h
        src/core/components/transform/transform.h
        src/core/components/transform/transform.c
        src/utils/list.h
        src/utils/list.c
        src/utils/dictionary.h
        src/utils/dictionary.c
        src/utils/string_dictionary.h
        src/utils/string_hasher.h
        src/utils/string_hasher.c
        src/utils/string_dictionary.c
        src/core/tmap.h
        src/core/tmap.c
        src/utils/action.h
        src/utils/action.c
        src/core/components/rendering/camera.h
        src/core/components/rendering/camera.c
        src/core/entity_collection.h
        src/core/entity_collection.c
        example.c
        src/rendering/renderer_pipeline.h
        src/rendering/renderer_pipeline.c
        src/utils/rect.h
        src/rendering/rendering_primitive.h
        src/rendering/rendering_layer.h
)

target_link_libraries(MyCoolGameEngine PRIVATE SDL3::SDL3)
