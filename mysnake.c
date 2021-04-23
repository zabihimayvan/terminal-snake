/* Isn't controlled by user input yet,
    need to figure out how to get user input without interrupting program
 * Also, doesn't erase the snake tails after changing direction
    We could keep track of tails that need to be erased in a list,
    when the list is not empty, run an extra eraser on those tails
 */

/* Keypad control stuff comes from Chase's code
    I just modified it for how I was keeping track of direction
 * inMotion and the game over message were also from Chase's code.
 */

#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define DELAY 60000 //Controls speed of snake

void draw(); //Initialize screen and draw snake pit border

int main(){

    draw();

    int inMotion = 1;
    char dead[] = "GAME OVER";
    char exitMessage[] = "Press any key to exit.";

    int x=1, y=1; //Snake head position
    int xprev=1, yprev=1; //Control for snake eraser
    int xmax = COLS-1, ymax = LINES-1; //Boundaries of pit
    int xdir = 1, ydir = 1, toggle = 1; //Controls direction

    char snake[] = "o";
    int snakeSize = 5;
    char eraser[] = " ";

    //Count variable is just for testing
    int count = 0;

    while(inMotion == 1) {
        if ((xprev >= 1) && (xprev < xmax) && (yprev >= 1) && (yprev < ymax)) {
            //Prints " " over the previous printing to erase.
            mvprintw(yprev,xprev,eraser);
        }
      
        int ch;
        //ch = getch();

        if (ch == KEY_UP) {
            if ((toggle == -1) && (ydir == 1)) {
                //Lose game if direction reverses
                inMotion = 0;
            }
            toggle = -1;
            ydir = -1;
            xprev = x;
        }
        else if (ch == KEY_DOWN) {
            if ((toggle == -1) && (ydir == -1)) 
                inMotion = 0;
            toggle = -1;
            ydir = 1;
            xprev = x;
        }
        else if (ch == KEY_LEFT) {
            if ((toggle == 1) && (xdir == 1)) 
                inMotion = 0;
            toggle = 1;
            xdir = -1;
            yprev = y;
        }
        else if (ch == KEY_RIGHT) {
            if ((toggle == 1) && (xdir == -1)) 
                inMotion = 0;
            toggle = 1;
            xdir = 1;
            yprev = y;
        }
        
        //These "if (count)" statements are test cases for movement
        //Delete when we figure out user input
        count++;
        if (count == 20) {
            ch = KEY_DOWN;          
        }
        if (count == 30) {
            ch = KEY_LEFT;
        }
        if (count == 40) {
            ch = KEY_DOWN;
        }

        if (toggle > 0) {
            mvprintw(y,x,snake);
        
            refresh();
            usleep(DELAY);

            if ((x >= xmax) || (x < 1)){
                inMotion = 0;
            }
            else {
                if (xdir > 0)
                    xprev = x - snakeSize + 1;
                else {
                    xprev = x + snakeSize - 1;
                }
                x += xdir;
            }
        }

        else {
            mvprintw(y,x,snake);
            refresh();
            usleep(DELAY);

            if((y >= ymax) || (y < 1)) {
                inMotion = 0;
            }
            else {
                if (ydir > 0)
                    yprev = y - snakeSize + 1;
                else {
                    yprev = y + snakeSize - 1;
                }
                y += ydir;
            }
        }
    }

    mvprintw((ymax/2)-1, xmax/2, dead);
    mvprintw(ymax/2,(xmax/2)-6,exitMessage);
    getch();

    endwin();

    return 0;
}

void draw() {
    //Initialize screen
    keypad(stdscr,TRUE);
    initscr();
    clear();
    noecho();
    curs_set(FALSE);

    //Draw the border of the snake pit
    char message[] = "*";
    move(0,0);
    for (int i=0; i<COLS; i++)
        addstr(message);
    for (int i=1; i<LINES-1; i++) {
        move(i,0);
        addstr(message);
    }
    move(LINES-1,0);
    for (int i=0; i<COLS; i++)
        addstr(message);
    for (int i=1; i<LINES-1; i++) {
        move(i,COLS-1);
        addstr(message);
    }
    refresh();
}
