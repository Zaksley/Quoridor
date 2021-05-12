#include <stdlib.h>
#include <stddef.h>
#include "../src/graph_modif.h"
#include "../tests/test_graph_shape.h"

#define TESTCASE(msg, test) printf("%-60s : %s\n", msg, test ? "\033[1;92mPassed\033[0m" : "\033[1;91mFailed\033[0m")

/* Checks if n is in t
 *
 * @param n a value to be looking for
 * @param t an array of value
 * @param s the length of t
 * @return 1 if true, 0 else
 */

int in_array(size_t n, size_t *t, size_t s)
{
   for (size_t i = 0; i < s; i++)
      if (*(t + i) == n)
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

/*
void print_graph(const struct graph_t *graph, size_t n, size_t *t, size_t s) 
{
   struct graph_t *square = graph__create_square(n);
   size_t N = n * n;
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
         num = gsl_spmatrix_uint_get(graph->t, i, j);
         ref = gsl_spmatrix_uint_get(square->t, i, j);
         if (num != 0)
         {
            if (in_array(i, t, s) || in_array(j, t, s))
               printf("|\033[01;91m%d\033[0m ", num);
            else if (ref != num)
               printf("|\033[01;35m%d\033[01;96m%d\033[0m", num, ref);
            else
               printf("|\033[01;34m%d\033[0m ", num);
         }
         else if (ref != 0)
         {
            if (in_array(i, t, s) || in_array(j, t, s))
               printf("|\033[01;32m#\033[0m ");
            else
               printf("|\033[01;91m#\033[0m ");
         }
         else
            printf("|  ");
      }
      printf("|%ld\n", i);
   }
}
*/

int test__shape(const struct graph_t *graph, size_t n, size_t *t, size_t s)
{
   struct graph_t *square = graph__create_square(n);
   size_t N = n * n;

   int num, ref;
   for (size_t i = 0; i < N; i++)
   {
      for (size_t j = 0; j < N; j++)
      {
         num = gsl_spmatrix_uint_get(graph->t, i, j);
         ref = gsl_spmatrix_uint_get(square->t, i, j);
         if (num != 0)
         {
            if (in_array(i, t, s) || in_array(j, t, s) || ref != num)
            {
               graph__free(square);
               return 0;
            }
         }
         else if (ref != 0)
         {
            if (!(in_array(i, t, s) || in_array(j, t, s)))
            {
               graph__free(square);
               return 0;
            } 
         }
      }
   }
   graph__free(square);
   return 1;
}

int test__torus_3() 
{
   struct graph_t* torus = graph__create_torus(3);
   size_t empty_vertices[1] = {4};
   int test = test__shape(torus, 3, empty_vertex, 1);
   graph__free(torus);
   
   return test;
}

int test__torus_6() 
{
   struct graph_t* torus = graph__create_torus(6);
   size_t empty_vertices[4] = {8, 9, 14, 15};
   int test = test__shape(torus, 6, empty_vertex, 4);
   graph__free(torus);
   
   return test;
}
