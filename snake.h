//
// Created by stefkoff on 11/22/2023.
//

#ifndef SNAKE_GAME_SNAKE_H
#define SNAKE_GAME_SNAKE_H
//
// Created by stefkoff on 22.11.23.
//
#include <cstdlib>
#include <ctime>

enum SnakeDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

typedef struct {
    int x;
    int y;
} sVect; // position vector. TODO: maybe a better name will be good

typedef struct S_Snake {
    sVect pos;
    SnakeDirection direction;
    struct S_Snake* next;
} Snake; // snake structure - it is like a queue

/**
 * This function will initialize the snake, based on the given default coordinates
 * @param maxX - the maximum X coordinate
 * @param maxY - the maximum Y coordinate
 * @return - the snake
 */
Snake* initSnake(int, int);
Snake* createSnake(Snake*, sVect, int*);
int isAtPos(Snake*, sVect);
void moveSnake(Snake*, sVect);
bool checkBite(Snake*);
Snake* incr(Snake*);
void freeSnake(Snake* snake);
int generateRandomNumber(int);
sVect generateDot(Snake*, int, int);

#endif //SNAKE_GAME_SNAKE_H
