#include <stdlib.h>
#include <dlfcn.h>
#include "utils.h"

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


// -------- MOVE

// TODO => vérifier que sur les positions, il n'y a pas de joueur adverse
struct moves* valid_positions(struct player* p)
{
   struct moves *global = malloc(sizeof(struct moves)); 
   struct move_t *valid = malloc(sizeof(struct move_t) * 5);
   struct move_t new; 

   int count_moves = 0;
   int value = -1;

      // Definitions move
   new.c = p->id; 
   new.t = MOVE; 
   struct edge_t e = {-1, -1};
   new.e[0] = e;
   new.e[1] = e;  

   for(int dir = 1; dir < 5; dir++)
   {
      value = graph__get_neighboor(p->graph, p->n, p->pos, dir);
      new.m = value; 

         // Authorized move
      if (value != -1)
      { 
            // Legal move 
         if (value != (int) p->ennemy_pos)
         {
            valid[count_moves] = new;
            count_moves++; 
         }
            // On ennemy position 
         else
         {
            int double_jump = graph__get_neighboor(p->graph, p->n, value, dir);
            new.m = double_jump; 
               // Legal move
            if (double_jump != -1)
            {
               valid[count_moves] = new;
               count_moves++; 
            }
               // Have to move on side 
            else
            {  
               enum direction d1, d2; 
               if (dir == NORTH || dir == SOUTH) 
               {
                  d1 = EAST; 
                  d2 = WEST; 
               }
               else
               {
                  d1 = NORTH;
                  d2 = SOUTH; 
               }

               int jump_side = graph__get_neighboor(p->graph, p->n, value, d1);
               new.m = jump_side; 
               valid[count_moves] = new;
               count_moves++; 

               jump_side = graph__get_neighboor(p->graph, p->n, value, d2);
               new.m = jump_side;
               valid[count_moves] = new; 
               count_moves++; 
            }

         }
      }
   }

   global->number_moves = count_moves;
   global->valid = valid; 

   return global;
}

// -------- MOVE



// -------- WALL

/* Return every possible wall usable on the board 
*
*  @param p pointer on the player
*  @return array move_t 
*/
struct walls* valid_walls(struct player* p)
{
      // Dynamic array Walls
   struct walls* global = malloc(sizeof(struct walls)); 
   struct move_t* walls = malloc(sizeof(struct move_t) * 1); 
   int size = 0; 
   int capacity = 1; 

   if (global == NULL) exit(0); 
   if (walls == NULL) exit(0); 

      // Setup Wall 
   struct move_t wall; 
   wall.t = WALL; 
   wall.c = p->id; 


   size_t n1, n2; 
   size_t checkTest = -1; 
   for(size_t node = 0; node < p->graph->num_vertices; node++)
   {
      for(int i=0; i<2; i++)
      {
            // Neighboors EAST-WEST
         if (i == 0)
         {
            n1 = graph__get_neighboor(p->graph, p->n, node, EAST); 
            n2 = graph__get_neighboor(p->graph, p->n, node, WEST); 
         }
            // Neighboors NORTH-SOUTH
         else
         {
            n1 = graph__get_neighboor(p->graph,p->n, node, NORTH); 
            n2 = graph__get_neighboor(p->graph,p->n, node, SOUTH); 
         }

         if (n1 != checkTest && n2 != checkTest)
         {
            if (graph__edge_exists(p->graph, node, n1) && graph__edge_exists(p->graph, node, n2))
            {
                  // Set the struct move_t wall 
               struct edge_t e1 = {node, n1};
               struct edge_t e2 = {node, n2}; 
               wall.m = node;
               wall.e[0] = e1;
               wall.e[1] = e2; 

                  // Check if we can put the wall
               int pathOk = checkPath(p, wall);
               if (pathOk)
               {
                     // Realloc size to Walls 
                  if (size == capacity)
                  {
                     capacity *= 2; 
                     walls = realloc(walls, sizeof(struct move_t) * capacity);
                  }

                  walls[size] = wall; 
                  size++; 
               }
            }
         }
      }
   }

   global->valid = walls;
   global->number_walls = size; 

   return global;   
}


/* Put a wall on the board meaning destroying 2 edges on the graph
*
*  @param p pointer on the player
*  @param wall the wall we are posing 
*  @return 0 if the wall's installation is successful, -1 if it failed
*/
int put_wall(struct graph_t* graph, struct move_t wall)
{
      // Remove edges
   int removed;
   removed = graph__remove_edge(graph, wall.e[0].fr, wall.e[0].to); 
   if (removed == -1) return -1;
   removed = graph__remove_edge(graph, wall.e[1].fr, wall.e[1].to);
   if (removed == -1) return -1;

   return 0; 
}

/* Shifting by the left an array
*  
*  @param s the array
*  @param begin where we start
*  @param end where we stop
*/
void shift_left(size_t s[], int begin, int end)
{
   int i=0;

   while(i <= end-begin )
   { 
      s[begin+i-1] = s[begin+i];
      i++;
   }  
}

/* Checking if a player is still able to move to the victory if we put a specific wall
*
*  @param pointer on the player
*  @param Specific color of studied player
*  @param Where the studied player is
*  @return 1 if player can still win, 0 instead
*/
int existPath_Player(struct player* p, size_t number_player, size_t pos_player)
{
   int* marked = calloc(p->graph->num_vertices, sizeof(int)); 
   size_t* waitingList = malloc(p->graph->num_vertices * sizeof(size_t)); 

   size_t current = pos_player; 
   int size = 0; 
   waitingList[size] = current;
   size++; 

   size_t neighboor = -1; 
   
   while (size != 0)
   {  
      current = waitingList[0]; 
      marked[current] = 1; 
      shift_left(waitingList, size-1, 0); 
      size--;

      for(int dir = 0; dir < 5; dir++)
      {
            // If neighboor exist + non treated 
         neighboor = graph__get_neighboor(p->graph, p->n, current, dir);
         if ( neighboor != (size_t) -1 && marked[neighboor] != 0)
         {
            waitingList[size] = graph__get_neighboor(p->graph, p->n, current, dir); 
            size++;
         }
      }
   }

   for(size_t node = 0; node < p->graph->num_vertices; node++)
   {
         // Find a winning path 
      if (marked[node] && (int) gsl_spmatrix_uint_get(p->graph->o, number_player, node) == 1)
      {
         return 1; 
      }
   }

   return 0; 

}

/* Check if posing a wall is allowed 
*
*  @param pointer on the player
*  @param specific wall we test
*  @return 1 if posing this wall is allowed, 0 if not and -1 if errors
*/
int checkPath(struct player* p, struct move_t wall)
{
      // Put Fake Wall (for test)
   put_wall(p->graph, wall); 

   int check_1 = existPath_Player(p, p->pos, p->id);
   int check_2; 
   if (p->id == BLACK) check_2 = existPath_Player(p, p->ennemy_pos, WHITE);
   else existPath_Player(p, p->ennemy_pos, BLACK);
   

      // Remove testing Wall
   int test = -1; 
   test = graph__remove_edge(p->graph, wall.e[0].fr, wall.e[0].to);
   if (test == -1) return -1; 
   test = graph__remove_edge(p->graph, wall.e[1].fr, wall.e[1].to);
   if (test == -1) return -1; 

   if (check_1 && check_2) return 1;
   return 0; 

}




// -------- WALL


