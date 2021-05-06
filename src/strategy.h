#ifndef _QUOR_STRATEGY_H_
#define _QUOR_STRATEGY_H_

#include "utils.h"

struct node
{
    size_t v; 
    size_t site_predecessor; 
    int dist; 
}; 

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

struct moves_valids* dijkstra(struct graph_t* graph, size_t n, size_t pos, size_t ennemy_pos, enum color_t c, size_t* winning_nodes, size_t numb_win);


/*
size_t path_dijkstra(struct player* p);
*/

#endif