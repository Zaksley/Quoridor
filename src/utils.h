#ifndef _QUOR_UTILS_H_
#define _QUOR_UTILS_H_

#include "player.h"

// ===================  PLAYER =================== 

struct player
{
      // Description player
   size_t pos; 
   size_t ennemy_pos; 
   enum color_t id;
   int first_move; 

      // Description graph
   struct graph_t * graph;
   size_t n; 
   size_t num_walls;

      // Stock walls
   struct graph_t* naked_graph;
   int* wall_installed; 

      // Win
   int numb_win; 
   size_t* winning_nodes;
   size_t* owned_nodes; 
   

      // Functions client
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

/* Create a fake player fast to test functions
*
*  @param graph Graph created that we link to our player
*  @param n size of graph
*  @param pos position of the player
*  @param ennemy_pos ennemy position
*  @return a player
*/
struct player* initialize_test_player(struct graph_t* graph, size_t n, size_t pos, size_t ennemy_pos); 

/* Return an initialized player
*
*  @param self player created in Client
*  @param id color chosed
*  @param graph graph from server
*  @param num_walls number of walls chosed by server
*  @return player initialized 
*/
struct player initialization_player(struct player self, enum color_t id, struct graph_t* graph, size_t num_walls); 

/* Free elements of player
*
*  @param self struct player to free
*/
void finalization_player(struct player self); 


/* Gets opposite color
*
*  @param color of player
*  @return opposite color
*/
enum color_t other_player(enum color_t player); 

/* Gets opposite structure player
*
*  @param p array of players (Server)
*  @param c color of a player
*  @return opposite player
*/
struct player* player_color(struct player** p, enum color_t c); 

// ===================  PLAYER =================== 

// ===================  MOVE =================== 

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

// ===================  MOVE =================== 

// =================== WALL =================== 

size_t position_square(size_t left_square, int n);

size_t min_node(size_t node, size_t n_neighboor, size_t n1, size_t n2);

/* Check if in a square of 4 nodes, there is already a wall
*
*
*/
int already_wall_in_square(int wall_used[], size_t n, size_t left_square);


      // ======= Usefull functions on walls =======

/* Check if a wall isn't already in the list
*
*  @param size size of array moves
*  @param moves array stocking the moves
*  @param wall checking a specific wall
*  @return booleen 1 if wall is in the list, 0 otherwise 
*/
int wall_in_list(int size, struct move_t* moves, struct move_t wall);


      // ======= Usefull functions on walls =======

/*
*  @param pointer on the player
*  @return array of every move_t wall usable
*/
struct moves_valids* valid_walls(struct player* p); 

/* Put a wall on the board meaning destroying 2 edges on the graph
*
*  @param p pointer on the player
*  @param wall the wall we are posing 
*  @return 0 if the wall's installation is successful, -1 if it failed
*/
int put_wall(struct player* p, struct move_t wall); 

/* Destroy a wall on the board meaning adding 2 edges on the graph
*
*  @param p player used
*  @param wall the wall we are posing 
*  @param dir direction used to add edge
*  @return 0 if the wall destruction is successful, -1 if it failed
*/
int destroy_wall(struct player* p, struct move_t wall, int dir); 

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
int existPath_Player(struct player* p, enum color_t color, size_t pos);

/* Check if posing a wall is allowed 
*
*  @param pointer on the player
*  @param specific wall we test
*  @param d1 direction 1 of center vertex
*  @param d2 direction 2 of center vertex
*  @return 1 if posing this wall is allowed, 0 otherwise
*/
int checkPath(struct player* p, struct move_t wall, int dir); 

// =================== WALL =================== 

#endif