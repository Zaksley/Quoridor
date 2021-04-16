#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#include "test_graph_fonc.h"

int test__empty_graph__edge_exists()
{
   size_t n = 10;
   struct graph_t *graph = graph__initialize(n);
   for (size_t i = 0; i < n*n; i++)
   {
      for (size_t j = 0; j < n*n; j++)
	      if (graph__edge_exists(graph, i, j))
         {
            graph__free(graph);
            return 0;
         }
   }
   graph__free(graph);
   return 1;
}

int test__added_edge__edge_exists()
{
   size_t n = 10;
   struct graph_t *graph = graph__initialize(n);
   for (size_t i = 0; i < n*n - 1; i++)
   {
      graph__add_edge(graph, i, i+1, WEST);
      if (graph__edge_exists(graph, i, i+1) != 1)
      {
         graph__free(graph);
         return 0;
      }
   }
   graph__free(graph);
   return 1;
}

int test__removed_edge__edge_exists()
{
   size_t n = 10;
   struct graph_t *graph = graph__initialize(n);
   for (size_t i = 0; i < n*n - 1; i++)
   {
      graph__add_edge(graph, i, i+1, WEST);
      if (graph__edge_exists(graph, i, i+1) != 1)
      {
         graph__free(graph);
	      return 0;
      }
      graph__remove_edge(graph, i, i+1);
      if (graph__edge_exists(graph, i, i+1))
      {
         graph__free(graph);
         return 0;
      }
   }
   graph__free(graph);
   return 1;
}

int test__empty_graph__get_neighboor()
{
   size_t n = 10;
   struct graph_t *graph = graph__initialize(n); 
   for (size_t i = 0; i < n*n; i++)
   {
      for (enum direction d = NORTH; d < 5; d++)
      if (graph__get_neighboor(graph, n, i, d) != -1)
      {
         graph__free(graph);
         return 0;
      }
   }
   graph__free(graph);
   return 1;
}

int test__added_vertex__get_neighboor()
{
   size_t n = 10;
   struct graph_t *graph = graph__initialize(n);
   graph__add_edge(graph, 5, 6, WEST);
   if (graph__get_neighboor(graph, n, 5, WEST) != 6)
   {
      graph__free(graph);
      return 0;
   }
   for (enum direction d = NORTH; d < 5; d++)
      if (d != WEST && graph__get_neighboor(graph, n, 5, d) != -1)
      {
         graph__free(graph);
         return 0;
      }
	graph__free(graph);
   return 1;
}

int test__removed_vertex__get_neighboor()
{
   size_t n = 10;
   struct graph_t *graph = graph__initialize(n);
   graph__add_edge(graph, 5, 6, WEST);
   if (graph__get_neighboor(graph, n, 5, WEST) != 6)
   {
      graph__free(graph);
      return 0;
   }
   graph__remove_edge(graph, 5, 6);
   if (graph__get_neighboor(graph, n, 5, WEST) != -1)
   {
      graph__free(graph);
      return 0;
   }
   graph__free(graph);
   return 1; 
}

int test__same_edges__copy()
{
   size_t n = 10;
   struct graph_t *graph = graph__create_torus(n);
   struct graph_t *copy_g = graph__copy(graph, n);
   for (size_t i = 0; i < n*n; i++)
      for (size_t j = 0; j < n*n; j++)
	      if (graph__edge_exists(graph, i, j) != graph__edge_exists(copy_g, i, j))
         {
            graph__free(graph);
            graph__free(copy_g);
	         return 0;
         }
   graph__free(graph);
   graph__free(copy_g);
   return 1;
}

int test__same_neighboors__copy()
{
   size_t n = 10;
   struct graph_t *graph = graph__create_torus(n);
   struct graph_t *copy_g = graph__copy(graph, n);
   for (size_t i = 0; i < n*n; i++)
      for (enum direction d = NORTH; d < 5; d++)
	      if (graph__get_neighboor(graph, n, i, d) != graph__get_neighboor(copy_g, n, i, d))
         {
            graph__free(graph);
            graph__free(copy_g);
	         return 0;
         }
   graph__free(graph);
   graph__free(copy_g);
   return 1;
}

int test__modify_add__copy()
{
   size_t n = 10;
   struct graph_t *graph = graph__initialize(n);
   struct graph_t *copy = graph__copy(graph, n);
   for (size_t i = 0; i < n*n - 1; i++)
   {
      graph__add_edge(copy, i, i + 1, WEST);
      if (graph__edge_exists(graph, i, i + 1) == graph__edge_exists(copy, i, i + 1))
      {
         graph__free(graph);
         graph__free(copy);
	      return 0;
      }
   }
   graph__free(graph);
   graph__free(copy);
   return 1;
}

int test__modify_remove__copy()
{
   size_t n = 10;
   struct graph_t *graph = graph__initialize(n);
   struct graph_t *copy = graph__copy(graph, n);
   for (size_t i = 0; i < n*n - 1; i++)
   {
      if (graph__remove_edge(copy, i, i + 1) == 0)
	      if (graph__edge_exists(graph, i, i + 1) == graph__edge_exists(copy, i, i + 1))
         {
            graph__free(graph);
            graph__free(copy);
	         return 0;
         }
   }
   graph__free(graph);
   graph__free(copy);
   return 1;
}

int test__twice__add_edge()
{
   size_t n = 10;
   struct graph_t *graph = graph__initialize(n);
   for (size_t i = 0; i < n*n -1; i++)
   {
      graph__add_edge(graph, i, i + 1, EAST);
      if (graph__add_edge(graph, i, i + 1, SOUTH) != -1)
      {
         graph__free(graph);
	      return 0;
      }
   }
   graph__free(graph);
   return 1;
}

int test__remove__add_edge()
{
   size_t n = 10;
   struct graph_t *graph = graph__initialize(n);
   for (size_t i = 0; i < n*n -1; i++)
   {
      graph__add_edge(graph, i, i + 1, WEST);
      if (graph__remove_edge(graph, i, i + 1) != 0)
      {
         graph__free(graph);
	      return 0;
      }
   }
   graph__free(graph);
   return 1;
}

int test__twice__remove_edge()
{
   size_t n = 10;
   struct graph_t *graph = graph__create_square(n);
   for (size_t i = 0; i < n*n -1; i++)
   {
      graph__remove_edge(graph, i, i + 1);
      if (graph__remove_edge(graph, i, i + 1) != -1)
      {
         graph__free(graph);
	      return 0;
      }
   }
   graph__free(graph);
   return 1;
}

int test__add__remove_edge()
{
   size_t n = 10;
   struct graph_t *graph = graph__create_square(n);
   for (size_t i = 0; i < n*n -1; i++)
   {
      graph__remove_edge(graph, i, i + 1);
      if (graph__add_edge(graph, i, i + 1, EAST) != 0)
      {
         graph__free(graph);
	      return 0;
      }
   }
   graph__free(graph);
   return 1;
}
