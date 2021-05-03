#ifndef _QUOR_STRATEGY_H_
#define _QUOR_STRATEGY_H_

#include "utils.h"

/* Return either a winning move reachable (first choice) or the best move to close the gap
*
*  @param pos Position of the player
*  @param winning_nodes List of winning position
*  @param numb_win Length of the winning list
*  @param moves List of moves available
*  @return The best move possible to rush
*/
size_t rushing_path(size_t pos, size_t* winning_nodes, size_t numb_win, struct moves_valids* moves); 

size_t path_dijkstra(struct player* p);

#endif