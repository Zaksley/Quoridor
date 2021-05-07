#include <stdlib.h>
#include <dlfcn.h>
#include "utils.h"

// ===================  PLAYER =================== 

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

/* Create a fake player fast to test functions
*
*  @param graph Graph created that we link to our player
*  @param n size of graph
*  @param pos position of the player
*  @param ennemy_pos ennemy position
*  @return a player
*/
struct player* initialize_test_player(struct graph_t* graph, size_t n, size_t pos, size_t ennemy_pos, enum color_t id)
{
      //Functions 
	struct player* test_player = get_functions("./install/libplayer_move_random.so");

      // Basic initialization
   test_player->id = id;
	test_player->pos = pos;
	test_player->ennemy_pos = ennemy_pos; 

   test_player->num_walls = 20; 
	test_player->n = n;
   test_player->graph = graph;
   test_player->naked_graph = graph__copy(test_player->graph, test_player->n);

      // No need
   test_player->first_move = -1; 
	
      // Walls placed
	test_player->wall_installed = calloc( (test_player->n-1)*(test_player->n-1), sizeof(int)); 

      // Winning & Owning nodes
   test_player->numb_win = graph__count_ownership(test_player->graph, test_player->n, test_player->id); 
   test_player->winning_nodes = malloc(sizeof(size_t) * test_player->numb_win);
   test_player->owned_nodes = malloc(sizeof(size_t) * test_player->numb_win);

   graph__list_ownership(test_player->graph, test_player->n, test_player->id, test_player->owned_nodes);
   graph__list_ownership(test_player->graph, test_player->n, other_player(test_player->id), test_player->winning_nodes);

	return test_player;
}

/* Return an initialized player
*
*  @param self player created in Client
*  @param id color chosed
*  @param graph graph from server
*  @param num_walls number of walls chosed by server
*  @return player initialized 
*/
struct player initialization_player(struct player self, enum color_t id, struct graph_t* graph, size_t num_walls)
{
      // Basic initialization
   self.id = id;
   self.num_walls = num_walls;
   self.n = graph__get_size(graph);
   self.graph = graph__copy(graph, self.n);
   self.naked_graph = graph__copy(self.graph, self.n); 

   self.first_move = 1; 

      // Walls placed
   self.wall_installed = calloc( (self.n-1)*(self.n-1), sizeof(int)); 

      // Winning & Owning nodes
   self.numb_win = graph__count_ownership(self.graph, self.n, self.id); 
   self.winning_nodes = malloc(sizeof(size_t) * self.numb_win);
   self.owned_nodes = malloc(sizeof(size_t) * self.numb_win);

   graph__list_ownership(self.graph, self.n, self.id, self.owned_nodes);
   graph__list_ownership(self.graph, self.n, other_player(self.id), self.winning_nodes);

   return self; 
}

/* Free elements of player
*
*  @param self struct player to free
*/
void finalization_player(struct player self)
{
      // Free arrays
   free(self.wall_installed);
   free(self.winning_nodes);
   free(self.owned_nodes); 

      // Free graph
   graph__free(self.graph);
}



/* Gets opposite color
*
*  @param color of player
*  @return opposite color
*/
enum color_t other_player(enum color_t player)
{
   if (player == WHITE) return BLACK; 
   else return WHITE; 
}

/* Gets opposite structure player
*
*  @param p array of players (Server)
*  @param c color of a player
*  @return opposite player
*/
struct player* player_color(struct player** p, enum color_t c)
{
   if (p[0]->id == c) return p[0];
   else return p[1]; 
}

/* Gets opposite dir
*
*
*/
int get_second_dir(int dir)
{
   if (dir == 1 || dir == 2)  return 3;
   else return 1; 
}

// ======= Usefull functions to check in list =======

/* Check if a wall isn't already in the list
*
*  @param size size of array moves
*  @param moves array stocking the moves
*  @param wall checking a specific wall
*  @return booleen 1 if wall is in the list, 0 otherwise 
*/
int move_in_list(struct moves_valids* moves, struct move_t move)
{
   for(int i=0; i<moves->number; i++)
   {
      if (moves->valid[i].m == move.m) return 1; 
   }
   return 0; 
}

/* Checking if a vertex isn't already in a vertex list
*  
*  @param waiting Waitinglist - size_t array
*  @param size Size of the list 
*  @param v vertex studied
*  @return Booleen: 1 => V in list / 0 => V not in list
*/
int in_vertexList(size_t* list, int size, size_t v)
{
   for(int i=0; i<size; i++)
   {
      if (list[i] == v)
      {
         return 1; 
      }
   }

   return 0; 
}

/* Check if a edge is equal to another
*
*  @param e1 edge one
*  @param e2 edge two
*  @return Booleen : 1 (True) or 0 (False)
*/
int edge_equal(struct edge_t e1, struct edge_t e2)
{
   if (e1.fr == e2.fr && e1.to == e2.to)  return 1; 
   else if (e1.fr == e2.to && e1.to == e2.fr)   return 1; 

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


// ======= Usefull functions to check in list =======

// ===================  PLAYER =================== 

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

               // Checking n1 & n2 not outside graph
            n1 = graph__get_neighboor(p->naked_graph, p->n, node, second_dir); 
            n2 = graph__get_neighboor(p->naked_graph, p->n, n_neighboor, second_dir);
            if (n1 == checkTest || n2 == checkTest) continue; 

               // Get real n1 & n2 (by edges)
            n1 = graph__get_neighboor(p->graph, p->n, node, second_dir); 
            n2 = graph__get_neighboor(p->graph, p->n, n_neighboor, second_dir);

               // * All edges on side are cut
            if (n1 == checkTest && n2 == checkTest) 
            {
               n1 = graph__get_neighboor(p->naked_graph, p->n, node, second_dir);
               n2 = graph__get_neighboor(p->naked_graph, p->n, n_neighboor, second_dir);
               
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


/* Checking if a player at a specific pos can reach a winning position depending of the color
*
*  @param graph graph of the game
*  @param n n size of the graph
*  @param color color studied
*  @param pos position studied
*  @return 1 if player can still win, 0 instead
*/
int existPath_Player(struct player* p, enum color_t color, size_t pos)
{
   size_t numb_nodes = p->n*p->n; 
   int* marked = calloc(numb_nodes, sizeof(int)); 
   size_t* waitingList = malloc(sizeof(size_t) * numb_nodes); 
   
   for(size_t i = 0; i<numb_nodes; i++)
   {
      waitingList[i] = (size_t) -1; 
   }

   size_t current = pos; 
   int size = 1; 
   int to_treat = 0; 
   waitingList[to_treat] = current;
   
   
   size_t neighboor = -1; 
   while (waitingList[to_treat] != (size_t) -1 && (size_t) to_treat < numb_nodes )
   {  
      
      current = waitingList[to_treat]; 
      marked[current] = 1; 
      
      to_treat++; 
      
      for(int dir = NORTH; dir < 5; dir++)
      {
            // If neighboor exist + non treated 
         neighboor = graph__get_neighboor(p->graph, p->n, current, dir);
         if ( (neighboor != (size_t) -1))
         {
            if (marked[neighboor] == 0 && !in_vertexList(waitingList, size, neighboor))
            {
               waitingList[size] = neighboor;
               size++;
            }
         }
      }
   }

   
   // - List winning position
   size_t* list; 
   if (p->id == color)   list = p->winning_nodes; 
   else                  list = p->owned_nodes; 

      /*
   int  numb_win = graph__count_ownership(graph, n, color);
   size_t* list = malloc(sizeof(size_t) * numb_win); 
   graph__list_ownership(graph, n, other_player(color), list); 
   */

   for(size_t node = 0; node < numb_nodes; node++)
   {
         // Find a winning path 
      if (marked[node])
      {
         for(int i = 0; i< p->numb_win; i++)
         {
            if (node == list[i])
            {
               
               free(marked);
               free(waitingList);
               return 1; 
            }
         }
      }
   }

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

   int check_player1 = existPath_Player(p, p->id, p->pos);
   int check_player2 = existPath_Player(p, other_player(p->id), p->ennemy_pos);

      // Remove testing Wall
   destroy_wall(p, wall, dir); 

   if (check_player1 && check_player2)
   {
      return 1;
   }
   return 0; 

}

// =================== WALL =================== 


