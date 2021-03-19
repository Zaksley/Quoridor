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
void graph__free(struct graph_t * graph){
	gsl_spmatrix_free(graph->t);
	gsl_spmatrix_free(graph->o);
	free(graph);
}

/*