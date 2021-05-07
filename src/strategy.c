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

/*  Check if a vertex is in a list of nodes
*
*   @param nodes array of nodes
*   @param numb_nodes length of nodes array
*   @param v vretex studied
*   @return booleen : 1 (True) / 0 (False)
*/
int vertex_in_nodeList(struct node* nodes, int numb_nodes, size_t v)
{
   for(int i=0; i<numb_nodes; i++)
   {
      if (nodes[i].v == v)  return 1; 
   }

   return 0; 
}

int add_vertex(struct node* nodes, int number_nodes, struct node new)
{
      // Add node to list
   if (!vertex_in_nodeList(nodes, number_nodes, new.v))
   {
      nodes[number_nodes] = new; 
      number_nodes++; 
   }

   return number_nodes;
}

/* Gets the path by reaching predecessors
*
*   @param nodes list of nodes
*   @param end last node (final move)
*   @param c color to construct moves
*   @return dijkstra path (move[0] = actual pos, move[numb_nodes-1] = winning pos)
*/
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

/* Calculate path by Dijkstra algorithm
*
*   @param graph a graph
*   @param n size of graph
*   @param pos position 0 
*   @param ennemy_pos position of the ennemy
*   @param c color of player studied
*   @param winning_nodes array of winning nodes (to search last node)
*   @param numb_win length of winning nodes array
*   @return dijsktra path (move[0] = actual pos, move[numb_nodes-1] = winning pos)
*/
struct moves_valids* dijkstra(struct graph_t* graph, size_t n, size_t pos, size_t ennemy_pos, enum color_t c, size_t* winning_nodes, size_t numb_win)
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

               // Double jump
            if (neighboor == ennemy_pos)
            {
               size_t double_jump = (size_t) graph__get_neighboor(graph, n, neighboor, dir);
               new.v = double_jump; 

               int second_dir = get_second_dir(dir);


                  // Go on side
               if (double_jump == (size_t) -1)
               {
                  size_t n1 = (size_t) graph__get_neighboor(graph, n, neighboor, second_dir);
                  size_t n2 = (size_t) graph__get_neighboor(graph, n, neighboor, second_dir+1);

                  if (n1 != (size_t) -1)  
                  {
                     new.v = n1; 
                        // Winning position discovered
                     if (in_vertexList(winning_nodes, numb_win, new.v))
                     {
                        struct moves_valids* path = get_predecessor(nodes, new, c); 
                        free(nodes);
                        return path;
                     }

                     number_nodes = add_vertex(nodes, number_nodes, new);
                  }

                  if (n2 != (size_t) -1) 
                  {
                     new.v = n2; 
                        // Winning position discovered
                     if (in_vertexList(winning_nodes, numb_win, new.v))
                     {
                        struct moves_valids* path = get_predecessor(nodes, new, c); 
                        free(nodes);
                        return path;
                     }
                     number_nodes = add_vertex(nodes, number_nodes, new);
                  }
               }
                  // Double jump
               else  
               {
                  new.v = double_jump; 
                  number_nodes = add_vertex(nodes, number_nodes, new);
               }
            }

               // Normal move
            else number_nodes = add_vertex(nodes, number_nodes, new);

            // Winning position discovered
            if (in_vertexList(winning_nodes, numb_win, new.v))
            {
               struct moves_valids* path = get_predecessor(nodes, new, c); 
               free(nodes);
               return path;
            }
         }
      }
      to_treat++; 
   }

   exit(0);
}

/* 
* NOT USED
int wall_from_edge(struct moves_valids* walls, struct edge_t e)
{
   for(int i=0; i<walls->number; i++)
   {
      if ( edge_equal(walls->valid[i].e[0], e) || edge_equal(walls->valid[i].e[1], e) )
      {
         return i;
      }
   }

   return -1;
}
*/

/* Create an edge from another edge and a direction
*
*  @param p player
*  @param e1 edge to create by
*  @param second_dir direction to get nodes
*  @return edge created
*/
struct edge_t get_edge_by_dir(struct player* p, struct edge_t e1, int second_dir)
{
      // Get edge from one dir
   size_t n1 = graph__get_neighboor(p->naked_graph, p->n, e1.fr, second_dir);
   size_t n2 = graph__get_neighboor(p->naked_graph, p->n, e1.to, second_dir);
   
   struct edge_t e2 = {.fr = n1, .to = n2};
   return e2; 
}

/* Return a wall that cut the ennemy path without cutting the own path of the player
*
*  @param p player
*  @param path path to victory for the player
*  @param ennemy_path path to victory for the opponent
*  @return a wall that cut ennemy's path OR a move if there is no wall to put
*/
struct move_t cut_ennemy_path(struct player* p, struct moves_valids* path, struct moves_valids* ennemy_path)
{
      // Initialization
   struct move_t wall = {.c = p->id, .t = WALL, .m = (size_t) -1}; 
   struct edge_t e1 = {-1, -1};
   struct edge_t e2 = {-1, -1}; 

   int dir = 0;
   int second_dir = 0;

      // Get all viable walls
   struct moves_valids* walls = valid_walls(p);

   for(int i = ennemy_path->number - 1; i > 0; i--)
   {
      e1.fr = ennemy_path->valid[i].m;
      e1.to = ennemy_path->valid[i-1].m;

         // Cutting my own
      if (move_in_list(path, ennemy_path->valid[i]) || move_in_list(path, ennemy_path->valid[i-1]))   continue;
      wall.e[0] = e1;

      dir = graph__get_dir(p->graph, e1.fr, e1.to);
      second_dir = get_second_dir(dir);

      // === First side
      e2 = get_edge_by_dir(p, e1, second_dir); 
      wall.e[1] = e2; 
         // Wall valid + don't cut my own path
      if (wall_in_list(walls->number, walls->valid, wall))  
      {
         free(walls);
         return wall;
      }

      // === Second side
      e2 = get_edge_by_dir(p, e1, second_dir+1); 
      wall.e[1] = e2;
         // Wall valid + don't cut my own path
      if (wall_in_list(walls->number, walls->valid, wall))  
      {
         free(walls);
         return wall;
      }
   }

   free(walls);
   // === NO WALL FOUND 
   struct move_t move = {.c = p->id, .t = MOVE, .m = path->valid[1].m,.e = { (struct edge_t) {-1, -1}, (struct edge_t) {-1, -1} }}; 
   return move;
}



struct move_t double_dijkstra_strategy(struct player* p)
{
   struct moves_valids* player_path = dijkstra(p->graph, p->n, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   struct moves_valids* ennemy_path = dijkstra(p->graph, p->n, p->ennemy_pos, p->pos, other_player(p->id), p->owned_nodes, p->numb_win);

      // Choose to move to the win
   if (p->num_walls <= 0 ||player_path->number <= ennemy_path->number) 
   {
      struct move_t move = player_path->valid[1]; 
      free(player_path);
      free(ennemy_path);
      return move;
   }
      // Put a wall to disturb ennemy 
   else
   {
      struct move_t wall = cut_ennemy_path(p, player_path, ennemy_path); 
      free(player_path);
      free(ennemy_path);
      return wall; 
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