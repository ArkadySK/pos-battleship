# Project: Server
add_executable(server server.c)

# Libs
target_link_libraries(server PUBLIC ipc game_logic utils)
