#include <bits/types/clock_t.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>

#define DELAY 60000 //Controls speed of snake

int maxY, maxX;
int nextX, nextY; 
int snakeLength = 3, inMotion = 1;
int randTimer, randValue, randX, randY;
int highScore = 0;
char currentDir;
void newTrophy();
void addTrophy();
void startTimer();
clock_t timer;

/* a coordPair will contain an xCoord and a yCoord, which will represent the coordinates
of the snake's body */
typedef struct coordPair {
    int xCoord;
    int yCoord;
} coordPair;

/* creates the snake as an array of coordinate pairs */ 
coordPair snakeBody[255] = {};
coordPair trophy;

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
    startTimer();
    snakeLength = 3;

    //begin with snake moving right
    currentDir = 'R';
    inMotion = 1;
    
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

    //Snake starts with length of 3
    int j = 0;
    for(int i = snakeLength; i >= 0; i--) {
        coordPair currentPoint;
        
        //snake starts on left edge, vertically centered
        currentPoint.xCoord = i;
        currentPoint.yCoord = maxY / 2; 

        snakeBody[j] = currentPoint;
        j++;
    }

    newTrophy();
    addTrophy();    

    refresh();
}

void clearScreen(){
    //Print over everything except the border
    char message[] = " ";
    move(1,1);
    for (int i=1; i<LINES-1; i++) {
        for (int j=1; j<COLS-1; j++) {
            move(i,j);
            addstr(message);
        }
    }
}

void exitGame() {
    if (highScore < snakeLength)
        highScore = snakeLength;

    mvprintw((maxY/2)-1, maxX/2, "GAME OVER");
    mvprintw(maxY/2,(maxX/2)-2, "High Score: %d", highScore);
    mvprintw((maxY/2)+1,(maxX/2)-6, "Start a new game? [Y/N]");
    refresh();
    int a = getchar();
    if ((a=='Y') || (a=='y')) {
        startGame();
    }
    else if ((a=='N') || (a=='n')) {
        clear();
        endwin();
        exit(1);
    }
    else {
        mvprintw((maxY/2)+2,(maxX/2)-7, "PLEASE ENTER A VALID KEY");
        exitGame();
    }
}

void newTrophy() {
    srand(time(NULL));
    randValue = (rand() % 9) + 1;
    randX = ((rand() % (COLS-4))+2);
    randY = ((rand() % (LINES-4))+2);
    trophy.xCoord = randX;
    trophy.yCoord = randY;
    timer = clock();
}

void addTrophy() {
    mvprintw(trophy.yCoord, trophy.xCoord, "%d", randValue);
}

void clearTrophy() {
    mvprintw(trophy.yCoord, trophy.xCoord, " ");
}

void startTimer() {
    randTimer = (rand() % 9) + 1;
    timer = clock();
}

void growSnake() {
    snakeLength += randValue;
}

bool snakeColliding() {
    int x = snakeBody[0].xCoord;
    int y = snakeBody[0].yCoord;

    int g = 1;
    while (g < (snakeLength - 1)){
        if (snakeBody[g].xCoord == x  && snakeBody[g].yCoord == y)
            return true;
        g++;
    }
    return false;
}

bool wallColliding() {
    int x = snakeBody[0].xCoord;
    int y = snakeBody[0].yCoord;

    if (x == COLS-1 || x == 0 || y == LINES-1 || y == 0)
        return true;
    
    return false;
}

void draw() {
    clearScreen(); //Clear all but the border

    //Print dead in the center of the screen if inMotion == 0
    if(!inMotion)
        exitGame();

    //Draw the snake
    for(int i = 0; i < snakeLength; i++) {
        mvprintw(snakeBody[i].yCoord, snakeBody[i].xCoord, "o");
    }

    addTrophy();
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

        //Check to see if the timer went off before the snake 
        if ((timer/CLOCKS_PER_SEC) >= randTimer) {
            clearTrophy();
            newTrophy();
            addTrophy();
            startTimer();
        }

        //Check to see if the snake got the trophy
        if(snakeBody[0].xCoord == trophy.xCoord && snakeBody[0].yCoord == trophy.yCoord) {
            growSnake();
            newTrophy();
            startTimer();
        }

        //end game if the snake hits the edges
        if(wallColliding() || snakeColliding()) {
            inMotion = 0;
        }

        //draw screen, snake
        draw();
    }    

}