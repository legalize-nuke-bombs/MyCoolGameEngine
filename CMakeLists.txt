cmake_minimum_required(VERSION 4.3)
project(MyCoolGameEngine C)

set(CMAKE_C_STANDARD 11)

add_executable(MyCoolGameEngine main.c
        src/core/Entity.h
        src/core/Entity.c
)
