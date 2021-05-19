#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <dlfcn.h>
#include "utils.h"

#define NUMB_PLAYER 2
#define NUMB_WALLS 10

#define MAX_SIZE_GRAPH 15

#define DEFAULT_SIZE_GRAPH 10
#define DEFAULT_TYPE_GRAPH 'c'
#define DEFAULT_PLAYER1 "./install/Rick_C137.so"
#define DEFAULT_PLAYER2 "./install/libplayer_rick_scientist.so"

static int size_graph = DEFAULT_SIZE_GRAPH; 
static char type_graph = DEFAULT_TYPE_GRAPH; 
static char* player_one = DEFAULT_PLAYER1;
static char* player_two = DEFAULT_PLAYER2; 

////////////////////////////////////////////////////////////////
// Function for parsing the options of the program
// Currently available options are :
// -m <graph size> : sets the size n of graph
// -t <graph type> : sets the type of graph
// player1 : Decide the lib for the first player
// player2 : Decide the lib for the second player

void parse_opts(int argc, char* argv[])
{
   int opt;
   while ((opt = getopt(argc, argv, "m:t:")) != -1)
   {
      switch (opt)
	   {
         case 'm':
            if (atoi(optarg) > 0 && atoi(optarg) < 15)
            {
               size_graph = atoi(optarg);
            }
            else  size_graph = DEFAULT_SIZE_GRAPH; 
            break;

         case 't':
            if (optarg[0] == 'c' || optarg[0] == 't' || optarg[0] == 'h' || optarg[0] == 's')   
            {
               type_graph = optarg[0]; 
            }
            break; 
         
         default:
            fprintf(stderr, "Usage: %s [-m graph size] \n", argv[0]);
            fprintf(stderr, "Usage: %s [-t graph type] \n", argv[0]);
            fprintf(stderr, "Usage: %s string: player_name", argv[0]); 
            exit(EXIT_FAILURE);
      }
   }

         // Player 1
   if (optind < argc)
   {
      if(   !strcmp(argv[optind], "./install/libplayer_usain_bolt.so")
         || !strcmp(argv[optind], "./install/libplayer_rick_scientist.so")
         || !strcmp(argv[optind], "./install/libplayer_move_random.so")
         || !strcmp(argv[optind], "./install/libplayer_random.so"))
      {
         player_one = argv[optind];
      }
   }
            // Player 2
   if (optind+1 < argc)
   {
      if(   !strcmp(argv[optind+1], "./install/libplayer_usain_bolt.so")
         || !strcmp(argv[optind+1], "./install/libplayer_rick_scientist.so")
         || !strcmp(argv[optind+1], "./install/libplayer_move_random.so")
         || !strcmp(argv[optind+1], "./install/libplayer_random.so")
         )
      {
         player_two = argv[optind+1];
      }
   }

}

////////////////////////////////////////////////////////////////


int main(int argc, char* argv[])
{
   int seed = time(0); 
   //int seed = 1621430369; 
   srand(seed);
   parse_opts(argc, argv); 
   printf("Taille du graphe : %d\n", size_graph);
   printf("Type de graphe : %c\n", type_graph);
   printf("Seed du jeu : %d\n", seed);


   // ================== Initializing Server ==================

      // Get players 
   void* handle_p1; 
   void* handle_p2; 
   
   handle_p1 = dlopen(player_one, RTLD_LAZY);
   handle_p2 = dlopen(player_two, RTLD_LAZY);
   struct player* player1 = get_functions(handle_p1);
   struct player* player2 = get_functions(handle_p2); 
   struct player* players[NUMB_PLAYER]; 

   int random = rand() % 2; 
      // Random selection of who is playing first
   if (random == 0)
   {
      players[0] = player1;
      players[1] = player2;
   }
   else
   {
      players[0] = player2;
      players[1] = player1; 
   }

      // Central Graph - Server 
   struct graph_t* server_Graph;
   switch(type_graph)
   {
      case 'c':
         server_Graph = graph__create_square(size_graph);
         break; 
      case 't':
         server_Graph = graph__create_torus(size_graph);
         break; 
      case 'h':
         server_Graph = graph__create_chopped(size_graph);
         break;
      case 's':
         server_Graph = graph__create_snake(size_graph);
         break; 
      default:
         server_Graph = graph__create_square(size_graph);
         break; 
   }
   

      // ===== Initialize players (Server) =====

   players[0]->id = BLACK;
   players[1]->id = WHITE;
   for(int p = 0; p < NUMB_PLAYER; p++)
   {
      players[p]->num_walls = NUMB_WALLS; 
      players[p]->n = size_graph; 
      players[p]->wall_installed = calloc( (players[p]->n-1)*(players[p]->n-1), sizeof(int)); 
      players[p]->pos = size_graph*size_graph/2; 
      players[p]->ennemy_pos = size_graph*size_graph/2;
      players[p]->first_move = 1; 
   }


      // Add Owner position
   for(int i=0; i<size_graph; i++)
   {
      graph__add_ownership(server_Graph, size_graph*size_graph - size_graph + i, WHITE);
      graph__add_ownership(server_Graph, i, BLACK);
   }

      // === Copy of graphs ===
   struct graph_t* Naked_graph = graph__copy(server_Graph, players[0]->n);
   for(int i=0; i<NUMB_PLAYER; i++)
   {
      players[i]->graph = server_Graph;
      players[i]->naked_graph = Naked_graph; 
   }
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

   players[0]->pos = 0;
   players[1]->pos = 0;
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
   printf("Début de la partie opposant %s et %s!\n\n", players[0]->get_name(), players[1]->get_name()); 
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
               fprintf(stderr, "VICTOIRE DU JOUEUR %s pour cause de tricherie ennemie\n", players[opposite_value(p)]->get_name());
               exit(0); 
            }
            players[p]->pos = update_move.m;
            players[p]->ennemy_pos = 0;
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
               //fprintf(stderr, "Erreur: move %ld pas autorisé \n", update_move.m);
               fprintf(stderr, "Erreur : Joueur %s a tenté de se déplacer sur une case non disponible \n", players[p]->get_name()); 
               fprintf(stderr, "VICTOIRE DU JOUEUR %s pour cause de tricherie ennemie \n", players[opposite_value(p)]->get_name());
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
               int wall_destroyed = put_wall(players[0], update_move); 

               //printf("Côté Serveur : Joueur %d pose mur {%ld-%ld, %ld-%ld \n", players[p]->id, update_move.e[0].fr, update_move.e[0].to, update_move.e[1].fr, update_move.e[1].to);

               players[p]->num_walls -= 1; 

               if (wall_destroyed == -1) 
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
            if (loop != 0 && players[p]->pos == players[p]->winning_nodes[i] && isPlaying)
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
            //printf("Côté Serveur: Joueur %d (position = %ld / position ennemie = %ld) \n", players[p]->id, players[p]->pos, players[p]->ennemy_pos);
            //graph__display(server_Graph, size_graph, player_color(players, WHITE)->pos, player_color(players, BLACK)->pos );
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

   return EXIT_SUCCESS; 
}
