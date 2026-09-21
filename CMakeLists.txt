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
)
