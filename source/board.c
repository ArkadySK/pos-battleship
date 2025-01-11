#include "board.h"

void board_init(board* b, int size)
{
    b->board_ = malloc(size * sizeof(char*));
    for (int i = 0; i < size; i++)
    {
        b->board_[i] = calloc(size, sizeof(char));
        for (int j = 0; j < size; j++)
        {
            b->board_[i][j] = NOT_HIT;
        } 
    }

    b->size_ = size;
    b->destroyed_ = 0;
}

void board_destroy(board* b)
{
    for (int i = 0; i < b->size_; i++)
    {
        free(b->board_[i]);
    }
    free(b->board_);    
}

void board_display(board* b_own, board* b_enemy)
{
    //clear_screen();
    printf("       Your board      |      Enemy board\n");
    printf("   A B C D E F G H I J |   A B C D E F G H I J\n");
    for (int j = 0; j < b_own->size_; j++)
    {
        printf("%2d ", j);
        for (int i = 0; i < b_own->size_; i++)
        {
            printf("%c ", b_own->board_[i][j]);
        }
        printf("|%2d ", j);
        for (int i = 0; i < b_enemy->size_; i++)
        {
            if (b_enemy->board_[i][j] == SHIP)
            {
                printf("%c ", NOT_HIT);
            }
            else
            {
                printf("%c ", b_enemy->board_[i][j]);
            }
        }
        printf("\n");
    }
}

bool validate_coords(char* coords)
{
    return((coords[0] >= 'A') && (coords[0] <= 'J') && (coords[1] >= '0') && (coords[1] <= '9'));
}

bool validate_rotation(char rotation)
{
    return((rotation == DOWN) || (rotation == RIGHT));
}

bool validate_tile(int x, int y, board* b)
{
    if ((x < 0 || x >= b->size_) || (y < 0 || y >= b->size_))
    {
        return true;
    }
    return (!(b->board_[x][y] == SHIP));
}

bool validate_position(char* position, int size, board* b)
{
    int x = position[0] - 'A';
    int y = position[1] - '0';
    bool down = true;
    if (position[2] == 'r') down = false;
    if (down)
    {
        if (y + size > b->size_) return false;
        for (int i = x - 1; i <= x + 1; i++)
        {
            for(int j = y - 1; j <= y + size + 1; j++)
            {
                if (!validate_tile(i, j, b)) return false;
            }
        }
    }
    else
    {
        if (x + size > b->size_) return false;
        for(int i = x - 1; i <= x + size + 1; i++)
        {
            for (int j = y - 1; j <= y + 1; j++)
            {
                if (!validate_tile(i, j, b)) return false;
            }
        }
    }
    return true;
}

void get_ship(char* position, int size, board* b)
{
    while (true)
    {
        char* result = fgets(position, 4 * sizeof(char), stdin);
        while (getchar() != '\n');
        if (result == NULL)
        {
            printf("An error occured, try again\n");
            continue;
        }
        if (strrchr(position, '\n'))
        {
            printf("Not enough characters to place ship, try again\n");
            continue;
        }
        if (!(validate_coords(position) && validate_rotation(position[2])))
        {
            printf("Input not valid, try again\n");
            continue;
        }
        if (!validate_position(position, size, b))
        {
            printf("Ship out of bounds, or conflicting with other ships, try again\n");
            continue;
        }
        break;
    }
    printf("Placement accepted!");
}

void finalise_placement(char* position, int size, board* b)
{
    int x = position[0] - 'A';
    int y = position[1] - '0';
    bool down = true;
    if (position[2] == 'r') down = false;
    if (down)
    {
        for (int i = y; i < y + size ; i++)
        {
            b->board_[x][i] = SHIP;
        }
    }
    else
    {
       for (int i = x; i < x + size ; i++)
        {
            b->board_[i][y] = SHIP;
        } 
    }
}

void place_ships(board* b)
{
    printf("-------------INFO--------------\n");
    printf("Ship coordinates are given as: \"A2d\" \n");
    printf("X coordinate: A-J\n");
    printf("Y coordinate: 0-9\n");
    printf("Rotation: d = down, r = right\n");
    printf("Ships cannot be adjacent to other ships\n");
    printf("-------------------------------\n");
    printf("Press any key to continue...\n");
    getchar();
    while (getchar() != '\n');
    board_display(b, b);
    char* position = calloc(4, sizeof(char));
    printf("Where do you want to place the 5 tile ship?\n");
    get_ship(position, 5, b);
    finalise_placement(position, 5, b);
    board_display(b, b);
    printf("Where do you want to place the 4 tile ship?\n");
    get_ship(position, 4, b);
    finalise_placement(position, 4, b);
    board_display(b, b);
    printf("Where do you want to place the first 3 tile ship?\n");
    get_ship(position, 3, b);
    finalise_placement(position, 3, b);
    board_display(b, b);
    printf("Where do you want to place the second 3 tile ship?\n");
    get_ship(position, 3, b);
    finalise_placement(position, 3, b);
    board_display(b, b);
    printf("Where do you want to place the 2 tile ship?\n");
    get_ship(position, 2, b);
    finalise_placement(position, 2, b);
    board_display(b, b);
    printf("All ships successfully placed\n");
    free(position);
}

bool check_destroyed(int x, int y, board* b)
{
    int checking;
    if (y > 0) {
        checking = y - 1;
        do
        {
            if (b->board_[x][checking] == SHIP) return false;
            checking -= 1;
        } while ((checking >= 0) && (b->board_[x][checking] == HIT_SHIP));
    }
    if (y < b->size_ - 1)
    {
        checking = y + 1;
        do
        {
            if (b->board_[x][checking] == SHIP) return false;
            checking += 1;
        } while ((checking < b->size_) && (b->board_[x][checking] == HIT_SHIP));
    }
    if (x > 0) {
        checking = x - 1;
        do
        {
            if (b->board_[checking][y] == SHIP) return false;
            checking -= 1;
        } while ((checking >= 0) && (b->board_[checking][y] == HIT_SHIP));
    }
    if (x < b->size_ - 1)
    {
        checking = x + 1;
        do
        {
            if (b->board_[checking][y] == SHIP) return false;
            checking += 1;
        } while ((checking < b->size_) && (b->board_[checking][y] == HIT_SHIP));
    }
    return true;
}

// Output will probably be sent to server
int receive_shot(int x, int y, board* b)
{
    if (b->board_[x][y] == SHIP)
    {
        b->board_[x][y] = HIT_SHIP;
        if (check_destroyed(x, y, b))
        {
            b->destroyed_++;
            if (b->destroyed_ == 5)
            {
                return -1; // Game over
            }
        }
        return 1;
    }
    b->board_[x][y] = HIT_WATER;
    return 0;    
}

void get_shot(char* shot, board* b_enemy)
{
    while (true)
    {
        char* result = fgets(shot, sizeof(char) * 3, stdin);
        while (getchar() != '\n');
        if (result == NULL) {
            printf("An error occured, try again\n");
            continue;
        }
        if (strrchr(shot, '\n'))
        {
            printf("Not enough characters determine shot, try again\n");
            continue;
        }
        if (!validate_coords(shot))
        {
            printf("Input not valid, try again\n");
            continue;
        }
        int x = shot[0] - 'A';
        int y = shot[1] - '0';
        if(b_enemy->board_[x][y] != NOT_HIT)
        {
            printf("You have already shot there, try again\n");
            continue;
        }
        break;
    }
    printf("Shot accepted!\n");
}

// The output of this is probably just gonna get sent to server
char* shoot(board* b_enemy)
{
    printf("Shots are given as \"A2\"\n");
    printf("X coordinate: A-J\n");
    printf("Y coordinate: 0-9\n");
    printf("Where do you want to shoot?\n");
    char* shot = calloc(3, sizeof(char));
    get_shot(shot, b_enemy);
    return shot;
}

// Adam, dont forget to save the last shot before you send it to server, you'll need it for this
void mark_hit(int x, int y, int hit, board* b_enemy)
{
    if (hit == 1)
    {
        b_enemy->board_[x][y] = HIT_SHIP;
        return;
    }
    b_enemy->board_[x][y] = HIT_WATER;
}