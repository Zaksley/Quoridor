#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "utils.h"

#define NUMB_PLAYER 2
#define NUMB_WALLS 5

/*
 * Get functions from a special player with a dynamic lib.so and stock it in a new struct
 * 
 * @param lib path to the player library 
 * @precond lib is a valid path to a dynamic library
 * @returns a player with initialized functions
 */
struct player* get_functions(char* lib)
{
   // Initialization 
   struct player* player = malloc(sizeof(struct player)); 

   // Get functions
   char* (*get_name)(); 
   void (*initialize) (enum color_t, struct graph_t*, size_t);
   struct move_t (*player_play) (struct move_t); 
   void (*finalize) (); 

   // Open the lib 
   void * handle;
   handle = dlopen(lib, RTLD_LAZY);

   if (!handle) {
      fprintf(stderr, "%s\n", dlerror());
      exit(EXIT_FAILURE);
   }

   dlerror(); // Clear any existing error 

   // Putting functions in the struct 
   *(void **) (&get_name) = dlsym(handle, "get_player_name");
   *(void **) (&player_play) = dlsym(handle, "play"); 
   *(void **) (&initialize) = dlsym(handle, "initialize"); 
   *(void **) (&finalize) = dlsym(handle, "finalize"); 

   player->get_name = get_name; 
   player->player_play = player_play;
   player->initialize = initialize;
   player->finalize = finalize;

   // dlclose(handle); - To close at the end ?

   return player; 
}

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
         players[p]->first_move = 1; 

         // Winning positions add 
         for(int i=0; i<size_graph; i++)
         {
            graph__add_ownership(server_Graph, i, players[p]->id);
         }
      }
      else 
      {
         players[p]->id = BLACK; 
         players[p]->first_move = 1; 

         // Winning positions add 
         for(int i=0; i<size_graph; i++)
         {
            graph__add_ownership(server_Graph, server_Graph->num_vertices - size_graph + i, players[p]->id);
         }
      }     
   }

   // ======


   // ===== Initialize players (Client) ===== 

   struct graph_t* graphs[2] = {graph__copy(server_Graph, size_graph), graph__copy(server_Graph, size_graph)}; 
   for(int p = 0; p < NUMB_PLAYER; p++)
   {
      players[p]->initialize(p, graphs[p], NUMB_WALLS);
   }

   // ======


   // ================== Loop Game
   int isPlaying = 1; 
   int loop=0; 

   struct move_t update_move; 
   while (isPlaying) 
   {
      if (loop >= 6) break; 

      for (int p=0; p<NUMB_PLAYER; p++)
      {

            // FIRST MOVE - No move before to give
         if (loop == 0 && p == 0)
         {
            update_move = players[p]->player_play((struct move_t){}); 
         }
         else
         {
            update_move = players[p]->player_play(update_move);
         }

            // Update Player_move 
         if (update_move.t == MOVE)
         {
            players[p]->pos = update_move.m;
            players[other_player(players[p]->id)]->ennemy_pos = update_move.m;
         }
         
         printf("Côté serveur: Joueur %d (position = %ld / position ennemie = %ld) \n", players[p]->id, players[p]->pos, players[p]->ennemy_pos);
         
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
