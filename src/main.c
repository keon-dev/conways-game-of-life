/*
 *
 * 	File:			main.c
 *	Author:			Keon Madani
 *	Description:	Main file for NCURSES implementation of Conway's Game of Life.
 *	Date Created:	21 January 2024
 *	Last Modified:	21 January 2024
 *
 */

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "cgol.h"

/*
 * prints a cell to the window whose color depends on its state
 * 	- LIVE_CELL: state = 1 (white)
 * 	- DEAD_CELL: state = 0 (black)
 */
void print_cell(WINDOW *w, int state) {
	wattron(w, COLOR_PAIR(state));
	waddch(w, ' '); // we print 2 chars per column because it looks better 
	waddch(w, ' ');
	wattroff(w, COLOR_PAIR(state));
}

/*
 * prints game matrix to ncurses window
 */
void display_board(WINDOW *w, CGOL *game) {
	int x, y;
	box(w, 0, 0);

	for (y = 0; y < game->rows; y++) {
		wmove(w, y + 1, 1);
		for (x = 0; x < game->cols; x++) {
			if ( !(cgol_get(game, y, x) == DEAD_CELL) ) {
				print_cell(w, 1);
			} else {
				print_cell(w, 0);
			}
		}
	}
	wnoutrefresh(w);
}

void display_options(WINDOW *w, CGOL *game) {
	box(w, 0, 0);

	wattron(w, A_STANDOUT | A_BOLD);
	mvwprintw(w, 1, 1, "Options:");

	wattroff(w, A_STANDOUT | A_BOLD);

	mvwprintw(w, 2, 2, "p:  pause simulation");
	mvwprintw(w, 3, 2, "r:  reset board");
	mvwprintw(w, 4, 2, "F1: quit");
	mvwprintw(w, 9, 1, "Simulation: %s", game->paused ? "PAUSED" : "RUNNING");
	
	wrefresh(w);
}

/*
 * ncurses color initialization
 */
void init_colors(void) {
	start_color();
	init_pair(DEAD_CELL, COLOR_BLACK, COLOR_BLACK);	// dead cell color (black)
	init_pair(LIVE_CELL, COLOR_BLACK, COLOR_WHITE); // live cell color (white)
}

/*
 *
 * ncurses initialization
 */
void init_ncurses(void) {
	initscr();				// start curses mode
	cbreak();				// disable line buffering
	noecho();
	timeout(0);				// no blocking on getch()
	curs_set(0);			// set cursor state to invisible
	keypad(stdscr, TRUE);	// enable f-keys
	init_colors();			// setup colors	
}

int main(int argc, char **argv) {
	// init
	WINDOW *w, *options;
	CGOL* game;
	game = cgol_create(30, 30);
	init_ncurses();

	// create interface windows
	options = newwin(10, (game->cols * 2) + 2, 0, 0);
	w = newwin(game->rows + 2, (game->cols * 2) + 2, 10, 0);

	bool running = true;

	// game loop
	while (running) {
		if ( !(game->paused) ) {
			tick(game);
		}

		display_board(w, game);
		display_options(options, game);
		doupdate();
		sleep_milli(10);


		switch (getch()) {
			case KEY_F(1):
				running = false;
				break;
			case 'r':
				cgol_erase_matrix(game);
				cgol_populate(game);
				break;
			case 'p':
				if ( !(game->paused) ) {
					game->paused = true;
				} else {
					game->paused = false;
				}
				break;
			default: 
				break;
		}
	
	}
	

	cgol_delete(game);
	wclear(stdscr);
	endwin();
	return 0;
}

/* * * * */

