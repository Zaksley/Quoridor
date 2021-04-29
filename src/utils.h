#ifndef _QUOR_UTILS_H_
#define _QUOR_UTILS_H_

#include "player.h"


struct player
{
   size_t pos; 
   size_t ennemy_pos; 
   enum color_t id;
   int first_move; 

   struct graph_t * graph;
   size_t n; 
   size_t num_walls;


   char* (*get_name)();
   void (*initialize) (enum color_t, struct graph_t*, size_t);
   struct move_t (*player_play) (struct move_t);
   void (*finalize) (); 
}; 

struct moves_valids
{
   struct move_t* valid; 
   int number; 
};


/*
 *   Contains every special function of a player
 */
struct player* get_functions(char* lib);

/* Return the color of the other player
*
*  @param color of a player
*  @return opposite color 
*/
enum color_t other_player(enum color_t player); 

/* Return a player in relation to a specific color 
*
*  @param array of players
*  @param color we want
*  @return player with the color specified
*/
struct player* player_color(struct player** p, enum color_t c); 

// =================== Functions with MOVE = 1 (Moving pawn) ===================

/* Return a list of every valid position to move
*
*  @param p struct player who has to move 
*  @return list of move_t
*/
struct moves_valids* valid_positions(struct player*); 

/*
*  Add a position new to the array valid 
*/
void add_position(struct move_t* valid, struct move_t new, int* count_moves); 

/*
*
*/
size_t rushing_path(size_t pos, size_t* winning_nodes, size_t numb_win, struct moves_valids* moves); 


// =================== Functions with MOVE = 0 (Posing wall) ===================

/*
*  @param pointer on the player
*  @return array of every move_t wall usable
*/
struct moves_valids* valid_walls(struct player* p); 

/* Put a wall on the board meaning destroying 2 edges on the graph
*
*  @param graph we want to update
*  @param wall the wall we are posing 
*  @return 0 if the wall's installation is successful, -1 if it failed
*/
int put_wall(struct graph_t* graph, struct move_t wall); 

/* Destroy a wall on the board meaning adding 2 edges on the graph
*
*  @param graph graph of the board
*  @param wall the wall we are posing 
*  @param dir direction used to add edge
*  @return 0 if the wall destruction is successful, -1 if it failed
*/
int destroy_wall(struct graph_t* graph, struct move_t wall, int dir); 

/* Shifting by the left an array
*  
*  @param s the array
*  @param begin where we start
*  @param end where we stop
*/
void shift_left(size_t s[], int begin, int end); 

/* Checking if a vertex isn't already in the vertex list
*  
*  @param waiting Waitinglist - size_t array
*  @param size Size of the waiting list 
*  @param v vertex studied
*  @return Booleen: 0 => In waitingList / 1 => Not in waiting list
*/
int not_already_inWaitinglist(size_t* waitingList, int size, size_t v);

/* Checking if a player is still able to move to the victory if we put a specific wall
*
*  @param pointer on the player
*  @param Specific color of studied player
*  @param Where the studied player is
*  @return 1 if player can still win, 0 otherwise
*/
int existPath_Player(struct graph_t* graph, size_t n, size_t color, size_t pos);

/* Check if posing a wall is allowed 
*
*  @param pointer on the player
*  @param specific wall we test
*  @param d1 direction 1 of center vertex
*  @param d2 direction 2 of center vertex
*  @return 1 if posing this wall is allowed, 0 otherwise
*/
int checkPath(struct player* p, struct move_t wall, int dir); 

#endif