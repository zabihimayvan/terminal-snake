#include <bits/types/clock_t.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h> // Needed for ITIMER_REAL

int DELAY = 70000; // Set default speed of snake
int maxY, maxX;
int nextX, nextY; 
int snakeLength = 3, inMotion = 1;
int randTimer, randValue, randX, randY;
int highScore = 0;
char currentDir;
void newTrophy();
void addTrophy();
void startTimer();
void startDir();
clock_t timer;
static void countdown(int);
static int set_ticker( int n_msecs );
static int trophy_gen_in_progress = 0;

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
    DELAY = 70000;

    //begin with snake moving right
    startDir();
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

void loseGame() {
    mvprintw((maxY/2)-1, maxX/2, "GAME OVER");
    exitGame();
}

void winGame() {
    mvprintw((maxY/2)-1, maxX/2, "YOU WIN!");
    exitGame();
}

void startDir() {
    srand(time(NULL));
    int randDir = (rand() % 4);

    if (randDir == 0)
        currentDir = 'R';
    else if (randDir == 1)
        currentDir = 'L';
    else if (randDir == 2)
        currentDir = 'U';
    else
        currentDir = 'D';
}

//Author: Chase
void newTrophy() {
    srand(time(NULL));
    randValue = (rand() % 9) + 1;
    randX = ((rand() % (COLS-4))+2);
    randY = ((rand() % (LINES-4))+2);
    trophy.xCoord = randX;
    trophy.yCoord = randY;
    timer = clock();
}

//Author: Chase/Derek
void addTrophy() {
    if (trophy_gen_in_progress) return;
    trophy_gen_in_progress = 1;
    mvprintw(trophy.yCoord, trophy.xCoord, "%d", randValue);
    trophy_gen_in_progress = 0;
}

//Author: Chase/Derek
void clearTrophy() {
    trophy_gen_in_progress = 1;
    mvprintw(trophy.yCoord, trophy.xCoord, " ");
    trophy_gen_in_progress = 0;
}

void startTimer() {
    int trophy_interval;
    trophy_interval = (rand() % (9+1-1)) + 1;    // Generate a digit randomly chosen from 1 to 9
    set_ticker((trophy_interval + 10) * 1000);   // A trophy expires after a random interval from 11 to 19 seconds
}

//Author: Chase/Derek
void growSnake() {
    int old_snakeLength = snakeLength;
    snakeLength += randValue;
    for (int i = snakeLength - 1; i > old_snakeLength - 1; i--)
    {
        snakeBody[i].xCoord = snakeBody[old_snakeLength -1].xCoord;
        snakeBody[i].yCoord = snakeBody[old_snakeLength -1].yCoord;
    }
    //increases speed propotional to length
    DELAY = DELAY - (randValue * 300);
    if (DELAY < 5000)
        DELAY = 5000;
}

//Author: Chase
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

//Author: Chase
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
        loseGame();

    //Draw the snake
    for(int i = 0; i < snakeLength; i++) {
        mvprintw(snakeBody[i].yCoord, snakeBody[i].xCoord, "o");
    }

    addTrophy();
    refresh();

    //controls snake speed
    usleep(DELAY); 
}

void countdown(int signum)
{
    if (!trophy_gen_in_progress)
    {
        clearTrophy();
    }
    if (!trophy_gen_in_progress)
    {
        newTrophy();
        addTrophy();
    }
    startTimer();
}

/*
 * arranges for interval timer to issue SIGALRMs at regular intervals
 * returns -1 on error, 0 for ok
 * arg in milliseconds, converted into whole seconds and microseconds
 * note: set_ticker(0) turns off ticker
*/
int set_ticker( int n_msecs )
{
        struct itimerval new_timeset;
        long    n_sec, n_usecs;

        n_sec = n_msecs / 1000 ;		/* int part	*/
        n_usecs = ( n_msecs % 1000 ) * 1000L ;	/* remainder	*/

        new_timeset.it_interval.tv_sec  = n_sec;        /* set reload       */
        new_timeset.it_interval.tv_usec = n_usecs;      /* new ticker value */
        new_timeset.it_value.tv_sec     = n_sec  ;      /* store this       */
        new_timeset.it_value.tv_usec    = n_usecs ;     /* and this         */

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

/* Main */
int main(int argc, char *argv[]) {

    //initialize screen and starting values
    startGame();

    signal(SIGALRM, countdown);

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

        //Author: Chase
        //Check to see if the snake got the trophy
        if(snakeBody[0].xCoord == trophy.xCoord && snakeBody[0].yCoord == trophy.yCoord) {
            growSnake();
            newTrophy();
            startTimer();
        }

        if (snakeLength >= (COLS+LINES)) {
            winGame();
        }        

        //Author: Chase
        //end game if the snake hits the edges
        if(wallColliding() || snakeColliding()) {
            inMotion = 0;
        }

        //draw screen, snake
        draw();
       
    }   
}