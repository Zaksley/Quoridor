#ifndef _QUOR_GRAPH_MODIF_H_
#define _QUOR_GRAPH_MODIF_H_

#include "graph.h"

/* Initializes a graph
 *
 * @param
 * @return en empty graph with no owners
 */
struct graph_t graph__initialize();

/* Frees a graph
 *
 * @param graph the graph to be freed
 */
void graph__free(struct graph_t * graph);

/* Creates a square graph
 *
 * @param n size of the graph
 * @return a full square graph with no owners
 */
struct graph_t graph__create_square(int n);

/* Copies a graph
 *
 * @param graph the graph to be copied
 * @return a copy of the graph
 */
struct graph_t * graph__copy(struct graph_t * graph);

/* Checks if there is an edge between two vertices
 *
 * @param graph a graph
 * @param first first vertex
 * @param second second vertex
 * @return 1 if there is an edge, 0 else
 */
int graph__edge_exists(struct graph_t * graph, size_t first, size_t second);

/* Gets the neighboor of a vertex
 *
 * @param graph a graph
 * @param v a vertex
 * @param d a direction (1:N, 2:S, 3:W, 4:E)
 * @return the neighboor number, -1 if there is no neighboor
 */
int graph__get_neighboor(struct graph_t * graph, size_t v, size_t d);

/* Adds an edge between two vertices in a graph
 *
 * @param graph a graph
 * @param first first vertex
 * @param second second vertex
 * @return 0 if the addition is successful, -1 if the edge was already there
 */
int graph__add_edge(struct graph_t * graph, size_t first, size_t second);

/* Removes an edge between two vertices in a graph
 *
 * @param graph a graph
 * @param first first vertex
 * @param second second vertex
 * @return 0 if the removal is successful, -1 if there was already no edge
 */
int graph__remove_edge(struct graph_t * graph, size_t first, size_t second);

/* Adds an owner to a vertex
 *
 * @param graph a graph
 * @param v the vertex number
 * @param owner the owner number
 * @return 0 if the addition is successful, -1 if the player already owned it
 */
int graph__add_ownership(struct graph_t * graph, size_t v, size_t owner);

#endif // _QUOR_GRAPH_MODIF_H_