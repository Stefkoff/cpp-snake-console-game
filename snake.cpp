#include "snake.h"

#define SNAKE_DEFAULT_SIZE 9

/**
 * Generate a random number between 0 and the given maximum
 * @param max Maximum of the random number
 * @return
 */
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
    // generate the entire snake, based on the default size
    Snake* snake = createSnake(nullptr, pos, &i);
    return snake;
}

Snake* createSnake(Snake* parent, sVect pos, int* curCount) {
    // check if the count of the snake items is equals to the default size
    // if so, we are ready to return the entire snake and get out of the recursion
    // note that the `pos.x == 0`. This is just in case that the previous calculations
    // did not calculate the starting positions of the snake's head.
    // ideally this should now happen
    if(*curCount == SNAKE_DEFAULT_SIZE || pos.x == 0) {
        return parent;
    }

    // no parent - we are at the heade
    if(parent == nullptr) {
        parent = static_cast<Snake *>(malloc(sizeof(Snake)));
        parent->pos = pos;
        parent->direction = RIGHT;
    }
    // create the next child
    Snake *child = static_cast<Snake *>(malloc(sizeof(Snake)));
    sVect post;
    // assign the position of the child with X equals to the parent X - 1
    // Note that, since this is the generation of the snake,
    // and the snake will be moving right on the X axis by default.
    post.x = pos.x - 1;
    post.y = pos.y;
    child->pos = post;
    child->direction = parent->direction;
    (*curCount)++;

    // create the next child if it needs to
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
        // move the next child with the position of its parent
        moveSnake(parent->next, oldPos);
    }
}


int isAtPos(Snake* snake, sVect pos) {
    // first check if the snake is at the exact position
    if(snake->pos.x == pos.x && snake->pos.y == pos.y) {
        return 1;
    }

    // snake (head) is not at the position,
    // check on of the entire snake is at the position
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
    // if there is a child, we have to free child's
    // resources first and then remove the parent
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
    // check if the new dot does not overlap with the entire snake
    if(isAtPos(snake, pos)) {
        return generateDot(snake, maxX, maxY);
    }

    return pos;
}
