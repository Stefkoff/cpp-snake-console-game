#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include "snake.h"
#include <termio.h>

#define MAX_X 50 // default size of the X-axis
#define MAX_Y 13 // default size of the Y-axis

/**
 * Wait for the user to press any key
 * @return keycode of the user input
 */
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

int main() {
    // generate the snake with some defaults
    Snake *snake = initSnake(MAX_X, MAX_Y);

    // generate the dot
    sVect dot = generateDot(snake, MAX_X, MAX_Y);

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
        /**
         * Wait for the user to press any of the characters:
         *
         *      w
         * a    s    d
         *
         * we will wait just for 100ms, and if the user
         * pressed any key, we will calculate the next
         * movement of the snake
         */
        char c = wait_for_input();
        sVect newPos = snake->pos;
        if (c) {
            switch (c) {
                case (int) 97:
                    // going left
                    // check first if we are going on the right
                    // if so, then nothing, because the snake
                    // cannot go to the opposite direction
                    if (snake->direction == RIGHT) {
                        break;
                    }
                    // move left
                    newPos.x--;
                    snake->direction = LEFT;
                    break;
                case (int) 100:
                    // going right
                    // check first if we are going on the left
                    // if so, then nothing, because the snake
                    // cannot go to the opposite direction
                    if (snake->direction == LEFT) {
                        break;
                    }
                    // move right
                    newPos.x++;
                    snake->direction = RIGHT;
                    break;
                case (int) 119:
                    // going up
                    // check first if we are going on the down
                    // if so, then nothing, because the snake
                    // cannot go to the opposite direction
                    if (snake->direction == DOWN) {
                        break;
                    }
                    // move up
                    newPos.y--;
                    snake->direction = UP;
                    break;
                case (int) 115:
                    // going down
                    // check first if we are going on the up
                    // if so, then nothing, because the snake
                    // cannot go to the opposite direction
                    if (snake->direction == UP) {
                        break;
                    }
                    // move down
                    newPos.y++;
                    snake->direction = DOWN;
                    break;
                default:
                    // nothing to do here, since everything will be handled below
            }
        } else {
            // auto move the snake by one slot,
            // depending on the direction
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

        // if the snake reached the end of the map,
        // then move it at the start to the opposite
        // direction of the movement
        if(newPos.y < 0) {
            newPos.y = MAX_Y - 1;
        } else if(newPos.y >= MAX_Y) {
            newPos.y = 0;
        }

        // calculate the new position of the movement
        moveSnake(snake, newPos);


        // check if the snake bite itself
        if(checkBite(snake)) {
            printf("Game Over!");
            break;
        }

        // check if the snake ate the dot
        // if so, then generate a new dot
        // and also increment the snake by one
        if(isAtPos(snake, dot) == 1) {
            snake = incr(snake);
            dot = generateDot(snake, MAX_X, MAX_Y);
        }

        usleep(400000);
    }

    // release the resources
    freeSnake(snake);

    return 0;
}