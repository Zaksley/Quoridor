#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "utils.h"

#define NUMB_PLAYER 2


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




int main()
{
   // ================= Initializing game 

   // Get players 
   struct player* player1 = get_functions("./install/libplayer.so");
   struct player* player2 = get_functions("./install/libplayer.so"); 
   struct player* players[2] = {player1, player2}; 

   int random = rand() % 2; 
   struct player* start = players[random]; 

   struct graph_t* myGraph = graph__create_square(3);
   graph__add_ownership(myGraph, 4, WHITE);

   // ==================
   for(int p = 0; p < NUMB_PLAYER; p++)
   {
      players[p]->initialize(p, myGraph, 5);
      printf("%s\n", players[p]->get_name());
   }

   int isPlaying = 1; 
   int loop=0; 

   while (isPlaying) 
   {
      if (loop >= 2) break; 
      for (int p=0; p<NUMB_PLAYER; p++)
      {
         players[p]->player_play((struct move_t){});
      }
      loop++; 
   }
    
   return 1; 
}
