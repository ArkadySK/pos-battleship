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
    return((coords[0] >= '0') && (coords[0] <= '9') && (coords[1] >= 'A') && (coords[1] <= 'J'));
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
        if (y + size >= 10) return false;

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
        if (x < 9)
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
        if ((y + size < 9) && (b->board_[x][y + size] == SHIP)
            && (b->board_[x - 1][y + size] == SHIP)
            && (b->board_[x + 1][y + size] == SHIP))
            return false;
    }
    else
    {
        // Check if ship is out of bounds
        if (x + size >= 10) return false;
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
        if ((x + size < 10) && (b->board_[x + size][y] == SHIP)
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
        if (y < 9)
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
        if (fgets(position, sizeof(char) * 4, stdin) == NULL)
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

int shoot(board* b, int x, int y)
{
    // TODO: Waffle
}