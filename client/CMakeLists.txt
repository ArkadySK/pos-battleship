# Project: Client
add_executable(client client.c)

# Libs
target_link_libraries(client PUBLIC ipc game_logic utils)
