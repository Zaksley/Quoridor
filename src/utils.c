#include <stdlib.h>
#include "utils.h"

// -------- MOVE

void add_position(struct move_t* valid, struct move_t new, int* count_moves)
{
   valid[*count_moves] = new;
   *count_moves++; 
}

// TODO => vérifier que sur les positions, il n'y a pas de joueur adverse
struct move_t* valid_positions(struct player* p)
{
   struct move_t valid[4] = {};
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
      value = graph__get_neighboor(p->graph, p->pos, dir);
      new.m = value; 
      if (value) add_position(valid, new, count_moves); 
   }

   return valid; 

}

// -------- MOVE



// -------- WALL
struct move_t* valid_walls(struct player* p)
{
      // Dynamic array Walls
   struct move_t* walls = malloc(sizeof(struct move_t) * 1); 
   int size = 0; 
   int capacity = 1; 
   if (walls == NULL) exit(0); 

      // Setup Wall 
   struct move_t wall; 
   wall.t = WALL; 
   wall.c = p->id; 


   size_t n1, n2; 
   for(size_t node = 0; node < p->graph->num_vertices; node++)
   {
      for(int i=0; i<2; i++)
      {
            // Neighboors EAST-WEST
         if (i == 0)
         {
            n1 = graph__get_neighboor(p->graph, node, EAST); 
            n2 = graph__get_neighboor(p->graph, node, WEST); 
         }
            // Neighboors NORTH-SOUTH
         else
         {
            n1 = graph__get_neighboor(p->graph, node, NORTH); 
            n2 = graph__get_neighboor(p->graph, node, SOUTH); 
         }

         if (n1 != -1 && n2 != -1)
         {
            if (graph__edge_exists(p->graph, node, n1) && graph__edge_exists(p->graph, node, n2))
            {
                  // Set the struct move_t wall 
               struct edge_t e1 = {node, n1};
               struct edge_t e2 = {node, n2}; 
               wall.m = node;
               wall.e[0] = e1;
               wall.e[1] = e2; 

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

   return walls;   
}


void put_wall(struct player* p, struct move_t wall)
{
      // Remove edges
   int removed;
   removed = graph__remove_edge(p->graph, wall.e[0].fr, wall.e[0].to); 
   removed = graph__remove_edge(p->graph, wall.e[1].fr, wall.e[1].to);

   return; 
}

void shift_left(size_t s[], int begin, int end)
{
   int i=0;

   while(i <= end-begin )
   { 
      s[begin+i-1] = s[begin+i];
      i++;
   }  
}


int existPath_Player(struct player* p, size_t number_player, size_t pos_player, struct move_t wall)
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
         neighboor = graph__get_neighboor(p->graph, current, dir);
         if ( neighboor != -1 && marked[neighboor] != 0)
         {
            waitingList[size] = graph__get_neighboor(p->graph, current, dir); 
            size++;
         }
      }
   }

   for(size_t node = 0; node < p->graph->num_vertices; node++)
   {
         // Find a winning path 
      if (marked[node] && (int) gsl_spmatrix_get(p->graph->o, number_player, node) == 1)
      {
         return 1; 
      }
   }

   return 0; 

}

int checkPath(struct player* p, struct move_t wall)
{
      // Put Fake Wall (for test)
   put_wall(p, wall); 

   int check_1 = existPath_Player(p, p->pos, p->id, wall);
   int check_2; 
   if (p->id == BLACK) check_2 = existPath_Player(p, p->ennemy_pos, WHITE, wall);
   else existPath_Player(p, p->ennemy_pos, BLACK, wall);
   

      // Remove testing Wall
   int test = -1; 
   test = graph__remove_edge(p->graph, wall.e[0].fr, wall.e[0].to);
   test = graph__remove_edge(p->graph, wall.e[1].fr, wall.e[1].to);

   if (check_1 && check_2) return 1;
   return 0; 

}




// -------- WALL