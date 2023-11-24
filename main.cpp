#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include "snake.h"
#include <termio.h>

#define MAX_X 50
#define MAX_Y 13

char wait_for_input() {
    char c = 0;
    struct termios oldSettings{}, newSettings{};

    tcgetattr(fileno(stdin), &oldSettings);
    newSettings = oldSettings;
    newSettings.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(fileno(stdin), TCSANOW, &newSettings);

    fd_set set;
    struct timeval tv{};

    tv.tv_sec = 0;
    tv.tv_usec = 100000;

    FD_ZERO(&set);
    FD_SET(fileno(stdin), &set);

    int res = select(fileno(stdin) + 1, &set, nullptr, nullptr, &tv);


    if (res > 0) {
        read(fileno(stdin), &c, 1);
    } else if (res < 0) {
        return 0;
    }

    tcsetattr(fileno(stdin), TCSANOW, &oldSettings);
    return c;
}

sVect generateDot(Snake* snake) {

    // not need to srand, because it was already seeded at the initSnake function
    int rX = (rand() % (MAX_X - 1));
    int rY = (rand() % (MAX_Y - 1));

    sVect pos;
    pos.x = rX;
    pos.y = rY;
    if(isAtPos(snake, pos)) {
        return generateDot(snake);
    }

    return pos;
}

int main() {
    Snake *snake = initSnake(MAX_X, MAX_Y);

    sVect dot = generateDot(snake);

    while (1) {
        system("clear");

        for (int y = 0; y < MAX_Y; y++) {
            for (int x = 0; x < MAX_X; x++) {
                sVect pos;
                pos.x = x;
                pos.y = y;

                if (isAtPos(snake, pos) == 1) {
                    printf("*");
                } else {
                    if(dot.x == x && dot.y == y){
                        printf("#");
                    } else {
                        printf("-");
                    }
                }
            }
            printf("\n");
        }
        char c = wait_for_input();
        sVect newPos = snake->pos;
        if (c) {
            switch (c) {
                case (int) 97:
                    if (snake->direction == RIGHT) {
                        break;
                    }
                    // move left
                    newPos.x--;
                    snake->direction = LEFT;
                    break;
                case (int) 100:
                    if (snake->direction == LEFT) {
                        break;
                    }
                    // move right
                    newPos.x++;
                    snake->direction = RIGHT;
                    break;
                case (int) 119:
                    if (snake->direction == DOWN) {
                        break;
                    }
                    // move up
                    newPos.y--;
                    snake->direction = UP;
                    break;
                case (int) 115:
                    if (snake->direction == UP) {
                        break;
                    }
                    // move down
                    newPos.y++;
                    snake->direction = DOWN;
                    break;
            }
        } else {
            switch (snake->direction) {
                case UP:
                    newPos.y--;
                    break;
                case DOWN:
                    newPos.y++;
                    break;
                case LEFT:
                    newPos.x--;
                    break;
                case RIGHT:
                    newPos.x++;
                    break;
            }
        }
        // make it look to the entire scree
        if(newPos.x < 0) {
            newPos.x = MAX_X - 1;
        } else if(newPos.x >= MAX_X) {
            newPos.x = 0;
        }

        if(newPos.y < 0) {
            newPos.y = MAX_Y - 1;
        } else if(newPos.y >= MAX_Y) {
            newPos.y = 0;
        }
        moveSnake(snake, newPos);


        if(checkBite(snake)) {
            printf("Game Over!");
            break;
        }
        if(isAtPos(snake, dot) == 1) {
            snake = incr(snake);
            dot = generateDot(snake);
        }

        usleep(400000);
    }

    freeSnake(snake);

    return 0;
}