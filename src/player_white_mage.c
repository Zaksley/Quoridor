#include <stdlib.h>
#include "utils.h"
#include "graph_modif.h"

struct player self; 
struct edge_t no_wall = {-1, -1}; 

/* Access to player informations
 * RETURNS:
 * - the player name as an [a-zA-Z0-9 -_]* string
 */
char const* get_player_name()
{
   return "Benediction"; 
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
   self.first_move = 1; 

   self.numb_win = graph__count_ownership(self.graph, self.n, self.id); 
   self.winning_nodes = malloc(sizeof(size_t) * self.numb_win);
}

/* Computes next move
* PARAM:
* - previous_move: the move from the previous player
* RETURNS:
* - the next move for the player.
*/
/*
STRATEGY :
Modified quick box
First : create a dijkstra trap
Then lure the other player into the trap
When the player is about to win, trigger the trap and start running

Final trap looks like this :
o o o o o o
---
o o|o o o o
   |
o o|o o o o
    ---
o o o o|o o
       |
o o o o|o o

o o o X o o

Si le piège n'est pas activé (0)
   Si le joueur vient d'avancer
      Trouver le bord le plus proche
      Si le mur noir est plaçable
         Activer le piège
         Placer le mur noir
      Sinon
         S'il est déjà placé (cas 1)
            Activer le piège
            Se rapprocher de l'arrivée
         S'il y a un mur en croix (cas 2)
            Si ce mur touche le bord
               Rendre inactivable le piège
               Placer le mur dans la continuité de ce mur
            Sinon
               Activer le piège
               Placer le mur noir en le décalant d'une case vers le côté proche
         S'il y a un mur chevauchant les positions gagnantes (cas 3)
            On essaie de décaler le mur noir d'une case vers le côté proche
            Si c'est possible
               Poser le mur noir
               Activer le piège
            Sinon
               Rendre inactivable le piège
               Placer un mur perpandiculairement pour créer un coude
         Si le mur est l'équivalent du mur noir avancé d'une case (cas 4)
            Activer le piège
            Se rapprocher de l'arrivée
   Sinon
      S'il s'est décalé sur un côté (angoisse)
         Poser un mur pour protéger les positions gagnantes en face de la sortie du piège
      S'il vient de poser un mur
         Si on est en avance
            Poser un mur pour se bloquer nous-même
         Sinon
            Si en se déplaçant on est en avance
               Poser un mur pour se bloquer nous-même
            Sinon
               Avancer

Si le piège ne pourra pas être activé (2)
   STRATEGIE D'E NON-PIEGE
   Stratégie anti-bolt, mettre des murs dans la continuité
   // OPTIMISABLE

Si le piège est activé (donc est ouvert) (1)
   Si un mur peut être posé pour forcer l'ouverture du piège
      Si on peut placer un mur pour fermer le piège
         Placer un mur pour fermer le piège
         Fermer le piège
      Sinon
         Si on peut placer un mur pour rallonger le couloir
            Placer un mur pour rallonger le couloir
         Sinon
            Avancer vers l'arrivée
            /// Il y a mieux, mais le cas est maudit, à optimiser
   Sinon
      Si on est en phase horizontale
         Si le joueur est bloqué dans le couloir
            Avancer vers l'arrivée (on a le temps)
         Sinon
            Si la taille du couloir est > 2
               Placer un mur pour réduire la taille du couloir
               (Pas besoin de revérifier le forçage d'ouverture : sinon l'autre s'est bloqué seul)
            Si la taille du couloir est 2
               Passer en phase verticale
               On imagine augmenter la profondeur du couloir en plaçant un mur
               Si ce mur est plaçable
                  Si cette action referme le piège
                     Poser le mur
                     Fermer le piège
                  Sinon
                     Poser un mur pour augmenter la profondeur du couloir
               Sinon
                  Si un mur "avant verticalement" existe
                     /// On continuera le couloir à partir de lui
                     Avancer vers l'arrivée (on a le temps)
                  Si un mur "après verticalement" existe
                     Restreindre la sortie disponible par un mur
                     Demander le cas spécifique 4 (sortie à bloquer)
                     (Au tour suivant, la bloquer définitivement par un autre mur)
            Si la taille du couloir est 1
               Passer en phase verticale
               On imagine augmenter la profondeur du couloir en plaçant un mur
               Si cette action referme le piège
                  ??
               Sinon
                  Poser un mur pour augmenter la profondeur du couloir
            Si la taille du couloir est 0 (l'autre s'est bloqué seul)
               Fermer le piège
               Avancer vers l'arrivée
      Si on est en phase verticale
         Si le joueur est bloqué dans le couloir
            Avancer vers l'arrivée (on a le temps)
         Sinon
            On imagine augmenter la profondeur du couloir en plaçant un mur
            Si cette action referme le piège
               Placer un mur pour fermer le piège
               Fermer le piège
            Sinon
               Poser un mur pour augmenter la profondeur du couloir

Si le piège a été fermé (3)
   STRATEGIE D'APRES-PIEGE
   On se précipite vers l'arrivée

Sinon (autres cas spécifiques)
   S'il y a une sortie à bloquer (4)
      Bloquer la sortie indiquée

*/
/*------*/
int _prev_move_is_wall = 0;
int _turn_count = 0;
int _ennemy_last_dir = ERROR_DIRECTION;
int _forward = ERROR_DIRECTION;
int _backward = ERROR_DIRECTION;
int _trap_setup = 0;
/*------*/
struct move_t play(struct move_t previous_move)
{

   // DO NOT MODIFY ----------------------------------------------
   // ENNEMY HAS MOVED
   if (previous_move.t == MOVE) 
   {
      _ennemy_last_dir = graph__get_dir(self.graph, self.ennemy_pos, previous_move.m);
      self.ennemy_pos = previous_move.m;
      _prev_move_is_wall = 0; 
   }
   // ENNEMY HAS PUT A WALL
   else if (previous_move.t == WALL)
   {
      int wall_destroyed = put_wall(self.graph, previous_move); 
      if (wall_destroyed == -1)  fprintf(stderr, "Erreur (Client) - Retirer un mur n'a pas fonctionné\n"); 
      else _prev_move_is_wall = 1;
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
      // Start at the center
      move.m = list[self.n / 2]; 
      self.pos = move.m;
      self.first_move = 0;

      // Player forward/backward directions
      if(self.id == 0) 
      {
         _forward = NORTH;
         _backward = SOUTH; 
      } 
      else 
      {
         _forward = SOUTH;
         _backward = NORTH;
      }

      // ===== Free tables
      free(list);
      // =====

   }
   // ------------------------------------------------------------


   // ===== Other moves =====
   else 
   {
      _turn_count++; 
      struct moves_valids* moves = valid_walls(&self);

      // FIRST MOVE FORWARD OF THE ENNEMY
      if(_ennemy_last_dir == _backward && _trap_setup == 0){

         _trap_setup = 1;
         
         // Finds the right wall for the trap
         int nearest = SOUTH;


         if(self.id == WHITE){ // The other player is upside
            // Finds the nearest side of the graph

         }else{
            // The other player is downside
         }

         // Is the wall possible ?
         if(prev_move_is_wall == 1)
         {
            ;;
         }

         // Places the wall
      }

      // // === Chosed to move === 
      // if (random == 0 || self.num_walls <= 0 || moves->number <= 0 )
      // {
      //    move.t = MOVE; 
      //    move.e[0] = no_wall;
      //    move.e[1] = no_wall; 

      //    moves = valid_positions(&self);
      //    if (moves->number > 0)
      //    {
      //       move.m = rushing_path(self.pos, self.winning_nodes, self.numb_win, moves);
      //       self.pos = move.m;
      //    }
      //    else
      //    {
      //       fprintf(stderr, "0 move trouvé - BUG");
      //       exit(1);
      //    }
      // }
      // // === Chosed to put a wall ===
      // else
      // {
      //    move.t = WALL; 
      //    struct move_t chosen_wall = moves->valid[rand() % moves->number]; 
      //    move.e[0] = chosen_wall.e[0]; 
      //    move.e[1] = chosen_wall.e[1];
      //    move.m = chosen_wall.m; 
         
      //    int wall_destroyed = put_wall(self.graph, chosen_wall); 
      //    self.num_walls -= 1; 
      //    if (wall_destroyed == -1) 
      //    {
      //       fprintf(stderr, "Erreur (Client) - Retirer un mur n'a pas fonctionné\n"); 
      //       exit(2); 
      //    }
      //    //printf("Côté Client : Joueur %d pose mur entre %ld et %ld\n", self.id, move.e[0].fr, move.e[0].to);
      // }

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
