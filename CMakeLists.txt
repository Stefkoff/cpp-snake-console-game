cmake_minimum_required(VERSION 3.26)
project(snake_game)

set(CMAKE_CXX_STANDARD 17)

add_executable(snake_game main.cpp
        snake.cpp
        snake.h)
