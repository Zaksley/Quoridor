#include "stdlib.h"
#include "utils.h"

void add_position(struct move_t* valid, struct move_t new, int* count_moves)
{
   valid[*count_moves] = new;
   *count_moves++; 
}

// TODO => vérifier que sur les positions, il n'y a pas de joueur adverse
struct move_t* valid_positions(struct player* p)
{
   struct move_t valid[4] = {};
   struct move_t new; 
   int count_moves = 0; 
   
   int value = -1;

      // Definitions move
   new.c = p->id; 
   new.t = MOVE; 
   struct edge_t e = {-1, -1};
   new.e[0] = e;
   new.e[1] = e;  

   for(int dir = 1; dir < 5; dir++)
   {
      value = graph__get_neighboor(p->graph, p->pos, dir);
      new.m = value; 
      if (value) add_position(valid, new, count_moves); 
   }

   return valid; 

}