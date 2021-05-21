#ifndef _QUOR_STRATEGY_WHITE_MAGE_H_
#define _QUOR_STRATEGY_WHITE_MAGE_H_

#include "utils.h"


// ============ White Mage =============

/* Finds the closest side of the board for a player
 * 
 * @param graph a graph
 * @param n size of the graph
 * @param pos Player position
 * @return the direction towards the closest side (EAST or WEST)
 */
enum direction find_closest_side(struct graph_t* graph, size_t n, size_t pos);

/* Finds the position of the black wall (white mage strategy)
 * 
 * @param graph a graph
 * @param n size of the graph
 * @param side Player closest side
 * @param color Player color
 * @param pos Player position
 * @return the black wall
 */
struct move_t find_black_wall(struct graph_t* graph, size_t n, enum direction side, enum color_t color, size_t pos);

/* Finds the position of a wall protecting a vertical line end position
 *
 * @param graph a graph
 * @param n size of the graph
 * @param pos position of a vertex on the vertical line
 * @param d vertical direction
 * @return a valid protecting wall
 */
struct move_t protect_line(struct graph_t* graph, size_t n, size_t pos, enum direction d);

/* Finds the position of a wall in front of a player
 *
 * @param graph a graph
 * @param n size of the graph
 * @param pos position of the player
 * @param d blocked direction
 * @return a valid blocking wall next to the player
 */
struct move_t find_blocking_wall(struct graph_t* graph, size_t n, size_t pos, enum direction d);

/* Finds the position of the trap "end door" wall
 *
 * @param graph a graph
 * @param n size of the graph
 * @param pos vertex in the hallway
 * @param d hallway direction
 * @return a valid end door wall closing the trap
 */
struct move_t find_trap_door(struct graph_t* graph, size_t n, size_t pos, enum direction d);

/* Finds the position of a continuing horizontal wall for the trap setup
 *
 * @param graph a graph
 * @param n size of the graph
 * @param pos player position next to the horizontal walls
 * @param d direction of the trap
 * @param side direction of the horizontal walls
 * @return a valid horizontal wall continuing the trap
 */
struct move_t find_next_horizontal_wall(struct graph_t* graph, size_t n, size_t pos, enum direction d, enum direction side);

/* Finds the position of a continuing vertical wall for the trap hallway
 *
 * @param graph a graph
 * @param n size of the graph
 * @param pos player position next to the vertical walls
 * @param d direction of the trap
 * @param side direction of the closest board side
 * @return a valid horizontal wall continuing the trap
 */
struct move_t find_next_vertical_wall(struct graph_t* graph, size_t n, size_t pos, enum direction d, enum direction side);

/* Checks if a player is ahead of another
 *
 * @param graph a graph
 * @param n size of the graph
 * @param pos1 position of the first player
 * @param pos2 position of the second player
 * @return 1 if player 1 is ahead, 0 else
 */
int is_ahead(struct graph_t* graph, size_t n, size_t pos1, size_t pos2);

/* Checks if the trap can be closed the wrong way
 *
 * @param graph a graph
 * @param n size of the graph
 * @param pos (ennemy) player position
 * @param exit exit vertex
 * @return 1 if a wall can close the trap, 0 else
 */
int is_trap_still_valid(struct graph_t* graph, size_t n, size_t pos, size_t exit);

/* Computes the width of the trap hallway 
 *
 * @param graph a graph
 * @param n size of the graph
 * @param pos vertex in the hallway
 * @return the hallway width at that position
 */
int hallway_width(struct graph_t* graph, size_t n, size_t pos);

#endif
