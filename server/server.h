#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include "../source/message.h"
#include "bot_board.h"

#define MAX_PLAYERS 2
#define PLAYER_TYPE_HUMAN 0
#define PLAYER_TYPE_BOT 1

typedef struct {
    board b_own;
    board b_enemy;
} BotState;

typedef struct {
    int socket;
    int opponent_socket;
    bool has_opponent;
    int player_type;
    BotState* bot_state;
} Player;

int find_player_slot(int socket);

int find_empty_slot();

int initialize_server(int port);

void* handle_client(void* arg);