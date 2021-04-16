#include <stdlib.h>
#include "utils.h"
#include "graph_modif.h"

/* struct data */
/* { */
/*    enum color_t id; */
/*    struct graph_t *graph; */
/*    size_t num_walls; */
/* }; */

struct player self; 
struct edge_t no_wall = {-1, -1}; 

/* Access to player informations
 * RETURNS:
 * - the player name as an [a-zA-Z0-9 -_]* string
 */
char const* get_player_name()
{
   return "Move & Walls - Random"; 
}

/* Player initialization
 * PARAM:
 * - id        : the color assigned to the player
 * - graph     : the graph where the game is played
 * - num_walls : the number of walls assigned to the player
 * PRECOND:
 * - `id` is either BLACK or WHITE
 * - `graph` is a heap-allocated copy of the graph where the game is
 *   played, that must be freed in the end
 * - `num_walls` is the number of edges of `graph` divided by 15,
     rounded up
 * - initialize has never been called before
 */
void initialize(enum color_t id, struct graph_t* graph, size_t num_walls)
{
   self.id = id;
   self.num_walls = num_walls;
   self.n = 5;
   self.graph = graph; 

   if (self.id == WHITE)
   {
      self.pos = 0;
      self.ennemy_pos = 20;
   }
   else
   {
      self.pos = 20;
      self.ennemy_pos = 0;
   }
   
}

/* Computes next move
* PARAM:
* - previous_move: the move from the previous player
* RETURNS:
* - the next move for the player.
*/
struct move_t play(struct move_t previous_move)
{
   (void) previous_move; 


   // Creation of the new move 
   struct move_t move; 
   move.c = self.id; 
   move.t = MOVE; 
   move.e[0] = no_wall;
   move.e[1] = no_wall; 

   // ==== First move
   if (self.first_move)
   {
      size_t* list = malloc(sizeof(size_t) * self.n); 
      graph__list_ownership(self.graph, self.n, self.id, list); 
      move.m = list[rand() % self.n]; 

      // ===== Free tables
      free(list);
      // =====
   }
   // ==== Other moves
   else
   {
      struct moves* moves = valid_positions(&self);
      move.m = moves->valid[rand() % moves->number_moves].m; 
      printf("MOVE CHOISI %ld pour joueur %d\n", move.m, self.id);
      self.pos = move.m;

      // ===== Free tables
      free(moves->valid);
      free(moves);
      // =====
   }

   return move;  
}

/* Announces the end of the game to the player, and cleans up the
   memory he may have been using.
 * POSTCOND:
 * - every allocation done during the calls to initialize and play
 *   functions must have been freed
 */
void finalize()
{
   printf("Libération de Random...\n");
   graph__free(self.graph);
   printf("OK !\n");
}
