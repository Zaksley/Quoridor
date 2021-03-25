#include <stdlib.h>
#include <stdio.h>
#include <gsl/gsl_spmatrix.h>

#include "graph_modif.h"

/* Initializes a graph
 *
 * @return en empty square graph with no owners
 */
struct graph_t * graph__initialize()
{
	struct graph_t * graph = malloc(sizeof(struct graph_t));

	gsl_spmatrix * t = gsl_spmatrix_alloc(1,1);
	gsl_spmatrix * o = gsl_spmatrix_alloc(1,1);

	graph->num_vertices = 1;
	graph->t = t;
	graph->o = o;
	return graph;
}

/* Frees a graph
 *
 * @param graph the graph to be freed
 */
void graph__free(struct graph_t * graph)
{
	gsl_spmatrix_free(graph->t);
	gsl_spmatrix_free(graph->o);
	free(graph);
}

/* Creates a square graph
 *
 * @param n size of the graph
 * @return a full square graph with no owners
 */
struct graph_t * graph__create_square(size_t n)
{
  struct graph_t * g = malloc(sizeof(struct graph_t));
   g->num_vertices = n*n;
   g->t = gsl_spmatrix_alloc(n*n, n*n);
   g->o = gsl_spmatrix_alloc(2, n*n);
   for(size_t i = 0; i < n*n; i++) {
      if (i%n != n-1)
	 graph__add_edge(g, i, i + 1, WEST);
      if (i + n < n*n)
	 graph__add_edge(g, i, i + n, NORTH);
   }
   return g;
}

/* Creates a toric graph
 *
 * @param n size of the graph
 * @return a toric square graph with no owners
 */
struct graph_t * graph__create_torus(size_t n)
{
  struct graph_t * g = malloc(sizeof(struct graph_t));
  g->num_vertices = n*n;
  g->t = gsl_spmatrix_alloc(n*n, n*n);
  g->o = gsl_spmatrix_alloc(2, n*n);
  for(size_t i = 0; i < n; i++) {
    for(size_t j = 0; j < n; j++) {
      if (j < n - 1 && !(i >= n/3 && i < 2*n/3 && j >= n/3 - 1 && j < 2*n/3))
	graph__add_edge(g, i*n + j, i*n + j + 1, WEST);
      if (i < n - 1 && !(j >= n/3 && j < 2*n/3 && i >= n/3 - 1 && i < 2*n/3))
	graph__add_edge(g, i*n + j, (i+1)*n + j, NORTH);
    }
  }
  return g;
}

/* Creates a chopped graph
 *
 * @param n size of the graph
 * @return a H-shaped graph with no owners
 */
struct graph_t * graph__create_chopped(size_t n)
{
  struct graph_t * g = malloc(sizeof(struct graph_t));
  g->num_vertices = n*n;
  g->t = gsl_spmatrix_alloc(n*n, n*n);
  g->o = gsl_spmatrix_alloc(2, n*n);
  for(size_t i = 0; i < n; i++) {
    for(size_t j = 0; j < n; j++) {
      if (j < n - 1 && !((i < n/3 || i >= 2*n/3) && j >= n/3 - 1 && j < 2*n/3))
	graph__add_edge(g, i*n + j, i*n + j + 1, WEST);
      if (i < n - 1 && !(j >= n/3 && j < 2*n/3 && (i < n/3 || i >= 2*n/3 - 1)))
	graph__add_edge(g, i*n + j, (i+1)*n + j, NORTH);
    }
  }
  return g;
}

/* Creates a snake-shaped graph
 *
 * @param n size of the graph
 * @return a snake-shaped graph with no owners
 */
struct graph_t * graph__create_snake(size_t n)
{
  struct graph_t * g = malloc(sizeof(struct graph_t));
  g->num_vertices = n*n;
  g->t = gsl_spmatrix_alloc(n*n, n*n);
  g->o = gsl_spmatrix_alloc(2, n*n);
  for(size_t i = 0; i < n; i++) {
    for(size_t j = 0; j < n; j++) {
      if (j < n - 1 && ((i*5/n)%2 || ((i < n/5 || i >= 4*n/5) && j < n/5 - 1)
			|| (i >= 2*n/5 && i <= 3*n/5 && j >= 4*n/5)))
	graph__add_edge(g, i*n + j, i*n + j + 1, WEST);
      if (i < n - 1 && (((i < 2*n/5 - 1 || i >= 4*n/5 - 1) && (j < n/5)) || (i >= n/5 && i < 2*n/5 - 1)
			|| (i >= 3*n/5 && i < 4*n/5 - 1) || (i >= n/5 && i < 4*n/5 - 1 && j >= 4*n/5)))
	graph__add_edge(g, i*n + j, (i+1)*n + j, NORTH);
    }
  }
  return g;
}

/* Adds an edge between two vertices in a graph
 *
 * @param graph a graph
 * @param first first vertex
 * @param second second vertex
 * @param d a direction (1:N, 2:S, 3:W, 4:E)
 * @return 0 if the addition is successful, -1 if the edge was already there
 */
int graph__add_edge(struct graph_t * graph, size_t first, size_t second, enum direction d)
{
   if (gsl_spmatrix_ptr(graph->t, first, second) != NULL)
      return -1;
   int edge1 = gsl_spmatrix_set(graph->t, second, first, d);
   int edge2 = gsl_spmatrix_set(graph->t, first, second, d%2?d+1:d-1);
   return (edge1 || edge2);
}

/* Removes an edge between two vertices in a graph
 *
 * @param graph a graph
 * @param first first vertex
 * @param second second vertex
 * @return 0 if the removal is successful, -1 if there was already no edge
 */
int graph__remove_edge(struct graph_t * graph, size_t first, size_t second)
{
  if (gsl_spmatrix_ptr(graph->t, first, second) == NULL)
      return -1;
  int edge1 = gsl_spmatrix_set(graph->t, second, first, 0);
  int edge2 = gsl_spmatrix_set(graph->t, first, second, 0);
  return (edge1 || edge2);
}
