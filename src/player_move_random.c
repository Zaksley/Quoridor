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
   return "Move ONLY - Random"; 
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
   self.graph = graph__copy(graph, 3);
   self.pos = 4;
   self.n = 3;
   //self.ennemy_pos??! 
}

/* Computes next move
* PARAM:
* - previous_move: the move from the previous player
* RETURNS:
* - the next move for the player.
*/
struct move_t play(struct move_t previous_move)
{
   /*
   * Player: Only move - Random move
   */
 
   // TODO Update Move ennemy 
   // self.ennemy_pos   

   struct move_t* moves = valid_positions(&self);
   for(int i = 0; i < 5; ++i)
   {
      //printf("move %d : -> %zu\n", i, moves[i].m);
   }

   // Creation of the new move 
   struct move_t move; 
   move.c = self.id; 
   move.t = MOVE; 
   move.e[0] = no_wall;
   move.e[1] = no_wall; 
   move.m = moves[rand() % 4].m; 
   printf("MOVE CHOISI %ld pour joueur %d\n", move.m, self.id);
   self.pos = move.m;

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
