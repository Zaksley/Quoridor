#include <stdlib.h>
#include <stddef.h>
#include "../src/graph_modif.h"

/* Checks if n is in t
 *
 * @param n a value to be looking for
 * @param t an array of value
 * @param s the length of t
 * @return 1 if true, 0 else
 */
int is_in(size_t n, size_t *t, size_t s) 
{
   for (size_t i = 0; i < s; i++)
      if (*(t+i) == n)
	 return 1;
   return 0;
}

/* Print a graph
 *
 * @param graph the graph to be printed
 * @param n size of the graph
 * @param t vertex that should have no edge
 * @param s size of t, number of vertex excluded
 * @return nothing
 */
void print_graph(const struct graph_t *graph, size_t n, size_t *t, size_t s) 
{
   struct graph_t *square = graph__create_square(n);
   size_t N = n*n;
   printf("|");
   for (size_t i = 0; i < N; i++) 
   {
      if (i < 10) 
	 printf(" ");
      printf("%ld|", i);
   }
   printf("\n");
   int num, ref;
   for (size_t i = 0; i < N; i++) 
   {
      for (size_t j = 0; j < N; j++) 
      {
	 num = (int)gsl_spmatrix_get(graph->t, i, j);
	 ref = (int)gsl_spmatrix_get(square->t, i, j);
	 if (num != 0) 
	 {
	    if (is_in(i, t, s) || is_in(j, t, s))
	       printf("|\033[01;91m%d\033[0m ", num);
	    else if (ref != num)
	       printf("|\033[01;35m%d\033[01;96m%d\033[0m", num, ref);
	    else
	       printf("|\033[01;34m%d\033[0m ", num);
	 }
	 else if (ref != 0) 
	 {
	    if (is_in(i, t, s) || is_in(j, t, s))
	       printf("|\033[01;32m#\033[0m ");
	    else
	       printf("|\033[01;91m#\033[0m ");
	 }
	 else printf("|  ");
      }
      printf("|%ld\n", i);
   }
}

int main()
{
   size_t n = 3;
   struct graph_t *g = graph__create_square(n);
   size_t t[0]; // list of vertex that should be excluded in the graph choosed
   print_graph(g, n, t, 0);
   return 0;
}
