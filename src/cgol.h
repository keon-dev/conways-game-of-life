/*
 *
 * 	File:			cgol.h
 *	Author:			Keon Madani
 *	Description:	Conway's Game of Life declarations.
 *	Date Created:	21 January 2024
 *	Last Modified:	21 January 2024
 *
 */


#ifndef CGOL_H
#define CGOL_H

#include <stdio.h>
#include <stdbool.h>

/*
 * possible states for 1x1 cell in the game's matrix
 */
typedef enum {
	DEAD_CELL, LIVE_CELL
} cell_state;

/*
 * row,col coordinate to define cell location in the game
 */ 
typedef struct {
	int row;
	int col;
} coords;

/*
 * a 1x1 cell object
 */
typedef struct {
	int state;
	coords location;
} game_cell;

/*
 * game object
 */
typedef struct {
	int rows;
	int cols;

	char* game_matrix;
	char* next_matrix;

	int tick_delay;
	bool paused;
} CGOL;

/*
 * controls tick speed of the game
 */
extern int TICK_DELAY;

// game helper functions
char cgol_get(CGOL *game, int row, int col);
char cgol_get_next(CGOL *game, int row, int col);
void cgol_set(CGOL *game, int row, int col, char value);
void cgol_set_next(CGOL *game, int row, int col, char value);
int cgol_adjacent(CGOL *game, int row, int col);
void cgol_populate(CGOL *game);

// game tick functions
void tick(CGOL *game);
void generate_next_matrix(CGOL *game);
void refresh_matrix(CGOL *game);

// game object management
void cgol_init(CGOL *game, int rows, int cols);
CGOL* cgol_create(int rows, int cols);
void cgol_clean(CGOL *game);
void cgol_delete(CGOL *game);
void cgol_erase_matrix(CGOL *game);

// misc. functions
void sleep_milli(int milliseconds);
int randrange(int min, int max);


#endif //CGOL_H
