# POS Battleship
Authors: Adam Majerík, Tomáš Karnet

## Information
This is an implementation of the game BattleShip for the class Princípy Operačných Systémov, for FRI UNIZA, year 2024/2025.

The game board is displayed in the following way for each player:
```
   A B C D E F G G I J
 1 o o o o o o o o o o
 2 x o o o o o o o o o
 3 . . . . . . . . . .
 4 o o o o o o o o o o
 5 o o . . . . . . o o
 6 . . . x x x x o o o
 7 x . o . o . o o o .
 8 x . o . o . o . . .
 9 . . . . . . . . . .
10 . . . . . . . . . .
```

o - Hit water

x - Hit BattleShip

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