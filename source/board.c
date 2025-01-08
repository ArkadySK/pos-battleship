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
        if (y + size >= 10) return false;
        for (int i = y; i < y + size; i++)
        {
            if (b->board_[x][i] == SHIP) return false;
        }

        if (x > 0)
        {
            for (int i = y; i < y + size; i++)
            {
                if (b->board_[x - 1][i] == SHIP) return false;
            }
        }

        if (x + size < 9)
        {
            for (int i = y; i < y + size; i++)
            {
                if (b->board_[x + 1][i] == SHIP) return false;
            }
        }

        if ((y > 0) && (b->board_[x][y - 1] == SHIP)
            && (b->board_[x - 1][y - 1] == SHIP)
            && (b->board_[x + 1][y - 1] == SHIP))
            return false;
        
        if ((y + size < 9) && (b->board_[x][y + size] == SHIP)
            && (b->board_[x - 1][y + size] == SHIP)
            && (b->board_[x + 1][y + size] == SHIP))
            return false;
    }
    else
    {
        if (x + size >= 10) return false;
        for (int i = x; i < x + size; i++)
        {
            if (b->board_[i][y] == SHIP) return false;
        }

        if ((x > 0) && (b->board_[x - 1][y] == SHIP)
            && (b->board_[x - 1][y - 1] == SHIP)
            && (b->board_[x - 1][y + 1] == SHIP))
            return false;
        
        if ((x + size < 10) && (b->board_[x + size][y] == SHIP)
            && (b->board_[x + size][y - 1] == SHIP)
            && (b->board_[x + size][y + 1] == SHIP))
            return false;
    }
    
}

void place_ships(board* b)
{
    printf("Ship coordinates are given as: \"A2d\" ");
    printf("X coordinate: A-J\n");
    printf("Y coordinate: 0-9\n");
    printf("Rotation: d = down, r = right");
    printf("Where do you want to place the 5 tile ship?\n");
    char line[4];
    bool accepted = false;
    while (!accepted)
    {
        int c;
        while ((c = getchar() != '\n') && c != EOF);
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("An error occured, try again\n");
            continue;
        }
        if (strrchr(line, '\n')) {
            printf("Not enough characters to place ship, try again\n");
            continue;
        }
        if ()
        {
            /* code */
        }
        
        
    }
    
    fgets(line, sizeof(line), stdin);
    
    // TODO: Waffle
}

int shoot(board* b, int x, int y)
{
    // TODO: Waffle
}