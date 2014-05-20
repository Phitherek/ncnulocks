#include "nulocks-core/Board.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <ncurses.h>
#include <cmath>
using namespace std;

void draw_layout(int rows, int cols) {
        std::string msg;
        msg = "NCNulocks v. 0.1 (C) 2014 by Phitherek_";
        mvprintw(0, (cols-msg.length())/2, "%s", msg.c_str());
        msg = "W, S, A, D to move, Q to exit";
        mvprintw(rows-1, (cols-msg.length())/2, "%s", msg.c_str());
}

void draw_board(Nulocks::Core::Board* b, int rows, int cols) {
	int height = rows-7;
	int width = cols-4;
	int startcol = 2;
	int endcol = cols-2;
	int startrow = 4;
	int endrow = rows-3;
	int blockwidth, blockheight;
	blockheight = height/b->getSize();
	blockwidth = width/b->getSize();
	if(blockwidth < 1 || blockheight < 1) {
		delete b;
		endwin();
		cout << "Your screen is too small to play this version!" << endl;
		exit(EXIT_FAILURE);
	}
	int i, j;
	int** repr;
	repr = new int*[b->getSize()];
	for(int q = 0; q < b->getSize(); q++) {
		repr[q] = new int[b->getSize()];
	}
	b->getRepresentation(&repr);
	for(int p = 0; p < b->getSize(); p++) {
		int cstartrow = startrow+(p*blockheight);
		int cendrow = cstartrow+blockheight;
		for(int q = 0; q < b->getSize(); q++) {
			int cstartcol = startcol+(q*blockwidth);
			int cendcol = cstartcol+blockwidth;
			mvprintw(cstartrow, cstartcol, "+");
			mvprintw(cendrow, cstartcol, "+");
			for(i = cstartcol+1; i < cendcol; i++) {
				mvprintw(cstartrow, i, "-");
				mvprintw(cendrow, i, "-");
			}
			mvprintw(cstartrow, cendcol, "+");
			mvprintw(cendrow, cendcol, "+");
			for(j = cstartrow+1; j < cendrow; j++) {
				mvprintw(j, cstartcol, "|");
				mvprintw(j, cendcol, "|");
			}
			mvprintw(cstartrow+(blockheight/2), cstartcol+(blockwidth/2), "%d", repr[p][q]);
		}
	}
	for(int q = 0; q < b->getSize(); q++) {
		delete[] repr[q];
	}
	delete[] repr;
}

int main() {
        initscr();
        int rows, cols;
        char action = 'i';
        std::string status = "initial";
        char* buffer;
        bool continue_requested = false;
        Nulocks::Core::Board* b = NULL;
        getmaxyx(stdscr, rows, cols);
        do {
                std::string msg;
                char buf[1000];
                int size, endpow;
                status = "typequery";
                while(status == "typequery") {
                	msg = "NCNulocks v. 0.1 (C) 2014 by Phitherek_";
                	mvprintw(0, (cols-msg.length())/2, "%s", msg.c_str());
                	msg = "Play: (o)riginal 2048-like game, (c)ustom game, (q)uit: ";
                	mvprintw(2, 0, "%s", msg.c_str());
                	refresh();
                	action = getch();
                	if(action == 'q') {
                	        break;
                	} else if(action == 'c') {
                	        clear();
                	        msg = "NCNulocks (C) 2014 by Phitherek_";
                	        mvprintw(0, (cols-msg.length())/2, "%s", msg.c_str());
                	        msg = "Size of the board: ";
                	        mvprintw(2, 0, "%s", msg.c_str());
                	        getstr(buf);
                	        size = atoi(buf);
                	        msg = "Ending power of 2: ";
                	        mvprintw(3, 0, "%s", msg.c_str());
                	        getstr(buf);
                	        endpow = atoi(buf);
                	        b = new Nulocks::Core::Board(size, endpow);
                	        status = "gamestart";
                	} else if(action == 'o') {
                	        b = new Nulocks::Core::Board;
                	        status = "gamestart";
                	}
                	clear();
                }
                if(status == "gamestart") {
                	msg = "NCNulocks v. 0.1 (C) 2014 by Phitherek_";
                	mvprintw(0, (cols-msg.length())/2, "%s", msg.c_str());
                	ostringstream msgss;
                	msgss.str("");
                	msgss << "GAME RULES: Move the blocks on the board and match the numbers to get to ";
                	msgss << pow(2, b->getWinPower());
                	msgss << " block! Press any key to continue, q to exit...";
                	msg = msgss.str();
                	mvprintw(rows/2, (cols-msg.length())/2, "%s", msg.c_str());
                	action = getch();
                	if(action == 'q') {
                		break;
                	}
                	status = "game";
                }
                if(status == "game") {
                	cbreak();
                	keypad(stdscr, true);
                	noecho();
                	curs_set(0);
                }
                while(status == "game") {
                	clear();
                	draw_layout(rows, cols);
                	mvprintw(2, 0, "Your score: %d", b->getScore());
                	draw_board(b, rows, cols);
                	if(b->gameLost()) {
                		status = "lost";
                		break;
                	} else if(b->gameWon() && !continue_requested) {
                		do {
                			clear();
                			msg = "NCNulocks v. 0.1 (C) 2014 by Phitherek_";
                			mvprintw(0, (cols-msg.length())/2, "%s", msg.c_str());
                			mvprintw(2, 0, "Your score: %d", b->getScore());
                			msg = "You WON! Press C to continue playing, Q to exit...";
                			mvprintw(rows/2, (cols-msg.length())/2, "%s", msg.c_str());
                			action = getch();
                		} while(action != 'c' && action != 'q');
                		if(action == 'c') {
                			continue_requested = true;
                			continue;
                		} else if(action == 'q') {
                			break;
                		}
                	} else if(b->gameWon()) {
                		msg = "You have WON this game!";
                		mvprintw(2, (cols-msg.length())/2, "%s", msg.c_str());
                	}
                	action = getch();
                	if(action == 'q') {
                		break;
                	} else if(action == 'w') {
                		b->moveUp();
                	} else if(action == 's') {
                		b->moveDown();
                	} else if(action == 'a') {
                		b->moveLeft();
                	} else if(action == 'd') {
                		b->moveRight();
                	}
                }
                if(status == "lost") {
                	clear();
                	msg = "NCNulocks v. 0.1 (C) 2014 by Phitherek_";
                	mvprintw(0, (cols-msg.length())/2, "%s", msg.c_str());
                	mvprintw(2, 0, "Your score: %d", b->getScore());
                	msg = "You have lost the game :(. Press any key to exit...";
                	mvprintw(rows/2, (cols-msg.length())/2, "%s", msg.c_str());
                	getch();
                	action = 'q';
                	break;
                }
        } while(action != 'q');
        if(b != NULL) {
                delete b;
        }
        endwin();
        printf("%s", "All done! Thanks for playing!\n");
        return EXIT_SUCCESS;
}
