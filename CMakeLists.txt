cmake_minimum_required(VERSION 3.22)
project(Battleship VERSION 0.1.0 LANGUAGES C)

# Set C17 standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Libs
#add_library(game_logic STATIC source/game_logic.c)
add_library(utils STATIC source/utils.c)
add_library(menu STATIC source/menu.c)
add_library(board STATIC source/board.c)

# Projects
add_subdirectory(client)
add_subdirectory(server)

