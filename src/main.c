#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "utils.h"

#define NUMB_PLAYER 2
#define NUMB_WALLS 20
#define SIZE_GRAPH 6
int main()
{
   // ================== Initializing game ==================

      // Get players 
   struct player* player1 = get_functions("./install/libplayer_move_random.so");
   struct player* player2 = get_functions("./install/libplayer_random.so"); 
   struct player* players[2] = {player1, player2}; 

   int random = rand() % 2; 

      // Central Graph - Server 

   int size_graph = SIZE_GRAPH; 
   struct graph_t* server_Graph = graph__create_square(size_graph);
   
      // ===== Initialize players (Server) =====

   for(int p = 0; p < NUMB_PLAYER; p++)
   {
      players[p]->num_walls = NUMB_WALLS; 
      if (p == random)   
      {
         players[p]->id = WHITE;
         players[p]->first_move = 1; 

         // add Owner position
         for(int i=0; i<size_graph; i++)
         {
            graph__add_ownership(server_Graph, server_Graph->num_vertices - size_graph + i, WHITE);
         }
      }
      else 
      {
         players[p]->id = BLACK; 
         players[p]->first_move = 1; 

         // add Owner position
         for(int i=0; i<size_graph; i++)
         {
            graph__add_ownership(server_Graph, i, BLACK);
         }
      }     
   }

      // ====== 

      // ===== Initialize Winning positions 

   size_t* list_p0 = malloc(sizeof(size_t) * size_graph); 
   size_t* list_p1 = malloc(sizeof(size_t) * size_graph); 
   graph__list_ownership(server_Graph, size_graph, other_player(players[0]->id), list_p0); 
   graph__list_ownership(server_Graph, size_graph, other_player(players[1]->id), list_p1);
   size_t* wins_places[2] = {list_p0, list_p1}; 


      // ======

      // ===== Initialize players (Client) ===== 

   struct graph_t* graphs[2] = {graph__copy(server_Graph, size_graph), graph__copy(server_Graph, size_graph)}; 
   for(int p = 0; p < NUMB_PLAYER; p++)
   {
      players[p]->initialize(p, graphs[p], NUMB_WALLS);
   }

      // ======

   // ==================


   

   // ================== Loop Game ==================
   int isPlaying = 1; 
   int loop=0; 

   struct move_t update_move; 
   while (isPlaying) 
   {

      // ===== Players Playing =====

      for (int p=0; p<NUMB_PLAYER; p++)
      {
         
            // === FIRST MOVE ===
         if (loop == 0 && p == 0)   update_move = players[p]->player_play((struct move_t){.t=NO_TYPE}); 
            // === OTHERS MOVE ===
         else
         {
            update_move = players[p]->player_play(update_move);
         }


            // === Update Server ===

         // === Move ===
         if (update_move.t == MOVE)
         {
            players[p]->pos = update_move.m;
            players[other_player(players[p]->id)]->ennemy_pos = update_move.m;
         }
         // === Wall ===
         else if (update_move.t == WALL)
         {  
               // = Tentative de triche = 
            if (players[p]->num_walls <= 0)
            {
               fprintf(stderr, "TRICHE - Tentative de poser un mur alors que plus de mur disponible\n"); 
               exit(3); 
            }
            
            int wall_destroyed = put_wall(server_Graph, update_move); 
            players[p]->num_walls -= 1; 
            if (wall_destroyed == -1)  fprintf(stderr, "Erreur (Server) - Retirer un mur n'a pas fonctionné"); 
         }

         
            // === Victory ===
         for(int i=0; i<size_graph; i++)
         {
            if (players[p]->pos == wins_places[p][i])
            {
               printf("Edge 3-9 existe? %d \n", graph__edge_exists(server_Graph, 3, 9));
               printf("VICTOIRE DU JOUEUR %s - Nombre de tours : %d & Position gagnante : %ld\n", players[p]->get_name(), loop, players[p]->pos); 
               return 1; 
            }
         }
         
            // === Debug ===
         printf("Côté Serveur: Joueur %d (position = %ld / position ennemie = %ld) \n", players[p]->id, players[p]->pos, players[p]->ennemy_pos);
         graph__display(server_Graph, size_graph, player_color(players, WHITE)->pos, player_color(players, BLACK)->pos );
      }
      loop++; 
   }

   // ================== Free elements ==================

      // ===== Free Graphs
   for(int p=0; p<NUMB_PLAYER; p++)
   {
      players[p]->finalize();
   }
   graph__display(server_Graph, size_graph, 0, 0);
   graph__free(server_Graph); 
      // =====

      // ===== Free Winning lists
   free(list_p0);
   free(list_p1); 
      // =====

   // ============================================

   return 1; 
}
