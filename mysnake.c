//Draw border of snake pit and initialize snake size 5 moving right

/* Can only move in x direction for now,
 * isn't controlled by user input yet
 */

#include <stdio.h>
#include <curses.h>
#include <unistd.h>

#define DELAY 60000 //Controls speed of snake

void draw();

int main(){

    draw();

    //Create the snake and automatically move it
    /* So far, only works in x-axis and
     * doesn't accept any user-input to change direction yet
     */

    int x=1, y=1;
    int xprev=1, yprev=1;
    int xmax = COLS-1, ymax = LINES-1;
    int xnext = 0;
    int xdir = 1, ydir = 1;

    char snake[] = "ooooo";
    int snakeSize = 5;
    char eraser[] = " ";

    while(1) {
        mvprintw(yprev,xprev,eraser);
        /* Code from class powerpoints clears the screen here but
         * that will also clear the border of the snake pit,
         * so instead this prints " " over the previous printing to erase.
         * Must keep track of previous coordinates (xprev and yprev)
         * and adjust if snake changes direction.
         */
        mvprintw(y,x,snake);
        
        refresh();
        usleep(DELAY);

        xnext = x + xdir;

        if ((xnext > (xmax - snakeSize)) || (xnext < 1)){
            xdir *= -1;
        }
        else {
            if (xdir > 0)
                xprev = x;
            else {
                xprev = x + snakeSize -1;
            }
            x += xdir;
        }
    }

    endwin();

    return 0;
}

void draw() {
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