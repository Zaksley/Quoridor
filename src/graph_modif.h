#ifndef _QUOR_GRAPH_MODIF_H_
#define _QUOR_GRAPH_MODIF_H_

#include "graph.h"

enum direction {NORTH=1, SOUTH, WEST, EAST, ERROR_DIRECTION=-1};

/* Initializes an empty square graph
 *
 * @param n size of the graph
 * @return en empty graph with no owners
 */
struct graph_t * graph__initialize(size_t n);

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
struct graph_t * graph__create_square(size_t n);

/* Creates a toric graph
 *
 * @param n size of the graph
 * @return a toric graph with no owners
 */
struct graph_t * graph__create_torus(size_t n);

/* Creates a chopped graph
 *
 * @param n size of the graph
 * @return a H-shaped graph with no owners
 */
struct graph_t * graph__create_chopped(size_t n);

/* Creates a snake-shaped graph
 *
 * @param n size of the graph
 * @return a snake-shaped graph with no owners
 */
struct graph_t * graph__create_snake(size_t n);

/* Copies a graph
 *
 * @param graph the graph to be copied
 * @param n number of vertices
 * @return a copy of the graph
 */
struct graph_t * graph__copy(struct graph_t * graph, size_t n);

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
 * @param n the size of the graph
 * @param v a vertex
 * @param d a direction (1:N, 2:S, 3:W, 4:E)
 * @return the neighboor number, -1 if there is no neighboor
 */
int graph__get_neighboor(struct graph_t * graph, size_t n, size_t v, enum direction d);

/* Get the direction between 2 vertex
 *
 * @param graph a graph
 * @param first first vertex
 * @param second second vertex
 * @return 0 if there is no direction, 1:N, 2:S, 3:W, 4:E
 */
int graph__get_dir(struct graph_t* graph, size_t v1, size_t v2); 

/* Adds an edge between two vertices in a graph
 *
 * @param graph a graph
 * @param first first vertex
 * @param second second vertex
 * @return 0 if the addition is successful, -1 if the edge was already there
 */
int graph__add_edge(struct graph_t * graph, size_t first, size_t second, enum direction d);

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

/* Verifies if a vertex is owned by a color
 *
 * @param graph a graph
 * @param v the vertex number
 * @param c the player number
 * @return 1 if the vertex is owned, 0 else
 */
int graph__is_owned(struct graph_t * graph, size_t v, size_t c);

/* Gets a list of owned positions for a player (containing non-accessible ones)
 *
 * @param graph a graph
 * @param n the size of the graph
 * @param c the player number
 * @param *l a size_t list of size n
 * @return nothing (side-effect on l)
 * @side-effect : l contains max n winning positions
 * @side-effect : if l[0] == n then there is no winning pos
 */
int graph__list_ownership(struct graph_t * graph, size_t n, size_t c, size_t* l);

/* Counts all the accessible owned positions
 *
 * @param graph a graph
 * @param n the size of the graph
 * @param c the player number
 * @return the number of accessible owned positions for player c
 */
int graph__count_ownership(struct graph_t * graph, size_t n, size_t c);

/* Gets the size of the graph
 * 
 * @param graph a graph
 * @return the graph size
 */
size_t graph__get_size(struct graph_t * graph);

/* Checks if one or two walls are at the specified position
 *
 * @param graph a graph
 * @param first a vertex next to the wall
 * @param wall_dir the direction of the wall (NORTH means a vertical upward wall)
 * @param other_side the direction crossing the wall
 * @return 1 if there are walls here, 0 else
 */
int graph__wall_exists(struct graph_t * graph, size_t first, enum direction wall_dir, enum direction other_side);

/* Finds the opposite direction
 *
 * @param d direction
 * @return the opposite direction
 */
enum direction opposite_dir(enum direction d);

/* Displays a graph for debug usage
 *
 * @param graph a graph
 * @param n the size of the graph
 * @param w white player
 * @param b black player
 * @return nothing
 */
void graph__display(struct graph_t * graph, size_t n, size_t w, size_t b);

#endif // _QUOR_GRAPH_MODIF_H_
