#include <ncurses.h>
#include <unistd.h>
#include <string.h>

#define LEFTEDGE 0
#define RIGHTEDGE 50
#define TOPEDGE 0
#define BOTTOMEDGE 20

int main(int argc, char *argv[]) {

    char snake[] = "====>";
    char blank[] = "     ";
    char dead[] = "Dead";
    int inMotion = 1;
    int dirX = +1;
    int dirY = 0;
    int posX = LEFTEDGE+1;
    int posY = 10;
    
    keypad(stdscr,TRUE); // enables working with the arrow keys
    initscr();
    noecho();
    curs_set(FALSE);

    initscr();
    clear();

    mvprintw(TOPEDGE, 25, "-");
    mvprintw(BOTTOMEDGE, 25, "-");
    mvprintw(10, RIGHTEDGE+5, "|");   
    mvprintw(10, LEFTEDGE, "|");

    while(inMotion != 0) {
        move(posY, posX);
        addstr(snake);
        move(LINES-1,COLS-1);
        refresh();
        usleep(100000);
        move(posY, posX);
        
        if (posX >= RIGHTEDGE) {
            addstr(blank);
            mvprintw(10, 24, dead);
            refresh();
            inMotion = 0;
        }
        else if (posX <= LEFTEDGE) {
            addstr(blank);
            mvprintw(10, 24, dead);
            refresh();
            inMotion = 0;
        }
        // else if (posY >= TOPEDGE) {
        //     addstr(blank);
        //     mvprintw(15, 25, dead);
        //     refresh();
        //     inMotion = 0;
        // }
        // else if (posY <= BOTTOMEDGE) {
        //     addstr(blank);
        //     mvprintw(15, 25, dead);
        //     refresh();
        //     inMotion = 0;
        // }
        else {
            int ch=getch();
            char buf[100];
            if (ch == KEY_UP) {
                strcpy(snake, "^");
                addstr(snake);
                dirY += 1;
            }
            else if (ch == KEY_DOWN) {
                strcpy(snake, "^");
                addstr(snake);
                dirY -= 1;
            }
            else if (ch == KEY_RIGHT) {
                strcpy(snake, "====>");
                addstr(snake);
                dirX += 1;
            }
            else if (ch == KEY_LEFT) {
                strcpy(snake, "<====");
                addstr(snake);
                dirX -= 1;
            }
            refresh();
        }

        addstr(blank);
        posX += dirX;
        posY += dirY;
    }

    sleep(5);
    endwin();
}