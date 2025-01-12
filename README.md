# POS Battleship
Authors: Adam Majerík, Tomáš Karnet

## Information
This is an implementation of the game BattleShip for the class Princípy Operačných Systémov, for FRI UNIZA, year 2024/2025.

The game board is displayed in the following way for each player:
```
   A B C D E F G H I J
 0 o o o o o o o o o o
 1 X o o o o o o o o o
 2 # . . . . . . . . .
 3 o o o o o o o o o o
 4 o o . . . . . . o o
 5 . . . X X X X o o o
 6 X . o . o . o o o .
 7 X . o . o . o # # #
 8 # . . . . . . . . .
 9 . . . . . # # # # #
```

\# - Players ship tile

o - Hit water

X - Hit BattleShip

. - Not yet hit



### How to run the code? 

1. Create a build folder
```
mkdir build
cd build
cmake ..
```

2. Launch a build process
```
cmake --build .
```

3. Start the program 
- client:
```
./bin/client
```
- server:
```
./bin/server
```
