#include "snake.h"

#define SNAKE_DEFAULT_SIZE 9

int generateRandomNumber(int max) {
    return rand() % (max - 1);
}

Snake *initSnake(int maxX, int maxY) {
    srand(time(0));

    // make sure that the snake + SNAKE_DEFAULT_SIZE does not go under 0
    // because we will see a smaller snake at start
    int rX = 0;
    do {
        rX = generateRandomNumber(maxX);
    } while((rX - SNAKE_DEFAULT_SIZE) <= 0);
    int rY = 0;
    do {
        rY = generateRandomNumber(maxY);
    } while((rY + SNAKE_DEFAULT_SIZE) < maxY - 1);

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

void freeSnake(Snake* snake) {
    if(snake->next) {
        freeSnake(snake->next);
    }

    delete(snake);
}

sVect generateDot(Snake* snake, int maxX, int maxY) {

    // not need to srand, because it was already seeded at the initSnake function
    int rX = generateRandomNumber(maxX);
    int rY = generateRandomNumber(maxY);

    sVect pos;
    pos.x = rX;
    pos.y = rY;
    if(isAtPos(snake, pos)) {
        return generateDot(snake, maxX, maxY);
    }

    return pos;
}
