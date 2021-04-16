#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gsl/gsl_spmatrix.h>
#include <gsl/gsl_spblas.h>
#include <gsl/gsl_spmatrix_uint.h>

#define MAX_GRAPH_SIZE 200

#include "graph_modif.h"

/* Initializes an empty square graph
 *
 * @param n size of the graph
 * @return an empty square graph with no owners
 */
struct graph_t * graph__initialize(size_t n)
{
   struct graph_t * graph = malloc(sizeof(struct graph_t));

   gsl_spmatrix_uint * t = gsl_spmatrix_uint_alloc(n*n, n*n);
   gsl_spmatrix_uint * o = gsl_spmatrix_uint_alloc(2,n*n);

   graph->num_vertices = 0;
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
   gsl_spmatrix_uint_free(graph->t);
   gsl_spmatrix_uint_free(graph->o);
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
   g->t = gsl_spmatrix_uint_alloc(n*n, n*n);
   g->o = gsl_spmatrix_uint_alloc(2, n*n);
   for(size_t i = 0; i < n*n; i++) 
   {
      // Adding left-right edges until right border
      if (i%n != n-1) graph__add_edge(g, i, i + 1, EAST);
      // Adding top-bottom edges until bottom border
      if (i + n < n*n) graph__add_edge(g, i, i + n, SOUTH);
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
   g->num_vertices = n*n*8/9;
   g->t = gsl_spmatrix_uint_alloc(n*n, n*n);
   g->o = gsl_spmatrix_uint_alloc(2, n*n);
   for(size_t i = 0; i < n; i++) 
   {
      for(size_t j = 0; j < n; j++) 
      {
	 // Adding left-right edges if not in the center square
	 if (j < n - 1 && !(i >= n/3 && i < 2*n/3 && j >= n/3 - 1 && j < 2*n/3))
	    graph__add_edge(g, i*n + j, i*n + j + 1, EAST);
	 // Adding top-bottom edges if not in the center square
	 if (i < n - 1 && !(j >= n/3 && j < 2*n/3 && i >= n/3 - 1 && i < 2*n/3))
	    graph__add_edge(g, i*n + j, (i+1)*n + j, SOUTH);
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
   g->num_vertices = n*n*7/9;
   g->t = gsl_spmatrix_uint_alloc(n*n, n*n);
   g->o = gsl_spmatrix_uint_alloc(2, n*n);
   for(size_t i = 0; i < n; i++)
   {
      for(size_t j = 0; j < n; j++)
      {
	 // Adding left-right edges if not in top-center or bottom-center square
	 if (j < n - 1 && !((i < n/3 || i >= 2*n/3) && (j >= n/3 - 1 && j < 2*n/3)))
	    graph__add_edge(g, i*n + j, i*n + j + 1, EAST);
	 // Adding top-bottom edges if not in top-center or bottom-center square
	 if (i < n - 1 && !((j >= n/3 && j < 2*n/3) && (i < n/3 || i >= 2*n/3 - 1)))
	    graph__add_edge(g, i*n + j, (i+1)*n + j, SOUTH);
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
   g->num_vertices = n*n*13/25;
   g->t = gsl_spmatrix_uint_alloc(n*n, n*n);
   g->o = gsl_spmatrix_uint_alloc(2, n*n);
   for(size_t i = 0; i < n; i++)
   {
      for(size_t j = 0; j < n; j++)
      {
	 // Adding left-right edges if in the snake shape
	 if (j < n - 1 && ((i*5/n)%2 || ((i < n/5 || i >= 4*n/5) && j < n/5 - 1)
			   || (i >= 2*n/5 && i <= 3*n/5 && j >= 4*n/5)))
	    graph__add_edge(g, i*n + j, i*n + j + 1, EAST);
	 // Adding top-bottom edges if in the snake shape
	 if (i < n - 1 && (((i < 2*n/5 - 1 || i >= 4*n/5 - 1) && (j < n/5)) 
			   || (i >= n/5 && i < 2*n/5 - 1)
			   || (i >= 3*n/5 && i < 4*n/5 - 1) 
			   || (i >= n/5 && i < 4*n/5 - 1 && j >= 4*n/5)))
	    graph__add_edge(g, i*n + j, (i+1)*n + j, SOUTH);
      }
   }
   return g;
}

/* Gets the neighboor of a vertex
 *
 * @param graph a graph
 * @param v a vertex
 * @param d a direction (1:N, 2:S, 3:W, 4:E)
 * @return the neighboor number, -1 if there is no neighboor
 */
int graph__get_neighboor(struct graph_t * graph, size_t n, size_t v, enum direction d)
{
   for (size_t i = 0; i < n*n; i++)
      if ((int)gsl_spmatrix_uint_get(graph->t, v, i) == d)
			return i;
   return -1;
}

/* Copies a graph
 *
 * @param graph the graph to be copied
 * @param n number of vertices
 * @return a copy of the graph
 */
struct graph_t * graph__copy(struct graph_t * graph, size_t n)
{
	struct graph_t * newgraph = malloc(sizeof(struct graph_t));
  	newgraph->num_vertices = graph->num_vertices;
  	newgraph->t = gsl_spmatrix_uint_alloc(n*n, n*n);
  	newgraph->o = gsl_spmatrix_uint_alloc(2, n*n);
  	gsl_spmatrix_uint_memcpy(newgraph->t, graph->t);
  	gsl_spmatrix_uint_memcpy(newgraph->o, graph->o);
  	return newgraph;
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
   if(d == ERROR_DIRECTION) return -1;
   if (gsl_spmatrix_uint_get(graph->t, first, second) != 0)
      return -1;
   int edge2 = gsl_spmatrix_uint_set(graph->t, first, second, d);
   int edge1 = gsl_spmatrix_uint_set(graph->t, second, first, d%2?d+1:d-1);
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
   if(gsl_spmatrix_uint_get(graph->t, first, second) == 0 
   	&& gsl_spmatrix_uint_get(graph->t, second, first) == 0)
   	return -1;
   int edge1 = gsl_spmatrix_uint_set(graph->t, second, first, 0);
   int edge2 = gsl_spmatrix_uint_set(graph->t, first, second, 0);
   return (edge1 || edge2);
}

/* Checks if there is an edge between two vertices
 *
 * @param graph a graph
 * @param first first vertex
 * @param second second vertex
 * @return 1 if there is an edge, 0 else
 */
int graph__edge_exists(struct graph_t * graph, size_t first, size_t second)
{
	if (gsl_spmatrix_uint_ptr(graph->t, first, second) == NULL)
		return 0;
	int d = gsl_spmatrix_uint_get(graph->t, first, second);
	if (d == 0) return 0;
	int opp = d%2?d+1:d-1; // Opposite direction
	return ((int)gsl_spmatrix_uint_get(graph->t, second, first) == opp);
}

/* Adds an owner to a vertex
 *
 * @param graph a graph
 * @param v the vertex number
 * @param owner the owner number
 * @return 0 if the addition is successful, -1 if the player already owned it
 */
int graph__add_ownership(struct graph_t * graph, size_t v, size_t owner)
{
   if(gsl_spmatrix_uint_get(graph->o, owner, v) == 1) return -1;
   int ownership = gsl_spmatrix_uint_set(graph->o, owner, v, 1);
   return ownership;
}

/* Verifies if a vertex is owned by a color
 *
 * @param graph a graph
 * @param v the vertex number
 * @param c the player number
 * @return 1 if the vertex is owned, 0 else
 */
int graph__is_owned(struct graph_t * graph, size_t v, size_t c)
{
	if(gsl_spmatrix_uint_get(graph->o, c, v) == 1) return 1;
	return 0;
}

/* Gets a list of owned positions for a player
 *
 * @param graph a graph
 * @param n the size of the graph
 * @param c the player number
 * @param *l a size_t list of size n
 * @return nothing
 * @side-effect : l contains max n winning positions
 * @side-effect : if l[0] == n then there is no winning pos
 */
int graph__list_ownership(struct graph_t * graph, size_t n, size_t c, size_t* l)
{
	int num = 0;
	for (size_t i = 0; i < n*n; i++)
		if(graph__is_owned(graph, i, c))
			l[num++] = i;
	return num;
}

/* Gets the size of the graph
 * 
 * @param graph a graph
 * @return the graph size
 */
size_t graph__get_size(struct graph_t * graph)
{
	return sqrt(graph->t->size1);
}

/* Displays a graph for debug usage
 *
 * @param graph a graph
 * @param n the size of the graph
 * @param w white player
 * @param b black player
 * @return nothing
 */
void graph__display(struct graph_t * graph, size_t n, size_t w, size_t b)
{
	printf("\033[1mDisplaying graph : size \033[1;96m%ld\033[1;97m | \033[1;96m%ld\033[1;97m vertices\033[0m\n",
		n, graph->num_vertices);
	size_t east, west, south, north;
	int southlinks[MAX_GRAPH_SIZE] = {0};

	// Finding winning positions
	size_t wp_1[MAX_GRAPH_SIZE] = {0};
	int nb_wp_1 = 0;
	size_t wp_2[MAX_GRAPH_SIZE] = {0};
	int nb_wp_2 = 0;
	int winning_pos;
	printf("\033[1mWinning positions (\033[1;96mwhite\033[1;97m, \033[1;91mblack\033[0;97m) : ");
	for(size_t i = 0; i < n*n; i++)
	{
		if(gsl_spmatrix_uint_get(graph->o, 0, i) == 1){
			wp_1[nb_wp_1] = i;
			nb_wp_1++;
			printf("\033[1;96m%ld\033[0;97m,", i);
		}
		if(gsl_spmatrix_uint_get(graph->o, 1, i) == 1){
			wp_2[nb_wp_2] = i;
			nb_wp_2++;
			printf("\033[1;91m%ld\033[0;97m,", i);
		}
	}
	printf("\n");


	for(size_t v = 0; v < n*n; v++)
	{
		east = graph__get_neighboor(graph, n, v, EAST);
		west = graph__get_neighboor(graph, n, v, WEST);
		south = graph__get_neighboor(graph, n, v, SOUTH);
		north = graph__get_neighboor(graph, n, v, NORTH);

		// Non-linked vertices
		if((int)east+(int)west+(int)south+(int)north == -4)
			printf(" -- ");
		// Accessible vertices
		else
		{
			winning_pos = 0;
			for(int i = 0; i < nb_wp_1; i++)
			{
				if(v == wp_1[i]) winning_pos += 1;
			}
			for(int i = 0; i < nb_wp_2; i++)
			{
				if(v == wp_2[i]) winning_pos += 2;
			}

			// Color modifiers
			if((v==b || v==w) && (w!=0 || b!=0))
			{
				if(v == w)				printf("[\033[7;97m%2ld\033[0;97m]", v); //White player
				if(v == b)				printf("[\033[7;95m%2ld\033[0;97m]", v); //Black player
			}
			else
			{
				if(winning_pos == 0)	printf("[\033[0;92m%2ld\033[0;97m]", v); //Random vertex
				if(winning_pos == 1)	printf("[\033[1;96m%2ld\033[0;97m]", v); //Winning pos for p1
				if(winning_pos == 2)	printf("[\033[1;91m%2ld\033[0;97m]", v); //Winning pos for p2
				if(winning_pos == 3)	printf("[\033[7;33m%2ld\033[0;97m]", v); //Winning pos for both
			}

		}

		// East links
		if(v%n < n-1)
		{
			if((int)east != -1) printf("<>"); 
			else printf("  "); 
		}
		
		// South links addition
		if(v/n < n-1)
		{
			if((int)south != -1) southlinks[v%n] = 1;
		}
		
		// South links printing
		if(v%n == n-1 && v/n != n-1)
		{
			printf("\n");
			for(size_t i = 0; i < n; i++)
			{
				if(southlinks[i] == 0)
					printf("    ");
				else
					printf(" || ");
				southlinks[i] = 0;
				printf("  ");
			}
			printf("\n");
		}
	}
	printf("\n");
}
