#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>

#define DELAY 60000 //Controls speed of snake

/*a coordPair will contain an xCoord and a yCoord, which will represent the coordinates
of the snake's body */
typedef struct coordPair {
    int xCoord;
    int yCoord;
} coordPair;

int maxY, maxX;
int nextX, nextY; 
int snakeLength = 5, inMotion = 1;
char currentDir;

/* creates the snake as an array of coordinate pairs, each coordPair contains
an xCoord and yCoord value*/
coordPair snakeBody[255] = {};

//function to start the game
void startGame() {

    //create screen
    initscr(); 
    noecho(); 
    keypad(stdscr, TRUE);
    wtimeout(stdscr, 0);
    curs_set(FALSE); 
    maxX = getmaxx(stdscr);
    maxY = getmaxy(stdscr); 

    //begin with snake moving right
    currentDir = 'R';
    inMotion = 1;

    clear(); 
    
    //Snake starts with length of 5
    int j = 0;
    for(int i = snakeLength; i >= 0; i--) {
        coordPair currentPoint;
        
        //snake starts on left edge, vertically centered
        currentPoint.xCoord = i;
        currentPoint.yCoord = maxY / 2; 

        snakeBody[j] = currentPoint;
        j++;
    }

    refresh();
}


void draw() {

    clear(); 
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

    //Print dead in the center of the screen if inMotion == 0
    if(!inMotion)
        mvprintw(maxY / 2, maxX / 2, "Dead");

    //Draw the snake
    for(int i = 0; i < snakeLength; i++) {
        mvprintw(snakeBody[i].yCoord, snakeBody[i].xCoord, "o");
    }

    refresh();

    //controls snake speed
    usleep(DELAY); 
}

/* Main */
int main(int argc, char *argv[]) {

    //initialize screen and starting values
    startGame();

    while(inMotion) {
        
        //handle user input
            int ch = getch();
            /*only allow 90 degree changes in direction, no reversing*/
            if(ch == KEY_RIGHT){
                if (currentDir == 'L')
                    inMotion = 0;
                else
                    currentDir = 'R';
            } else if (ch == KEY_LEFT) {
                if (currentDir == 'R')
                    inMotion = 0;
                currentDir = 'L';
            } else if(ch == KEY_DOWN) {
                if (currentDir == 'U')
                    inMotion = 0;
                currentDir = 'D';
            } else if(ch == KEY_UP) {
                if (currentDir == 'D')
                    inMotion = 0;
                currentDir = 'U';
            }

        //snake movement

            //set nextX and nextY based on snakeBody[0]
            nextX = snakeBody[0].xCoord;
            nextY = snakeBody[0].yCoord;

            //increment/decrement coordinates
            if(currentDir == 'R') nextX++;
            else if(currentDir == 'L') nextX--;
            else if(currentDir == 'U') nextY--;
            else if(currentDir == 'D') nextY++;

            snakeBody[snakeLength - 1].xCoord = nextX;
            snakeBody[snakeLength - 1].yCoord = nextY;

            coordPair temp = snakeBody[snakeLength - 1];
             for(int i = snakeLength - 1; i > 0; i--) {
                snakeBody[i] = snakeBody[i-1];
            }
            snakeBody[0] = temp;

            //end game if the snake hits the edges
            if((nextX >= maxX || nextX < 0) || (nextY >= maxY || nextY < 0)) {
                inMotion = 0;
            }

            //draw screen, snake
            draw();
        }

    endwin(); 


}
