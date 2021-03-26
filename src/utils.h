#ifndef _QUOR_UTILS_H_
#define _QUOR_UTILS_H_

#include "player.h"

/*
 *   Contains every special function of a player
 */
struct player
{
   size_t pos; 
   enum color_t id;
   struct graph_t * graph;
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





// *******************

#endif