#ifndef _QUOR_UTILS_H_
#define _QUOR_UTILS_H_

#include "player.h"

/*
 *   Contains every special function of a player
 */
struct player
{
   size_t pos; 
   size_t ennemy_pos; 
   enum color_t id;

   struct graph_t * graph;
   size_t n; 
   size_t num_walls;


   char* (*get_name)();
   void (*initialize) (enum color_t, struct graph_t*, size_t);
   struct move_t (*player_play) (struct move_t);
   void (*finalize) (); 
}; 

struct player* get_functions(char* lib);




// ******************* Functions with MOVE = 1 (Moving pawn)

/* Return a list of every valid position to move
*
*  @param p struct player who has to move 
*  @return list of move_t
*/

struct move_t* valid_positions(struct player*); 

/*
*  Add a position new to the array valid 
*/
void add_position(struct move_t* valid, struct move_t new, int* count_moves); 




// ******************* Functions with MOVE = 0 (Posing wall)

/*
*  @param pointer on the player
*  @return array of every move_t wall usable
*/
struct move_t* valid_walls(struct player* p); 

/* Put a wall on the board meaning destroying 2 edges on the graph
*
*  @param pointer on the player
*  @param wall the wall we are posing 
*  @return 0 if the wall's installation is successful, -1 if it failed
*/
int put_wall(struct player* p, struct move_t wall); 

/* Shifting by the left an array
*  
*  @param s the array
*  @param begin where we start
*  @param end where we stop
*/
void shift_left(size_t s[], int begin, int end); 

/* Checking if a player is still able to move to the victory if we put a specific wall
*
*  @param pointer on the player
*  @param Specific color of studied player
*  @param Where the studied player is
*  @return 1 if player can still win, 0 otherwise
*/
int existPath_Player(struct player* p, size_t number_player, size_t pos_player);

/* Check if posing a wall is allowed 
*
*  @param pointer on the player
*  @param specific wall we test
*  @return 1 if posing this wall is allowed, 0 otherwise
*/
int checkPath(struct player* p, struct move_t wall); 

#endif