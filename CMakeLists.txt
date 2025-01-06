cmake_minimum_required(VERSION 3.22)
project(Battleship VERSION 0.1.0 LANGUAGES C)

# Binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Libs
add_library(ipc STATIC source/ipc.c)
add_library(game_logic STATIC source/game_logic.c)
add_library(utils STATIC source/utils.c)

# Projects
add_subdirectory(client)
add_subdirectory(server)

