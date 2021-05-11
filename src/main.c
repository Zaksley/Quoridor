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
   void* handle_p1; 
   void* handle_p2; 
   handle_p1 = dlopen("./install/libplayer_rick_scientist.so", RTLD_LAZY);
   handle_p2 = dlopen("./install/libplayer_usain_bolt.so", RTLD_LAZY);
   struct player* player1 = get_functions(handle_p1);
   struct player* player2 = get_functions(handle_p2); 
   struct player* players[2] = {player1, player2}; 

   int random = rand() % 2; 

      // Central Graph - Server 

   int size_graph = SIZE_GRAPH; 
   struct graph_t* server_Graph = graph__create_square(size_graph);
   

      // ===== Initialize players (Server) =====

   for(int p = 0; p < NUMB_PLAYER; p++)
   {
      players[p]->num_walls = NUMB_WALLS; 
      players[p]->n = size_graph; 
      players[p]->wall_installed = calloc( (players[p]->n-1)*(players[p]->n-1), sizeof(int)); 
      players[p]->pos = size_graph*size_graph/2; 
      players[p]->ennemy_pos = size_graph*size_graph/2;

      if (p == random)   
      {
         players[p]->id = WHITE;
         players[p]->first_move = 1; 

         // add Owner position
         for(int i=0; i<size_graph; i++)
         {
            graph__add_ownership(server_Graph, size_graph*size_graph - size_graph + i, WHITE);
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

      // === Copy of graphs ===
   struct graph_t* server_copy = graph__copy(server_Graph, size_graph);
   struct graph_t* Naked_graph = graph__copy(server_Graph, players[0]->n);
   players[0]->graph = server_Graph;
   players[1]->graph = server_copy; 

   for(int i=0; i<NUMB_PLAYER; i++)
      players[i]->naked_graph = Naked_graph; 

      // === Copy of graphs ===


      // ===== Initialize Winning positions 
   int numb_win = graph__count_ownership(server_Graph, size_graph, WHITE);

   size_t* winning_p0 = malloc(sizeof(size_t) * numb_win); 
   size_t* owned_p0 = malloc(sizeof(size_t) * numb_win); 
   graph__list_ownership(server_Graph, size_graph, players[0]->id, owned_p0); 
   graph__list_ownership(server_Graph, size_graph, other_player(players[0]->id), winning_p0);

   players[0]->winning_nodes = winning_p0;
   players[0]->owned_nodes = owned_p0; 
   players[0]->numb_win = numb_win; 
   players[1]->winning_nodes = owned_p0; 
   players[1]->owned_nodes = winning_p0; 
   players[1]->numb_win = numb_win;

      // ======

      // ===== Initialize players (Server) =====



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
         if (loop == 0 && p == 0)   
         {
            update_move = players[p]->player_play((struct move_t){.t=NO_TYPE}); 
         }
            // === OTHERS MOVE ===
         else
         {
            update_move = players[p]->player_play(update_move);
         }

            // = Check cheat first placement =
         if (loop == 0)
         {
            if (!(in_vertexList(players[p]->owned_nodes, players[p]->numb_win, update_move.m)))
            {
               
               fprintf(stderr, "Erreur : Joueur %s a tenté d'apparaître sur une case non disponible au premier tour \n", players[p]->get_name()); 
               fprintf(stderr, "VICTOIRE DU JOUEUR %s pour cause de tricherie ennemie\n", player_color(players, players[p]->id)->get_name());
               exit(0); 
            }
         }

            // === Update Server ===
         // === Move ===
         if (update_move.t == MOVE)
         {
               // = Check cheat =  
            struct moves_valids* moves = valid_positions(players[p]);
            if (move_in_list(moves, update_move) || loop == 0)
            {
                  // = Update both players =
               players[p]->pos = update_move.m;
               players[other_player(players[p]->id)]->ennemy_pos = update_move.m;
            }
            else
            {
               fprintf(stderr, "Erreur : Joueur %s a tenté de se déplacer sur une case non disponible \n", players[p]->get_name()); 
               fprintf(stderr, "VICTOIRE DU JOUEUR %s pour cause de tricherie ennemie \n", player_color(players, players[p]->id)->get_name());
               exit(0); 
            }

            free(moves->valid);
            free(moves);
         }
         // ============

         // === Wall ===
         else if (update_move.t == WALL)
         {  
               // = Check cheat = 
            if (players[p]->num_walls <= 0)
            {
               fprintf(stderr, "TRICHE - Tentative de poser un mur alors que plus de mur disponible\n"); 
               exit(0); 
            }

            struct moves_valids* walls = valid_walls(players[p]);
            if (wall_in_list(walls->number, walls->valid, update_move))
            {
                  // = Update both graphs =
               int wall_destroyed_1 = put_wall(players[0], update_move); 
               int wall_destroyed_2 = put_wall(players[1], update_move);

               //printf("Côté Serveur : Joueur %d pose mur {%ld-%ld, %ld-%ld \n", players[p]->id, update_move.e[0].fr, update_move.e[0].to, update_move.e[1].fr, update_move.e[1].to);

               players[p]->num_walls -= 1; 
               if (wall_destroyed_1 == -1 || wall_destroyed_2 == -1)  
               {
                  fprintf(stderr, "Erreur (Server) - Poser un mur n'a pas fonctionné"); 
                  exit(1);
               }
            }
            else
            {
               fprintf(stderr, "Erreur : Joueur %s a tenté de poser un mur non disponible \n", players[p]->get_name()); 
               fprintf(stderr, "VICTOIRE DU JOUEUR %s pour cause de tricherie ennemie", player_color(players, players[p]->id)->get_name());
               exit(0); 
            }

            free(walls->valid);
            free(walls);
         }
         // ============
         
            // === Victory ===
         for(int i=0; i<players[p]->numb_win; i++)
         {
            if (loop != 0 && players[p]->pos == players[p]->winning_nodes[i])
            {
               printf("VICTOIRE DU JOUEUR %s - Nombre de tours : %d & Position gagnante : %ld\n", players[p]->get_name(), loop, players[p]->pos); 
               graph__display(server_Graph, size_graph, player_color(players, WHITE)->pos, player_color(players, BLACK)->pos );
               isPlaying = 0; 
               break;
            }
         }
         
            // === Debug ===
         if (isPlaying)
         {
            printf("Côté Serveur: Joueur %d (position = %ld / position ennemie = %ld) \n", players[p]->id, players[p]->pos, players[p]->ennemy_pos);
            graph__display(server_Graph, size_graph, player_color(players, WHITE)->pos, player_color(players, BLACK)->pos );
         }

      }
      loop++; 
   }

   // ================== Free elements ==================
   

      // ======= Free Clients ======= 
   for(int p=0; p<NUMB_PLAYER; p++)
   {
      players[p]->finalize();
   }  
      // ======= Free Clients ======= 


      // ======= Free Server ======= 

   // ==== Free Winning lists
   free(players[0]->winning_nodes);
   free(players[0]->owned_nodes);
   // ====

   // ==== Free Graphs
   free(players[0]->wall_installed);
   free(players[1]->wall_installed);
   graph__free(players[0]->graph);
   graph__free(players[1]->graph);
   graph__free(players[0]->naked_graph);
   // ====

   free(players[0]);
   free(players[1]); 

   // ==== Close libs
   dlclose(handle_p1);
   dlclose(handle_p2);
   // ====

      // ======= Free Server ======= 

   // ============================================

   return 1; 
}
