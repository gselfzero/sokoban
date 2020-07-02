#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <string.h>

#define WIN_WIDTH 79
#define WIN_HEIGHT 23
#define WIDTH 11
#define HEIGHT 8

enum eDirection {LEFT = 0, RIGHT, UP, DOWN} dir;

int playerX, playerY, finalX[7], finalY[7], destCount = 0, winCount = 7;

char map[8][11] = {
    
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' '},
    {'#', ' ', 'B', ' ', 'x', '#', 'x', '#', 'x', '#', ' '},
    {'#', ' ', ' ', '#', '#', '#', '@', '#', 'B', '#', ' '},
    {'#', ' ', '#', ' ', '#', ' ', 'B', ' ', ' ', '#', ' '},
    {'#', 'x', 'B', 'x', '#', 'B', ' ', ' ', '#', '#', ' '},
    {'#', ' ', 'B', ' ', 'B', ' ', ' ', ' ', '#', ' ', ' '},
    {'#', ' ', 'x', ' ', ' ', 'x', '#', '#', '#', ' ', ' '},
    {'#', '#', '#', '#', '#', '#', '#', ' ', ' ', ' ', ' '}
    
};

void printMap() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            const char tmpCh = map[i][j];
            mvprintw(i+LINES/2-WIDTH/2, j+COLS/2-HEIGHT/2, &tmpCh);
        }
	
    }
}

void setup() {
    int tmp = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if(map[i][j] == '@') {
                playerY = i;
                playerX = j;
            }
            if(map[i][j] == 'x') {
                finalY[tmp] = i;
                finalX[tmp] = j;
                tmp++;
            }
        }
    }
}

void rules() {
    switch (dir) {
        case LEFT:
            if(map[playerY][playerX-1] != '#') {
                if(map[playerY][playerX-1] == 'B' && map[playerY][playerX-2] != '#' && map[playerY][playerX-2] != 'B') {
                    map[playerY][playerX] = ' ';
                    map[playerY][playerX-1] = '@';
                    if(map[playerY][playerX-2] == 'x') {
                        destCount++;
                        map[playerY][playerX-2] = 'P';
                    } else {
                        map[playerY][playerX-2] = 'B';
                    }
                    playerX--;
                } else if((map[playerY][playerX-1] == ' ' || map[playerY][playerX-1] == 'x') && map[playerY][playerX-1] != 'B') {
                    map[playerY][playerX] = ' ';
                    map[playerY][playerX-1] = '@';
                    playerX--;
                }
            }
            break;
        case RIGHT:
            if(map[playerY][playerX+1] != '#') {
                if(map[playerY][playerX+1] == 'B' && map[playerY][playerX+2] != '#' && map[playerY][playerX+2] != 'B') {
                    map[playerY][playerX] = ' ';
                    map[playerY][playerX+1] = '@';
                    if(map[playerY][playerX+2] == 'x') {
                        destCount++;
                        map[playerY][playerX+2] = 'P';
                    } else {
                        map[playerY][playerX+2] = 'B';
                    }
                    playerX++;
                } else if((map[playerY][playerX+1] == ' ' || map[playerY][playerX+1] == 'x') && map[playerY][playerX+1] != 'B') {
                    map[playerY][playerX] = ' ';
                    map[playerY][playerX+1] = '@';
                    playerX++;
                }
            }
            break;
        case UP:
            if(map[playerY-1][playerX] != '#') {
                if(map[playerY-1][playerX] == 'B' && map[playerY-2][playerX] != '#' && map[playerY-2][playerX] != 'B') {
                    map[playerY][playerX] = ' ';
                    map[playerY-1][playerX] = '@';
                    if(map[playerY-2][playerX] == 'x') {
                        destCount++;
                        map[playerY-2][playerX] = 'P';
                    } else {
                        map[playerY-2][playerX] = 'B';
                    }
                    playerY--;
                } else if((map[playerY-1][playerX] == ' ' || map[playerY-1][playerX] == 'x') && map[playerY-1][playerX] != 'B') {
                    map[playerY][playerX] = ' ';
                    map[playerY-1][playerX] = '@';
                    playerY--;
                }
            }
            break;
        case DOWN:
            if(map[playerY+1][playerX] != '#') {
                if(map[playerY+1][playerX] == 'B' && map[playerY+2][playerX] != '#' && map[playerY+2][playerX] != 'B') {
                    if(map[playerY-2][playerX] == 'x') destCount++;
                    map[playerY][playerX] = ' ';
                    map[playerY+1][playerX] = '@';
                    if(map[playerY+2][playerX] == 'x') {
                        destCount++;
                        map[playerY+2][playerX] = 'P';
                    } else {
                        map[playerY+2][playerX] = 'B';
                    }
                    playerY++;
                } else if((map[playerY+1][playerX] == ' ' || map[playerY+1][playerX] == 'x') && map[playerY+1][playerX] != 'B') {
                    map[playerY][playerX] = ' ';
                    map[playerY+1][playerX] = '@';
                    playerY++;
                }
            }
            break;
    }
    for(int i = 0; i < 7; i++) {
         if(map[finalY[i]][finalX[i]] != 'x' && map[finalY[i]][finalX[i]] != 'P' && map[finalY[i]][finalX[i]] != '@') {
             map[finalY[i]][finalX[i]] = 'x';
         }
    }
    refresh();
}

int main() {
    bool gameOver = false;
    initscr();
    curs_set(0);
    noecho();
    setup();
    while(!gameOver) {
        mvprintw(0,0,"dests: %d", destCount);
        printMap();
        int ch = getch();
        if (ch == 'a') dir = LEFT;
        if (ch == 'd') dir = RIGHT;
        if (ch == 'w') dir = UP;
        if (ch == 's') dir = DOWN;
        if (ch == 'e') gameOver = true;
	if (ch == 'v') destCount = winCount;
        rules();
        if (destCount == winCount) {
	    clear();
            refresh();
	    char win[]="You win!";
            mvprintw(LINES/2,(COLS-strlen(win))/2, win);
            gameOver = true;
        }
    }

    if (destCount != winCount) {
	erase();
	char lose[]="You lose!";
        mvprintw(LINES/2,(COLS-strlen(lose))/2, lose);
    }
    refresh();
    getch();
    endwin();
    return 0;
}


