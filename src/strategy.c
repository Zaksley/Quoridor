#include <stdlib.h>
#include <dlfcn.h>
#include "strategy.h"


//struct edge_t no_wall = {-1, -1}; 

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
      exit(2);
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

//==========================================================
int vertex_in_nodeList(struct node* nodes, int numb_nodes, size_t v)
{
   for(int i=0; i<numb_nodes; i++)
   {
      if (nodes[i].v == v)  return 1; 
   }

   return 0; 
}

struct moves_valids* get_predecessor(struct node* nodes, struct node end, enum color_t c)
{
   int numb = end.dist+1; 
   struct moves_valids* path = malloc(sizeof(struct moves_valids));
   struct move_t* moves = malloc(sizeof(struct move_t) * numb);
   path->valid = moves; 
   path->number = numb; 

   struct node current = {.dist = end.dist, .site_predecessor = end.site_predecessor, .v = end.v};
   struct move_t move = {.c = c, .e = { (struct edge_t) {-1, -1}, (struct edge_t) {-1, -1}}, .t = MOVE};

   int i = numb-1;
   while(i > -1)
   {
      move.m = current.v;
      path->valid[i] = move; 

      // Actualisation
      current.v = nodes[current.site_predecessor].v; 
      current.site_predecessor = nodes[current.site_predecessor].site_predecessor; 

      i -= 1;
   }
   
   return path; 
}

struct moves_valids* dijkstra(struct graph_t* graph, size_t n, size_t pos, enum color_t c, size_t* winning_nodes, size_t numb_win)
{
   
      // Init first node
   int length_path = 0; 
   struct node position = {.dist = length_path, .site_predecessor = 0, .v = pos}; 

      // Waiting list
   struct node* nodes = malloc(sizeof(struct node) * n * n);
   nodes[0] = position; 
   
   size_t neighboor = -1; 
   struct node new; 
   struct node current; 

   size_t to_treat = 0; 
   size_t number_nodes = 1; 

   while (number_nodes < n*n)
   {
      current = nodes[to_treat]; 
      for(int dir =1; dir < 5; dir++)
      {
         neighboor = (size_t) graph__get_neighboor(graph, n, current.v, dir);

            // Not reachable 
         if (neighboor == (size_t) -1) continue;
         else
         {
            new.dist = current.dist + 1;
            new.site_predecessor = to_treat; 
            new.v = neighboor;

               // Winning position discovered
            if (in_vertexList(winning_nodes, numb_win, neighboor))
            {
               
               struct moves_valids* path = get_predecessor(nodes, new, c); 
               free(nodes);
               return path;
            }

               // Add node to list
            else if (!vertex_in_nodeList(nodes, number_nodes, neighboor))
            {
               nodes[number_nodes] = new; 
               number_nodes++; 
            }
         }
      }
      to_treat++; 
   }

   exit(0);
}

//struct move_t disturb_ennemy(struct player* p)

struct move_t double_dijkstra_strategy(struct player* p)
{
   struct moves_valids* player_path = dijkstra(p->graph, p->n, p->pos, p->id, p->winning_nodes, p->numb_win);
   struct moves_valids* ennemy_path = dijkstra(p->graph, p->n, p->ennemy_pos, other_player(p->id), p->owned_nodes, p->numb_win);

      // Choose to move to the win
   if (player_path->number <= ennemy_path->number) 
   {
      return player_path->valid[1]; 
   }
      // Put a wall to disturb ennemy 
   else
   {
         // TODO
      return ennemy_path->valid[1];
   }
}













// ==========================
   /*
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
   */