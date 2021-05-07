#ifndef _QUOR_STRATEGY_H_
#define _QUOR_STRATEGY_H_

#include "utils.h"

struct node
{
    size_t v; 
    size_t site_predecessor; 
    int dist; 
}; 

// ============ General functions =============

/* Return either a winning move reachable (first choice) or the best move to close the gap
*
*  @param pos Position of the player
*  @param winning_nodes List of winning position
*  @param numb_win Length of the winning list
*  @param moves List of moves available
*  @return The best move possible to rush
*/
size_t rushing_path(size_t pos, size_t* winning_nodes, size_t numb_win, struct moves_valids* moves); 


int vertex_in_nodeList(struct node* nodes, int numb_nodes, size_t v); 

struct moves_valids* get_predecessor(struct node* nodes, struct node end, enum color_t c); 

struct moves_valids* dijkstra(struct graph_t* graph, size_t n, size_t pos, enum color_t c, size_t* winning_nodes, size_t numb_win);


// ============ White Mage =============

/* Find the closest side of the board for a player
 * 
 * @param pos Player position
 * @param color Player color
 * @return the direction towards the closest side (EAST or WEST)
 */
enum direction find_closest_side(struct graph_t* graph, size_t pos, enum color_t color);

/* Find the position of the black whole (white mage strategy)
 * 
 * @param side Player closest side
 * @param color Player color
 * @return the black wall
 */
struct move_t find_black_wall(enum direction side, enum color_t color);

/*
size_t path_dijkstra(struct player* p);
*/

#endif