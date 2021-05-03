#include <stdlib.h>
#include <dlfcn.h>
#include "strategy.h"

/* Return either a winning move reachable (first choice) or the best move to close the gap
*
*  @param pos Position of the player
*  @param winning_nodes List of winning position
*  @param numb_win Length of the winning list
*  @param moves List of moves available
*  @return The best move possible to rush
*/
size_t rushing_path(size_t pos, size_t* winning_nodes, size_t numb_win, struct moves_valids* moves)
{

   if (moves->number <= 0) 
   {
      fprintf(stderr, "Error : No more moves available");
      exit(6);
   }

   size_t gap = (size_t) abs( (int) (winning_nodes[0] - pos)); 
   size_t best = moves->valid[0].m; 
   size_t test_gap; 
   for(int i=0; i<moves->number; i++)
   {
         // Finding winning position
      for(size_t j=0; j<numb_win; j++)
      {
         if (moves->valid[i].m == winning_nodes[j])
         {
            return moves->valid[i].m;
         }
      }

         // Else => Closing up the gap
      test_gap = (size_t) abs( (int) (winning_nodes[0] - moves->valid[i].m));
      if (test_gap < gap)
      {
         gap = test_gap; 
         best = moves->valid[i].m;
      }
   }

   return best;
}


int dijkstra(size_t n, struct graph_t *graph, size_t p0, int numb_win, size_t* win_pos)
{
   int capacity = 100;
   int nb_pos = 1;
   size_t *visited_pos = malloc(sizeof(size_t) * capacity);
   visited_pos[0] = p0;
   
   int add_pos(int n, int e)
   {
      if (e == -1)
         return 0;
      for(int i; i < n; i++)
      {
         if((int)visited_pos[i] == e)
            return 0;
      }
      if(n >= capacity)
      {
         capacity *= 2;
         visited_pos = realloc(visited_pos, sizeof(size_t) * capacity);
      }
      visited_pos[n] = (size_t)e;
      return 1;
   }

   int is_empty_intersect(int n1, size_t* l1, int n2, size_t* l2)
   {
      for(int i = 0; i < n1; i++)
      {
         for(int j = 0; j < n2; j++) {
            if(l1[i] == l2[j])
               return 0;
         }
      }
      return 1;
   }

   int nb_move = 0;
   while(is_empty_intersect(nb_pos, visited_pos, numb_win, win_pos))
   {
      int added_pos = 0;
      for(int i = 0; i < nb_pos; i++)
      {
         for(enum direction d = NORTH; d < 5; d++)
         {
            added_pos += add_pos(nb_pos + added_pos, graph__get_neighboor(graph, n, visited_pos[i], d));
         }
      }
      nb_pos += added_pos;
      nb_move++;
   }
   return nb_pos;
}

size_t path_dijkstra(struct player* p) 
{
   struct moves_valids* pos = valid_positions(p);
   int nb_pos = pos->number;
   int dist[nb_pos];
   int min = 0;
   for(int i = 0; i < nb_pos; i++) 
   {
      dist[i] = dijkstra(p->n, p->graph, (pos->valid[i]).m, p->numb_win, p->winning_nodes);
      if (dist[i] < dist[min])
         min = i;
   }
   return (pos->valid[min]).m;
}