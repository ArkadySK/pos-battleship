#define MSG_WAIT_PLAYER 'W'    // Wait for other player
#define MSG_START_GAME 'S'     // Start the game
#define MSG_YOUR_TURN 'T'      // Your turn
#define MSG_SHOT 'H'           // Shot coordinates
#define MSG_RESULT 'R'         // Shot result
#define MSG_GAME_OVER 'G'      // Game over
#define MSG_SINGLE_PLAYER 'B'  // Single player mode
#define MSG_MULTI_PLAYER 'M'   // Multi player mode

typedef struct {
    char type;          // Message type
    int x;             // X coordinate
    int y;             // Y coordinate
    int hit;           // 1 if hit, 0 if miss, -1 if game over
} Message;