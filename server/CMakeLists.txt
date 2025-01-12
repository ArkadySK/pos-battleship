# Project: Server
add_executable(server server.c bot_board.c)
target_include_directories(server PRIVATE ../source)
target_link_libraries(server PRIVATE board)
target_link_libraries(server PRIVATE utils)