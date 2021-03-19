#ifndef _QUOR_UTILS_H_
#define _QUOR_UTILS_H_

#include "player.h"

/*
 *   Contains every special function of a player
 */
struct player
{
   enum color_t id;
   struct graph_t * graph;
   size_t num_walls;
   char* (*get_name)();
   void (*initialize) (enum color_t, struct graph_t*, size_t);
   struct move_t (*player_play) (struct move_t);
   void (*finalize) (); 
}; 

struct player* get_functions(char* lib);


#endif
