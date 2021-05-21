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

/* Returns either a winning move reachable (first choice) or the best move to close the gap
*
*  @param pos Position of the player
*  @param winning_nodes List of winning position
*  @param numb_win Length of the winning list
*  @param moves List of moves available
*  @return The best move possible to rush
*/
size_t rushing_path(size_t pos, size_t* winning_nodes, size_t numb_win, struct moves_valids* moves); 

/*  Checks if a vertex is in a list of nodes
*
*   @param nodes array of nodes
*   @param numb_nodes length of nodes array
*   @param v vretex studied
*   @return booleen : 1 (True) / 0 (False)
*/
int vertex_in_nodeList(struct node* nodes, int numb_nodes, size_t v); 

// ============ Double dijkstra =============

/* Gets the path by reaching predecessors
*
*   @param nodes list of nodes
*   @param end last node (final move)
*   @param c color to construct moves
*   @return dijkstra path (move[0] = actual pos, move[numb_nodes-1] = winning pos)
*/
struct moves_valids* get_predecessor(struct node* nodes, struct node end, enum color_t c); 

/* Calculates path by Dijkstra algorithm
*
*   @param p player
*   @param pos position 0 
*   @param ennemy_pos position of the ennemy
*   @param c color of player studied
*   @param winning_nodes array of winning nodes (to search last node)
*   @param numb_win length of winning nodes array
*   @return dijsktra path (move[0] = actual pos, move[numb_nodes-1] = winning pos)
*/
struct moves_valids* dijkstra(struct player* p, size_t pos, size_t ennemy_pos, enum color_t c, size_t* winning_nodes, size_t numb_win);

/* Tells if cutting an edge is going to cut also my own path
*
*  @param path my path
*  @param v1 first node
*  @param v2 second node
*  @return Booleen 1:Yes / 0:False
*/
int is_cutting_path(struct moves_valids* path, size_t v1, size_t v2);

/* Study a specific wall - Putting it and checking the length of the ennemy path
*
*  @param p player
*  @param path player's path
*  @param walls valid walls
*  @param w wall checked
*  @param dir direction of the wall
*  @return length of the final ennemy path
*/
int study_wall(struct player* p, struct moves_valids* path, struct moves_valids* walls, struct move_t w, int dir); 

/* Return a wall that cut the ennemy path without cutting the own path of the player
*
*  @param p player
*  @param path path to victory for the player
*  @param ennemy_path path to victory for the opponent
*  @return a wall that cut ennemy's path OR a move if there is no wall to put
*/
struct move_t cut_ennemy_path(struct player* p, struct moves_valids* path, struct moves_valids* ennemy_path); 


/* Strategy of a player 
*
*  @param p player
*  @return move_t to play
*/
struct move_t double_dijkstra_strategy(struct player* p); 


// ============ SUPER Rick  =============

/* Find two edges on side of one, create the walls and add them to the struct if they are legal
*
*   @param p player 
*   @param walls list of all valid walls
*   @param w the struct where we add the wall
*   @param wall initial wall with only 1 interesting edge (e[0])
*   @return no return
*/
void add_wall_by_vertex(struct player* p, struct moves_valids* walls, struct moves_valids* w, struct move_t wall); 

/* Fill a moves_valids struct with all interesting walls by the state of ennemy path
*
*   @param p the player
*   @param ennemy_path path of the ennemy
*   @param walls list of all valid walls
*   @param w the struct that we fill
*   @return the struct w filled
*/
struct moves_valids* fill_wall_array(struct player* p, struct moves_valids* ennemy_path, struct moves_valids* walls, 
struct moves_valids* w); 

/* Select a wall from 2 LEGAL walls with an edge in commun based on odd/even edges
*
*  @param p player
*  @param wall1 first wall studied
*  @param wall2 second wall studied
*  @return wall 1 (odd edges by side or no difference between both) or wall 2 (odd edges by side)
*
*/
struct move_t best_wall_from_line(struct player* p, struct move_t wall1, struct move_t wall2); 

/* Calculates for all interesting walls the "best" one (ennemy_length - player_length) + chosing the closest from our player base
*
*   @param p player
*   @param w struct of interesting walls
*   @return best move from our definition, return a wall if no interesting walls
*/
struct move_t super_study_gap(struct player* p, struct moves_valids* w);

/* Calls function fill_wall_array + super_study_gap
*
*  @param p player
*  @param ennemy_path ennemy path
*  @return best wall or a move if no interesting wall
*/
struct move_t find_best_wall(struct player* p, struct moves_valids* ennemy_path);


/* Strategy of a player 
*
*  @param p player
*  @return move_t to play
*/
struct move_t super_double_dijkstra_strategy(struct player* p); 

#endif
