#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#include "../src/graph_modif.h"

#define TESTCASE(msg, test) printf("%-50s : %s\n", msg, test ? "\033[1;92mPassed\033[0m" : "\033[1;91mFailed\033[0m")

int test__empty_graph__edge_exists()
{
   size_t n = 10;
   struct graph_t *graph = graph__initialize(n);
   for (size_t i = 0; i < n*n; i++)
   {
      for (size_t j = 0; j < n*n; j++)
	 if (graph__edge_exists(graph, i, j))
	    return 0;
   }
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
	 return 0;
   }
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
	 return 0;
      graph__remove_edge(graph, i, i+1);
      if (graph__edge_exists(graph, i, i+1))
	 return 0;
   }
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
	 return 0;
   }
   return 1;
}

int test__added_vertex__get_neighboor()
{
   size_t n = 10;
   struct graph_t *graph = graph__initialize(n);
   graph__add_edge(graph, 5, 6, WEST);
   if (graph__get_neighboor(graph, n, 5, WEST) != 6)
      return 0;
   for (enum direction d = NORTH; d < 5; d++)
      if (d != WEST && graph__get_neighboor(graph, n, 5, d) != -1)
	 return 0;
   return 1;
}

int test__removed_vertex__get_neighboor()
{
   size_t n = 10;
   struct graph_t *graph = graph__initialize(n);
   graph__add_edge(graph, 5, 6, WEST);
   if (graph__get_neighboor(graph, n, 5, WEST) != 6)
      return 0;
   graph__remove_edge(graph, 5, 6);
   if (graph__get_neighboor(graph, n, 5, WEST) != -1)
      return 0;
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
	    return 0;
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
	    return 0;
   return 1;
}

int main() 
{
   TESTCASE("Empty graph have no edge" ,test__empty_graph__edge_exists());
   TESTCASE("Added edge exist", test__added_edge__edge_exists());
   TESTCASE("Added then removed edge don't exist", test__removed_edge__edge_exists());
   TESTCASE("Empty graph's vertices don't have neighboor", test__empty_graph__get_neighboor());
   TESTCASE("Added vertex is a neighboor", test__added_vertex__get_neighboor());
   TESTCASE("Added then removed vertex is not a neighboor", test__removed_vertex__get_neighboor());
   TESTCASE("A copy have the same edges as the original", test__same_edges__copy());
   TESTCASE("A copy have the same neighboors as the original", test__same_neighboors__copy());
   TESTCASE("Modifying a copy don't modify the original", 0);
   TESTCASE("Adding twice the same edge return error", 0);
   TESTCASE("Added edge can be removed", 0);
   TESTCASE("Removing twice the same edge return error", 0);
   TESTCASE("Removed edge can be added again", 0);
   return 0;
}
