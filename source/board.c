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
    printf("       Your board      |      Enemy board\n");
    printf("   A B C D E F G H I J |   A B C D E F G H I J\n");
    for (int i = 0; i < b_own->size_; i++)
    {
        printf("%2d ", i);
        for (int j = 0; j < b_own->size_; j++)
        {
            printf("%c ", b_own->board_[i][j]);
        }
        printf("|%2d ", i);
        for (int j = 0; j < b_enemy->size_; j++)
        {
            if (b_enemy->board_[i][j] = SHIP)
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

bool validate_position(char* position, int size, board* b)
{
    int x = position[0] - 'A';
    int y = position[1] - '0';
    bool down = true;
    if (position[2] == 'r') down = false;
    if (down)
    {
        // Check if ship is out of bounds
        if (y + size >= b->size_) return false;

        // Check if the tiles where the ship will be placed are occupied by another ship
        for (int i = y; i < y + size; i++)
        {
            if (b->board_[x][i] == SHIP) return false;
        }
        // Check if there are taken tiles to the left of the ship, if any
        if (x > 0)
        {
            for (int i = y; i < y + size; i++)
            {
                if (b->board_[x - 1][i] == SHIP) return false;
            }
        }
        // Check if there are taken tiles to the right of the ship, if any
        if (x < b->size_ - 1)
        {
            for (int i = y; i < y + size; i++)
            {
                if (b->board_[x + 1][i] == SHIP) return false;
            }
        }
        // Check if there are taken tiles above and diagonally above the ship, if any
        if ((y > 0) && (b->board_[x][y - 1] == SHIP)
            && (b->board_[x - 1][y - 1] == SHIP)
            && (b->board_[x + 1][y - 1] == SHIP))
            return false;
        // Check if there are taken tiles above and diagonally below the ship, if any
        if ((y + size < b->size_ - 1) && (b->board_[x][y + size] == SHIP)
            && (b->board_[x - 1][y + size] == SHIP)
            && (b->board_[x + 1][y + size] == SHIP))
            return false;
    }
    else
    {
        // Check if ship is out of bounds
        if (x + size >= b->size_) return false;
        // Check if the tiles where the ship will be placed are occupied by another ship
        for (int i = x; i < x + size; i++)
        {
            if (b->board_[i][y] == SHIP) return false;
        }
        // Check if there are taken tiles right and diagonally right of the ship, if any
        if ((x > 0) && (b->board_[x - 1][y] == SHIP)
            && (b->board_[x - 1][y - 1] == SHIP)
            && (b->board_[x - 1][y + 1] == SHIP))
            return false;
        // Check if there are taken tiles left and diagonally left of the ship, if any
        if ((x + size < b->size_ - 1) && (b->board_[x + size][y] == SHIP)
            && (b->board_[x + size][y - 1] == SHIP)
            && (b->board_[x + size][y + 1] == SHIP))
            return false;
        // Check if there are taken tiles above the ship, if any
        if (y > 0)
        {
            for (int i = x; i < x + size; i++)
            {
                if (b->board_[i][y = 1] == SHIP) return false;
            }
        }
        // Check if there are taken tiles below the ship, if any
        if (y < b->size_ - 1)
        {
            for (int i = x; i < x + size; i++)
            {
                if (b->board_[i][y + 1] == SHIP) return false;
            }
        }
    }
    return true;
}

void get_ship(char* position, int size, board* b)
{
    bool accepted = false;
    while (!accepted)
    {
        int c;
        while ((c = getchar() != '\n') && c != EOF);
        if (fgets(position, sizeof(char) * 3, stdin) == NULL)
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
    printf("Ship coordinates are given as: \"A2d\" ");
    printf("X coordinate: A-J\n");
    printf("Y coordinate: 0-9\n");
    printf("Rotation: d = down, r = right");
    printf("Ships cannot be adjacent to other ships\n");
    board_display(b, b);
    char position[4];
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
    printf("All ships successfully placed");
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
bool receive_shot(int x, int y, board* b)
{
    if (b->board_[x][y] == SHIP)
    {
        b->board_[x][y] = HIT_SHIP;
        if (check_destroyed(x, y, b))
        {
            b->destroyed_++;
            if (b->destroyed_ == 5)
            {
                // Probably gonna want to send the server an "im dead" message
            }
        }
        return true;
    }
    b->board_[x][y] = HIT_WATER;
    return false;    
}

void get_shot(char* shot, board* b_enemy)
{
    bool accepted = false;
    while (!accepted)
    {
        int c;
        while ((c = getchar() != '\n') && c != EOF);
        if (fgets(shot, sizeof(char) * 2, stdin) == NULL) {
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
        }
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
    char* shot[3];
    get_shot(shot, b_enemy);
    return shot;
}