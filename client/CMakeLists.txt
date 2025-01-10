# Project: Client
add_executable(client client.c)

# Libs
target_link_libraries(client utils menu board)
