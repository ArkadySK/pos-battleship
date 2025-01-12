#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "../source/board.h"
#include "../source/menu.h"
#include "../source/utils.h"
#include "../source/message.h"

#define SERVER_IP "127.0.0.1" // localhost

int initialize_client(int port);

void play_game(int sock);