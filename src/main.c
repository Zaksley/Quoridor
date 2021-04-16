#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "utils.h"

#define NUMB_PLAYER 2
#define NUMB_WALLS 5

enum color_t other_player(enum color_t player)
{
   if (player == WHITE) return BLACK; 
   else return WHITE; 
}

struct player* player_color(struct player** p, enum color_t c)
{
   if (p[0]->id == c) return p[0];
   else return p[1]; 
}

int main()
{
   // ================= Initializing game 

      // Get players 
   struct player* player1 = get_functions("./install/libplayer_move_random.so");
   struct player* player2 = get_functions("./install/libplayer_random.so"); 
   struct player* players[2] = {player1, player2}; 

   int random = rand() % 2; 

      // Central Graph - Server 
   int size_graph = 5; 
   struct graph_t* server_Graph = graph__create_square(size_graph);
   
    // ===== Initialize players (Server) =====
   for(int p = 0; p < NUMB_PLAYER; p++)
   {
        
      if (p == random)   
      {
         players[p]->id = WHITE;
         players[p]->pos = 0; 
         players[p]->ennemy_pos = 20; 

         // Winning positions add 
         for(int i=0; i<size_graph; i++)
         {
            graph__add_ownership(server_Graph, i, players[p]->id);
         }
      }
      else 
      {
         players[p]->id = BLACK; 
         players[p]->pos = 20; 
         players[p]->ennemy_pos = 0; 

         // Winning positions add 
         for(int i=0; i<size_graph; i++)
         {
            graph__add_ownership(server_Graph, server_Graph->num_vertices - size_graph + i, players[p]->id);
         }
      }     
   }


   struct graph_t* graphs[2] = {graph__copy(server_Graph, size_graph), graph__copy(server_Graph, size_graph)}; 

      // ===== Initialize players (Client) ===== 
   for(int p = 0; p < NUMB_PLAYER; p++)
   {
      players[p]->initialize(p, graphs[p], NUMB_WALLS);
   }



   // ================== Loop Game
   int isPlaying = 1; 
   int loop=0; 

   while (isPlaying) 
   {
      if (loop >= 6) break; 
      for (int p=0; p<NUMB_PLAYER; p++)
      {
         struct move_t update_move = players[p]->player_play((struct move_t){});

            // Update Player_move 
         if (update_move.t == MOVE)
         {
            players[p]->pos = update_move.m;
            players[other_player(players[p]->id)]->ennemy_pos = update_move.m;
         }
         printf("Joueur %d : Sa position = %ld / Position ennemie = %ld\n", players[p]->id, players[p]->pos, players[p]->ennemy_pos);
         
         //graph__display(server_Graph, size_graph, player_color(players, WHITE)->pos, player_color(players, BLACK)->pos );
      }
      loop++; 
   }

   // ===== Free Graphs
   for(int p=0; p<NUMB_PLAYER; p++)
   {
      players[p]->finalize();
   }
   graph__display(server_Graph, size_graph, 0, 0);
   graph__free(server_Graph); 
   // =====
   return 1; 
}
