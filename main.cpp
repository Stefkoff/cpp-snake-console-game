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

    tcgetattr( fileno( stdin ), &oldSettings );
    newSettings = oldSettings;
    newSettings.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr( fileno( stdin ), TCSANOW, &newSettings );

    fd_set set;
    struct timeval tv{};

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    FD_ZERO( &set );
    FD_SET( fileno( stdin ), &set );

    int res = select( fileno( stdin )+1, &set, nullptr, nullptr, &tv );


    if( res > 0 )
    {
        read( fileno( stdin ), &c, 1 );
    }
    else if( res < 0 )
    {
        return 0;
    }

    tcsetattr( fileno( stdin ), TCSANOW, &oldSettings );
    return c;
}

int main() {
    printf("Hello, World!\n");

    Snake* snake = initSnake(MAX_X, MAX_Y);

    while(1) {
        system("clear");
        for(int y = 0; y < MAX_Y; y++) {
            for(int x = 0; x < MAX_X; x++) {
                sVect pos;
                pos.x = x;
                pos.y = y;

                if(isAtPos(snake, pos) == 1) {
                    printf("*");
                } else {
                    printf("-");
                }
            }
            printf("\n");
        }

        char c = wait_for_input();
        sVect pos = snake->pos;
        if(c) {
            printf("Pressend Kye: %d\n", c);
            switch(c) {
                case (int) 97:
                    // move left
                   pos.y = snake->pos.y - 1;
                   break;
                case (int) 100:
                    // move right
                    pos.y = snake->pos.y + 1;
                    break;
                case (int) 119:
                    // move up
                    pos.x = snake->pos.x - 1;
                    break;
                case (int) 115:
                    // move down
                    pos.x = snake->pos.x + 1;
                    break;
            }
            moveSnake(snake, pos);
        }
        if(snake->pos.x == MAX_X  || snake->pos.y == MAX_Y ) {
            printf("Game over");
            break;
        }
        sleep(1);
    }

    return 0;
}