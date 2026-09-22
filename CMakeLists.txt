cmake_minimum_required(VERSION 4.3)
project(MyCoolGameEngine C)

set(CMAKE_C_STANDARD 11)

add_executable(MyCoolGameEngine main.c
        src/core/entity.h
        src/core/entity.c
        src/utils/vector2.h
        src/utils/vector2.c
        src/utils/vector2_math.h
        src/utils/vector2_math.c
        src/core/components/component.h
        src/core/components/component.c
        src/logging/logger.h
        src/logging/logger.c
        src/core/components/custom/printer.h
        src/core/components/custom/printer.c
        src/core/update_context.h
        src/core/entity_collection.h
        src/core/entity_collection.c
        src/core/scene.h
        src/core/scene.c
        version.h
        src/core/components/custom/transform.h
        src/core/components/custom/transform.c
)
