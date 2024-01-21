/*
 *
 * 	File:			cgol.c
 *	Author:			Keon Madani
 *	Description:	Conway's Game of Life logic.
 *	Date Created:	21 January 2024
 *	Last Modified:	21 January 2024
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> // nanosleep

#include "cgol.h"

int TICK_DELAY = 10;

/********************************
 *     game helper functions    *
 ********************************/

/*
 * return the cell at location row, col for the current state
 */
char cgol_get(CGOL *game, int row, int col) {
	return game->game_matrix[game->cols * row + col];
}

/*
 * return the cell at location row, col for the next state
 */
char cgol_get_next(CGOL *game, int row, int col) {
	return game->next_matrix[game->cols * row + col];
}

/*
 * set cell value at location row, col for the current state
 */
void cgol_set(CGOL *game, int row, int col, char value) {
	game->game_matrix[game->cols * row + col] = value;
}

/*
 * set cell value at location row, col for the next state
 */
void cgol_set_next(CGOL *game, int row, int col, char value) {
	game->next_matrix[game->cols * row + col] = value;
}

/*
 * returns the number of live cells adjacent to the target cell at location row, col
 */
int cgol_adjacent(CGOL *game, int row, int col) {
	int x, y;
	int num_adjacent = 0;
	for (y = -1; y < 2; y++) {
		for (x = -1; x < 2; x++) {
			if ( (x == 0 && y == 0) ) {
				// skip the target cell
				continue;
			}

			if ( cgol_get(game, row + y, col + x) == LIVE_CELL ) {
				num_adjacent += 1;
			}
		}
	}
	return num_adjacent;
}

/*
 * picks a random state for each cell in the game matrix
 */
void cgol_populate(CGOL *game) {
	int x, y;

	for (y = 0; y < game->rows; y++) {
		for (x = 0; x < game->cols; x++) {
			if ( randrange(1, 100) < 70 ) {
				cgol_set(game, y, x, 0);
			} else {
				cgol_set(game, y, x, 1);
			}
			
		}
	}
}


/*******************************
 *     game tick functions     *
 *******************************/

/*
 * do a single game tick
 */
void tick(CGOL *game) {
	game->tick_delay--;

	if ( game->tick_delay <= 0 ) {
		refresh_matrix(game);
		game->tick_delay = TICK_DELAY;
	}
}

/*
 * Generates the next frame of the game and stores it in next_matrix
 */
void generate_next_matrix(CGOL *game) {
	int x, y;
	int live_neighbours;
	char current_cell;

	for (y = 0; y < game->rows; y++) {
		for (x = 0; x < game->cols; x++) {
			current_cell = cgol_get(game, y, x);
			live_neighbours = cgol_adjacent(game, y, x);

			// underpopulation: each live cell with 1 or no neighbours dies
			if ( (current_cell == LIVE_CELL) && (live_neighbours <= 1) ) {
				cgol_set_next(game, y, x, DEAD_CELL);
			}
			
			// overpopulation: each live cell with 4 or more neighbours dies
			else if ( (current_cell == LIVE_CELL) && (live_neighbours >= 4) ) {
				cgol_set_next(game, y, x, DEAD_CELL);
			}

			// survival: each live cell with two or three neighbours survives
			else if ( (current_cell == LIVE_CELL) && (live_neighbours == 2 || live_neighbours == 3)) {
				cgol_set_next(game, y, x, LIVE_CELL);
			}

			// reproduction: each dead cell with three neighbours becomes populated
			else if ( (current_cell == DEAD_CELL) && (live_neighbours == 3) ) {
				cgol_set_next(game, y, x, LIVE_CELL);
			}
			
		}
	}
}

/*
 * Sets game->game_matrix to game->next_matrix
 */
void refresh_matrix(CGOL *game) {
	generate_next_matrix(game);

	int x, y;
	char cell_next_state;
	for (y = 0; y < game->rows; y++) {
		for (x = 0; x < game->cols; x++) {
			cell_next_state = cgol_get_next(game, y, x);
			cgol_set(game, y, x, cell_next_state);
		}
	}
}

/*********************************
 *     main public functions     *
 *********************************/

/*
 * game object initialization logic
 */
void cgol_init(CGOL *game, int rows, int cols) {
	game->rows = rows;
	game->cols = cols;
	game->game_matrix = malloc(rows * cols);
	game->next_matrix = malloc(rows * cols);
	memset(game->game_matrix, DEAD_CELL, rows * cols);
	memset(game->next_matrix, DEAD_CELL, rows * cols);
	game->tick_delay = TICK_DELAY;
	game->paused = false;
	srand(time(NULL));
}

/*
 * game object creation
 */
CGOL* cgol_create(int rows, int cols) {
	CGOL* game = malloc(sizeof(CGOL));
	cgol_init(game, rows, cols);
	cgol_populate(game);
	return game;
}

/*
 * game object cleanup & deletion
 */
void cgol_clean(CGOL *game) {
	free(game->game_matrix);
	free(game->next_matrix);	
}

void cgol_delete(CGOL *game) {
	cgol_clean(game);
	free(game);

}

void cgol_erase_matrix(CGOL *game) {
	memset(game->game_matrix, DEAD_CELL, game->rows * game->cols);
	memset(game->next_matrix, DEAD_CELL, game->rows * game->cols);
}

/***************************
 *     misc. functions     *
 ***************************/

void sleep_milli(int milliseconds) {
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = milliseconds * 1000 * 1000;
	nanosleep(&ts, NULL);
}

/*
 * generate a random int between min and max
 */
int randrange(int min, int max) {
	return min + rand() / (RAND_MAX / (max + min + 1) + 1);
}
