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
    printf("All ships successfully placed");
    free(position);
}

int shoot(board* b, int x, int y)
{
    // TODO: Waffle
}

int main() {
    //TODO Adam: testing purposes
    board b;
    board_init(&b, 10);
    place_ships(&b);
}