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
   return "Smart Usain Bolt"; 
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
   self.n = graph__get_size(graph);
   self.graph = graph__copy(graph, self.n);
   self.naked_graph = graph__copy(self.graph, self.n); 
   self.first_move = 1; 

   self.wall_installed = calloc(self.n*self.n, sizeof(int)); 

   self.numb_win = graph__count_ownership(self.graph, self.n, self.id); 
   self.winning_nodes = malloc(sizeof(size_t) * self.numb_win);
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
   *  Player Usain Bolt smart: Rush mid (if possible) 
   *  Probas to put walls IN FRONT of player
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
   struct move_t move; 
   move.c = self.id; 

   // ==== First move
   if (self.first_move)
   {
      move.t = MOVE; 
      move.e[0] = no_wall;
      move.e[1] = no_wall; 

      size_t* list = malloc(sizeof(size_t) * self.n); 
      graph__list_ownership(self.graph, self.n, self.id, list); 
      move.m = list[rand() % self.n]; 
      self.pos = move.m;
      self.first_move = 0; 

      // ===== Free tables
      free(list);
      // =====

   }
   // ===== Other moves =====
   else
   {
      int random = rand() % 2; 
      struct moves_valids* moves = valid_walls(&self);

      // === Chosed to move === 
      if (random == 0 || self.num_walls <= 0 || moves->number <= 0 )
      {
         move.t = MOVE; 
         move.e[0] = no_wall;
         move.e[1] = no_wall; 

         moves = valid_positions(&self);
         if (moves->number > 0)
         {
            move.m = rushing_path(self.pos, self.winning_nodes, self.numb_win, moves);
            self.pos = move.m;
         }
         else
         {
            fprintf(stderr, "0 move trouvé - BUG");
            exit(1);
         }
      }
      // === Chosed to put a wall ===
      else
      {
         move.t = WALL; 
         struct move_t chosen_wall = moves->valid[rand() % moves->number]; 
         move.e[0] = chosen_wall.e[0]; 
         move.e[1] = chosen_wall.e[1];
         move.m = chosen_wall.m; 
         
         int wall_destroyed = put_wall(&self, chosen_wall); 
         self.num_walls -= 1; 

         if (wall_destroyed == -1) 
         {
            fprintf(stderr, "Erreur (Client) - Retirer un mur n'a pas fonctionné\n"); 
            exit(2); 
         }
         //printf("Côté Client : Joueur %d pose mur entre %ld et %ld\n", self.id, move.e[0].fr, move.e[0].to);
      }

      // ===== Free tables
      free(moves->valid);
      free(moves);
      // =====
   }

   //printf("Côté Client : Joueur %d (position = %ld, position ennemie = %ld) \n", self.id, self.pos, self.ennemy_pos);

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
   free(self.winning_nodes);
   graph__free(self.graph);
   printf("OK !\n");
}
