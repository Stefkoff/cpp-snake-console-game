#include "snake.h"

#define SNAKE_DEFAULT_SIZE 9

Snake *initSnake(int maxX, int maxY) {
    srand(time(0));

    int rX = (rand() % (maxX + 1));
    int rY = (rand() % (maxY + 1));

    sVect pos;
    pos.x = rX;
    pos.y = rY;
    int i = 0;
    Snake* snake = createSnake(nullptr, pos, &i);
    return snake;
}

Snake* createSnake(Snake* parent, sVect pos, int* curCount) {
    if(*curCount == SNAKE_DEFAULT_SIZE || pos.x == 0) {
        return parent;
    }

    if(parent == nullptr) {
        parent = static_cast<Snake *>(malloc(sizeof(Snake)));
        parent->pos = pos;
        parent->direction = RIGHT;
    }
    Snake *child = static_cast<Snake *>(malloc(sizeof(Snake)));
    sVect post;
    post.x = pos.x - 1;
    post.y = pos.y;
    child->pos = post;
    child->direction = parent->direction;
    (*curCount)++;
    parent->next = createSnake(child, child->pos, curCount);

    return parent;
}


void moveSnake(Snake* parent, sVect pos) {
    /**
     * Here we have to replace each child with the parent position
     */
     sVect oldPos = parent->pos;
     parent->pos = pos;

    if(parent->next != nullptr) {
        moveSnake(parent->next, oldPos);
    }
}


int isAtPos(Snake* snake, sVect pos) {
    if(snake->pos.x == pos.x && snake->pos.y == pos.y) {
        return 1;
    }
    if(snake->next != nullptr && isAtPos(snake->next, pos)){
        return 1;
    }

    return 0;
}

bool checkBite(Snake* snake) {
    if(isAtPos(snake->next, snake->pos) == 1) {
        return true;
    }

    return false;
}

Snake* incr(Snake* snake) {
    auto* newSnake = static_cast<Snake *>(malloc(sizeof(snake)));
    newSnake->next = snake;
    newSnake->pos = snake->pos;
    newSnake->direction = snake->direction;
    moveSnake(snake->next, newSnake->pos);

    return newSnake;
}