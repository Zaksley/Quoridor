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
/*
*
*
*/
struct player* initialize_test_player(struct graph_t* graph, size_t n, size_t pos, size_t ennemy_pos)
{
	struct player* test_player = get_functions("./install/libplayer_move_random.so");
	test_player->pos = pos;
	test_player->ennemy_pos = ennemy_pos; 
   test_player->graph = graph;
	test_player->n = n;
	test_player->naked_graph = graph__copy(test_player->graph, test_player->n);
	test_player->wall_installed = calloc( (test_player->n-1)*(test_player->n-1), sizeof(int)); 
	return test_player;
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

int edge_equal(struct edge_t e1, struct edge_t e2)
{
   if (e1.fr == e2.fr && e1.to == e2.to)  return 1; 
   else if (e1.fr == e2.to && e1.to == e2.fr)   return 1; 

   return 0; 
}

int opposite_dir(int dir)
{
   switch(dir)
   {
         // North => South
      case 1:
         return 2; 
         break; 

         // South => North
      case 2:
         return 1; 
         break;

         // West => East
      case 3:
         return 4; 
         break; 
      
         // East => West
      case 4:
         return 3; 
         break; 
   }   
   
   return 0; 
}

// ===================  MOVE =================== 

struct moves_valids* valid_positions(struct player* p)
{
   struct moves_valids *global = malloc(sizeof(struct moves_valids)); 
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
               if (jump_side != -1)
               {
                  new.m = jump_side; 
                  valid[count_moves] = new;
                  count_moves++; 
               }

               jump_side = graph__get_neighboor(p->graph, p->n, value, d2);
               if (jump_side != -1)
               {
                  new.m = jump_side;
                  valid[count_moves] = new; 
                  count_moves++; 
               }
            }

         }
      }
   }

   global->number = count_moves;
   global->valid = valid; 

   return global;
}

// =================== MOVE =================== 



// =================== WALL =================== 

size_t position_square(size_t left_square, int n)
{
   return (left_square - left_square / n);
}

size_t min_node(size_t node, size_t n_neighboor, size_t n1, size_t n2)
{
         // Get Left square
   size_t left_square = node; 
   size_t nodes[3] = {n_neighboor, n1, n2}; 

   for(int i=0; i<3; i++)
   {
      if (left_square > nodes[i])   left_square = nodes[i]; 
   }

   return left_square;
}

int already_wall_in_square(int wall_used[], size_t n, size_t left_square)
{
   if (wall_used[position_square(left_square, n)] != 0)
   {
      return 1; 
   }

   return 0;
}

/* Check if a wall isn't already in the list
*
*  @param size size of array moves
*  @param moves array stocking the moves
*  @param wall checking a specific wall
*  @return booleen 1 if wall is in the list, 0 otherwise 
*/
int wall_in_list(int size, struct move_t* moves, struct move_t wall)
{
   for(int i=0; i < size; i++)
   {
      if (  edge_equal(wall.e[0], moves[i].e[0]) && edge_equal(wall.e[1], moves[i].e[1])   ) return 1;
      else if (  edge_equal(wall.e[1], moves[i].e[0]) && edge_equal(wall.e[0], moves[i].e[1])   ) return 1;
   }

   return 0; 
}


/* Return every possible wall usable on the board 
*
*  @param p pointer on the player
*  @return array move_t 
*/
struct moves_valids* valid_walls(struct player* p)
{
   
      // Dynamic array Walls
   struct moves_valids* global = malloc(sizeof(struct moves_valids)); 
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
   size_t n_neighboor; 
   int initial_dir, end_dir; 

   size_t checkTest = -1; 
   
   for(size_t node = 0; node < p->graph->num_vertices; node++)
   {
      for(int dir=1; dir<5; dir++)
      {
            // Neighboors EAST-WEST
         if (dir == 3 || dir == 4 )
         {
            initial_dir = 1;
            end_dir = 3; 
         }
            // Neighboors NORTH-SOUTH
         else
         {
            initial_dir = 3; 
            end_dir = 5; 
         }
         
         for(int second_dir = initial_dir; second_dir < end_dir; second_dir++)
         {
                  // Getting 3 others nodes
            n_neighboor = graph__get_neighboor(p->graph, p->n, node, dir);
               // targeted edge already cut
            if (n_neighboor == checkTest) continue;

            n1 = graph__get_neighboor(p->graph, p->n, node, second_dir); 
            n2 = graph__get_neighboor(p->graph, p->n, n_neighboor, second_dir);

               // * All edges on side are cut
            if (n1 == checkTest && n2 == checkTest) 
            {

               n1 = graph__get_neighboor(p->naked_graph, p->n, node, second_dir);
               n2 = graph__get_neighboor(p->naked_graph, p->n, n_neighboor, second_dir);

               if (n1 == checkTest || n2 == checkTest) continue;
               
                  // Wall IN the square
               if (already_wall_in_square(p->wall_installed, p->n, min_node(node, n_neighboor, n1, n2)))  continue; 
               else
               {
                  
                  // We add the wall (cause it's 2 walls on sides)
               }
            }

               // * 1 edge is already cut (First case)
            else if (n2 == checkTest)
            {
               n2 = graph__get_neighboor(p->graph, p->n, n1, dir);
                  // other one edge is cut
               if (n2 == checkTest) continue; 
               else
               {
                  // We add the wall
               }
            }

               // * 1 edge is already cut (Second case)
            else if (n1 == checkTest)
            {
               n1 = graph__get_neighboor(p->graph, p->n, n2, opposite_dir(dir)); 

                  // other one edge is cut
               if (n1 == checkTest) continue; 
               else
               {
                  // We add the wall
               }
               
            }

               // Edges are availables
            if (graph__edge_exists(p->graph, node, n_neighboor) && graph__edge_exists(p->graph, n1, n2))
            {
               
                  // Set the struct move_t wall 
               struct edge_t e1 = {node, n_neighboor};
               struct edge_t e2 = {n1, n2}; 
               wall.m = node;
               wall.e[0] = e1;
               wall.e[1] = e2; 
               
               
               if(!wall_in_list(size, walls, wall))
               {
                  
                  // Check if we can put the wall
                  int pathOk = checkPath(p, wall, dir);
                  
                  if (pathOk)
                  {
                        // Realloc size to Walls 
                     if (size == capacity)
                     {
                        capacity *= 2; 
                        walls = realloc(walls, sizeof(struct move_t) * capacity);
                     }

                     // Add a wall to the array
                     walls[size] = wall; 
                     size++; 
                  }
               }
            }
         }
      }
   }

   global->valid = walls;
   global->number = size; 

   return global;   
}

   // ====== Save old way to put walls ====== 

   /*
   size_t n1, n2; 
   size_t checkTest = -1; 
   int d1, d2; 
   for(size_t node = 0; node < p->graph->num_vertices; node++)
   {
      for(int i=0; i<2; i++)
      {
            // Neighboors EAST-WEST
         if (i == 0)
         {
            n1 = graph__get_neighboor(p->graph, p->n, node, EAST); 
            n2 = graph__get_neighboor(p->graph, p->n, node, WEST); 
            d1 = EAST;
            d2 = WEST;
         }
         
            // Neighboors NORTH-SOUTH
         else
         {
            n1 = graph__get_neighboor(p->graph,p->n, node, NORTH); 
            n2 = graph__get_neighboor(p->graph,p->n, node, SOUTH); 
            d1 = NORTH;
            d2 = SOUTH;
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
               
               int pathOk = checkPath(p, wall, d1, d2);
               

               if (pathOk)
               {
                     // Realloc size to Walls 
                  if (size == capacity)
                  {
                     capacity *= 2; 
                     walls = realloc(walls, sizeof(struct move_t) * capacity);
                  }

                  // Add a wall to the array
                  walls[size] = wall; 
                  size++; 
               }
            }
         }
      }
      */

     // ====== Save old way to put walls ====== 


/* Put a wall on the board meaning destroying 2 edges on the graph
*
*  @param p pointer on the player
*  @param wall the wall we are posing 
*  @return 1 if the wall installation is successful, -1 if it failed
*/
int put_wall(struct player* p, struct move_t wall)
{
      // Remove edges
   int test1, test2; 
   test1 = graph__remove_edge(p->graph, wall.e[0].fr, wall.e[0].to); 
   test2 = graph__remove_edge(p->graph, wall.e[1].fr, wall.e[1].to);
   
   if (test1 == -1 && test2 == -1)  return -1; 

   size_t left_square = min_node(wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to);
   p->wall_installed[position_square(left_square, p->n)] = 1; 

   return 1; 
}

/* Destroy a wall on the board meaning adding 2 edges on the graph
*
*  @param p player studied
*  @param wall the wall we are posing 
*  @param dir direction used to add edge
*  @return 0 if the wall destruction is successful, -1 if it failed
*/
int destroy_wall(struct player* p, struct move_t wall, int dir)
{
   int test = -1; 
   test = graph__add_edge(p->graph, wall.e[0].fr, wall.e[0].to, dir);
   if (test == -1) return -1; 
   test = graph__add_edge(p->graph, wall.e[1].fr, wall.e[1].to, dir);
   if (test == -1) return -1; 

   size_t left_square = min_node(wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to);
   p->wall_installed[position_square(left_square, p->n)] = 0; 

   return 0; 
}


/* Checking if a vertex isn't already in the vertex list
*  
*  @param waiting Waitinglist - size_t array
*  @param size Size of the waiting list 
*  @param v vertex studied
*  @return Booleen: 0 => In waitingList / 1 => Not in waiting list
*/
int not_already_inWaitinglist(size_t* waitingList, int size, size_t v)
{
   for(int i=0; i<size; i++)
   {
      if (waitingList[i] == v)
      {
         return 0; 
      }
   }

   return 1; 
}


/* Checking if a player at a specific pos can reach a winning position depending of the color
*
*  @param graph graph of the game
*  @param n n size of the graph
*  @param color color studied
*  @param pos position studied
*  @return 1 if player can still win, 0 instead
*/
int existPath_Player(struct graph_t* graph, size_t n, size_t color, size_t pos)
{
   int* marked = calloc(graph->num_vertices, sizeof(int)); 
   size_t* waitingList = malloc(sizeof(size_t) * graph->num_vertices); 
   
   for(size_t i = 0; i<graph->num_vertices; i++)
   {
      waitingList[i] = (size_t) -1; 
   }

   size_t current = pos; 
   int size = 1; 
   int to_treat = 0; 
   waitingList[to_treat] = current;
   
   
   size_t neighboor = -1; 
   while (waitingList[to_treat] != (size_t) -1 && (size_t) to_treat < graph->num_vertices )
   {  
      
      current = waitingList[to_treat]; 
      marked[current] = 1; 
      
      //shift_left(waitingList, 0, size-1); 
      to_treat++; 
      
      for(int dir = NORTH; dir < 5; dir++)
      {
            // If neighboor exist + non treated 
         
         neighboor = graph__get_neighboor(graph, n, current, dir);
         if ( (neighboor != (size_t) -1))
         {
            
            if (marked[neighboor] == 0 && not_already_inWaitinglist(waitingList, size, neighboor))
            {
               waitingList[size] = neighboor;
               size++;
            }
         }
      }
   }

   
   // - List winning position
   size_t* list = malloc(sizeof(size_t) * n); 
   graph__list_ownership(graph, n, other_player(color), list); 
   
   
   for(size_t node = 0; node < graph->num_vertices; node++)
   {
         // Find a winning path 
      if (marked[node])
      {
         for(size_t i = 0; i<n; i++)
         {
            if (node == list[i])
            {
               
               free(marked);
               
               free(list); 
               
               free(waitingList);
               return 1; 
            }
         }
      }
   }

   free(list); 
   free(marked);
   free(waitingList);
   return 0; 
}

/* Check if posing a wall is allowed 
*
*  @param pointer on the player
*  @param specific wall we test
*  @return 1 if posing this wall is allowed, 0 if not and -1 if errors
*/
int checkPath(struct player* p, struct move_t wall, int dir)
{
      // Put Fake Wall (for test)
   put_wall(p, wall); 

   int check_player1 = existPath_Player(p->graph, p->n, p->id, p->pos);
   int check_player2 = existPath_Player(p->graph, p->n, other_player(p->id), p->ennemy_pos);

      // Remove testing Wall
   destroy_wall(p, wall, dir); 

   if (check_player1 && check_player2)
   {
      return 1;
   }
   return 0; 

}




// -------- WALL


