#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#include "../src/graph_modif.h"

#define TESTCASE(msg, test) printf("%-60s : %s\n", msg, test ? "\033[1;92mPassed\033[0m" : "\033[1;91mFailed\033[0m")

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

int test__modify_add__copy()
{
   size_t n = 10;
   struct graph_t *graph = graph__initialize(n);
   struct graph_t *copy = graph__copy(graph, n);
   for (size_t i = 0; i < n*n - 1; i++)
   {
      graph__add_edge(copy, i, i + 1, WEST);
      if (graph__edge_exists(graph, i, i + 1) == graph__edge_exists(copy, i, i + 1))
	 return 0;
   }
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
	    return 0;
   }
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
	 return 0;
   }
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
	 return 0;
   }
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
	 return 0;
   }
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
	 return 0;
   }
   return 1;
}

int main() 
{
   printf("\033[1m----- Graph_modif | Functionnal tests -----\033[0m\n");
   printf("\033[1mgraph__edge_exists :\033[0m\n");
   TESTCASE("- edge_exists | empty graph have no existing edge" ,test__empty_graph__edge_exists());
   TESTCASE("- edge_exists | added edge exist", test__added_edge__edge_exists());
   TESTCASE("- edge_exists | added then removed edge don't exist", test__removed_edge__edge_exists());
   printf("\033[1mgraph__get_neighboor :\033[0m\n");
   TESTCASE("- get_neighboor | non-existing neighboors are not found", test__empty_graph__get_neighboor());
   TESTCASE("- get_neighboor | added neighboor is found", test__added_vertex__get_neighboor());
   TESTCASE("- get_neighboor | removed neighboor is not found", test__removed_vertex__get_neighboor());
   printf("\033[1mgraph__copy :\033[0m\n");
   TESTCASE("- copy | copied graph has the same edges", test__same_edges__copy());
   TESTCASE("- copy | copied graph has the same neighboors", test__same_neighboors__copy());
   TESTCASE("- copy | adding egdes on copy don't modify the original", test__modify_add__copy());
   TESTCASE("- copy | removing edges on copy don't modify the original", test__modify_remove__copy());
   printf("\033[1mgraph__add_edge :\033[0m\n");
   TESTCASE("- add_edge | adding twice the same edge return error", test__twice__add_edge());
   TESTCASE("- add_edge | added edge can be removed", test__remove__add_edge());
   printf("\033[1mgraph__remove_edge :\033[0m\n");
   TESTCASE("- remove_edge | removing twice the same edge return error", test__twice__remove_edge());
   TESTCASE("- remove_edge | removed edge can be added again", test__add__remove_edge());
   printf("\n");
   return 0;
}
