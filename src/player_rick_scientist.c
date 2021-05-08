#include <stdlib.h>
#include "utils.h"
#include "strategy.h"
#include "graph_modif.h"

struct player self; 
struct edge_t no_wall = {-1, -1}; 

/* Access to player informations
 * RETURNS:
 * - the player name as an [a-zA-Z0-9 -_]* string
 */
char const* get_player_name()
{
   return "Rick"; 
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
   self = initialization_player(self, id, graph, num_walls); 
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
   *  Player Rick: Dijkstra
   *  Put walls if the opponent is closer
   */

   if (previous_move.t == MOVE) 
   {
      self.ennemy_pos = previous_move.m; 
   }
      // Update le graphe en cas de Wall 
   else if (previous_move.t == WALL)
   {
      int wall_destroyed = put_wall(&self, previous_move); 

      if (wall_destroyed == -1)  fprintf(stderr, "Erreur (Client) - Retirer un mur n'a pas fonctionné\n"); 
   }

   // Creation of the new move 
   struct move_t move = {.c = self.id, .t = MOVE, .e = {(struct edge_t) {-1, -1}, (struct edge_t) {-1, -1}}, .m = -1 }; 
   
   // ==== First move
   if (self.first_move)
   {
      move.t = MOVE; 
      move.e[0] = no_wall;
      move.e[1] = no_wall; 

      size_t* list = malloc(sizeof(size_t) * self.numb_win); 
      graph__list_ownership(self.graph, self.n, self.id, list); 
      move.m = list[self.numb_win/2]; 
      self.pos = move.m;
      self.first_move = 0; 

      // ===== Free tables
      free(list);
      // =====

   }
   // ===== Other moves =====
   else
   {
      
      move = double_dijkstra_strategy(&self);

         // === Chosed to move === 
      if (move.t == MOVE)
      {
         self.pos = move.m;
      }

         // === Chosed to put a wall ===
      else if (move.t == WALL)
      {
         int wall_destroyed = put_wall(&self, move);

         if (wall_destroyed == -1) 
         {
            fprintf(stderr, "Erreur (Client) - Retirer un mur n'a pas fonctionné\n"); 
            exit(2); 
         }

         self.num_walls -= 1;
         //printf("Côté Client : Joueur %d pose mur {%ld-%ld, %ld-%ld}\n", self.id, move.e[0].fr, move.e[0].to, move.e[1].fr, move.e[1].to);
      }
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
   printf("Libération %s ...\n", self.get_name());
   finalization_player(self);
   /*
   free(self.winning_nodes);
   graph__free(self.graph);
   */
   printf("OK !\n");
}
