#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "../src/graph_modif.h"
#include "test_graph_struct.h"

#define TESTCASE(msg, test) printf("%-100s : %s\n", msg, test ? "\033[1;92mPassed\033[0m" : "\033[1;91mFailed\033[0m")


void test__graph_initialize()
{
	struct graph_t * graph = graph__initialize(4);
	TESTCASE("- initialize | graph has no vertices no matter its size", graph->num_vertices == 0);
	for(size_t i = 0; i < 4; i++)
	{
		for(size_t j = 0; j < 4; j++)
		{
			assert(gsl_spmatrix_uint_get(graph->t, i,j) == 0);
		}
	}
	TESTCASE("- initialize | graph has no edges", 1);
	graph__free(graph);
}

void test__graph_create_square()
{
	struct graph_t * graph = graph__create_square(1);
	TESTCASE("- create_square | graph with size 1 has 1 vertex", graph->num_vertices == 1);
	TESTCASE("- create_square | graph with size 1 has no edges", gsl_spmatrix_uint_get(graph->t, 0, 0) == 0);
	graph__free(graph);
	struct graph_t * graph2 = graph__create_square(4);
	TESTCASE("- create_square | graph with size 4 has 16 vertices", graph2->num_vertices == 16);
	for(size_t i = 0; i < 16; i+=4)
	{
		if(i%4 < 3) assert(gsl_spmatrix_uint_get(graph2->t, i, i+1) == EAST);
		if(i%4 > 0) assert(gsl_spmatrix_uint_get(graph2->t, i, i-1) == WEST);
		if(i < 12) assert(gsl_spmatrix_uint_get(graph2->t, i, i+4) == SOUTH);
		if(i > 3) assert(gsl_spmatrix_uint_get(graph2->t, i, i-4) == NORTH);
	}
	TESTCASE("- create_square | graph with size 4 has all of its edges", 1);
	graph__free(graph2);
}

void test__graph_create_torus()
{
	struct graph_t * graph = graph__create_torus(6);
	TESTCASE("- create_torus | right-sized graph has right nb of vertices", graph->num_vertices == 32);
	for(size_t i = 0; i < 36; i+=6)
	{
		// if(i%6 < 5 && ((i/6==2)&&(i<14||i>16)) && ((i/6==3)&&(i<20||i>22))) 
		// 	assert(gsl_spmatrix_uint_get(graph->t, i, i+1) == EAST);
		// if(i%6 > 0 && ((i/6==2)&&(i<15||i>17)) && ((i/6==3)&&(i<21||i>23))) 
		// 	assert(gsl_spmatrix_uint_get(graph->t, i, i-1) == WEST);
		// if(i < 30  && ((i%6==2)&&(||)) 
			//assert(gsl_spmatrix_uint_get(graph->t, i, i+6) == SOUTH);
		//if(i > 5  || (1==1)) assert(gsl_spmatrix_uint_get(graph->t, i, i-6) == NORTH);
	}
	TESTCASE("- create_torus | right-sized graph has all of its edges (td)", 0);
	TESTCASE("- create_torus | right-sized graph has its hole (todo)", 0);
	graph__free(graph);
}

void test__graph_create_chopped()
{
	TESTCASE("- (todo)", 0);
	assert(1 == 1);
}

void test__graph_create_snake()
{
	TESTCASE("- (todo)", 0);
	assert(1 == 1);
}

void test__graph_copy()
{
	struct graph_t * graph = graph__create_square(4);
	struct graph_t * graph2 = graph__copy(graph, 4);
	TESTCASE("- copy | copied graph has the same nb of vertices", graph2->num_vertices == graph->num_vertices);

	for(size_t i = 0; i < 4; i++)
	{
		for(size_t j = 0; j < 4; j++)
		{
			assert(gsl_spmatrix_uint_get(graph2->t, i, j) == gsl_spmatrix_uint_get(graph->t, i, j));
		}
	}
	TESTCASE("- copy | copied graph has the same edges", 1);
	graph__free(graph);
	graph__free(graph2);
}

void test__graph_get_neighboor()
{
	struct graph_t * graph = graph__create_square(4);
	TESTCASE("- get_neighboor | existing neighboor is found", graph__get_neighboor(graph, 4, 2, EAST) == 3);
	gsl_spmatrix_uint_set(graph->t, 2, 3, 0);
	TESTCASE("- get_neighboor | non-existing neighboor is not found", graph__get_neighboor(graph, 4, 2, EAST) == -1);
	TESTCASE("- get_neighboor | out-of-bounds neighboor is not found", graph__get_neighboor(graph, 4, 3, EAST) == -1);
	graph__free(graph);
}

void test__graph_add_edge()
{
	struct graph_t * graph = graph__initialize(4);
	// Left to right
	TESTCASE("- add_edge | impossible direction is not added", graph__add_edge(graph,5,6,ERROR_DIRECTION) == -1);
	TESTCASE("- add_edge | non-existing edge is added", graph__add_edge(graph,0,1,EAST) == 0);
	TESTCASE("- add_edge | already-existing edge is not added", graph__add_edge(graph,0,1,EAST) == -1);
	TESTCASE("- add_edge | first vertex has the correct directionnal link", gsl_spmatrix_uint_get(graph->t, 0, 1) == EAST);
	TESTCASE("- add_edge | second vertex has the opposite link", gsl_spmatrix_uint_get(graph->t, 1, 0) == WEST);
	// Top to bottom
	graph__add_edge(graph,0,4,SOUTH);
	assert(gsl_spmatrix_uint_get(graph->t, 0, 4) == SOUTH);
	assert(gsl_spmatrix_uint_get(graph->t, 4, 0) == NORTH);
	// Right to left
	graph__add_edge(graph,2,1,WEST);
	assert(gsl_spmatrix_uint_get(graph->t, 2, 1) == WEST);
	assert(gsl_spmatrix_uint_get(graph->t, 1, 2) == EAST);
	// Bottom to top
	graph__add_edge(graph,1,5,NORTH);
	assert(gsl_spmatrix_uint_get(graph->t, 1, 5) == NORTH);
	assert(gsl_spmatrix_uint_get(graph->t, 5, 1) == SOUTH);

	TESTCASE("- add_edge | all 4 directions are working", 1);
	graph__free(graph);
}

void test__graph_remove_edge()
{
	struct graph_t * graph = graph__create_square(4);
	// Left to right
	TESTCASE("- remove_edge | existing edge is removed", graph__remove_edge(graph,0,1) == 0);
	TESTCASE("- remove_edge | non-existing edge is not removed", graph__remove_edge(graph,0,1) == -1);
	gsl_spmatrix_uint_set(graph->t, 1, 0, WEST);
	TESTCASE("- remove_edge | one-sided edge is still removed", graph__remove_edge(graph,0,1) == 0);
	TESTCASE("- remove_edge | first vertex has lost directionnal link", gsl_spmatrix_uint_get(graph->t, 0, 1) == 0);
	TESTCASE("- remove_edge | second vertex has lost the opposite link", gsl_spmatrix_uint_get(graph->t, 1, 0) == 0);
	graph__free(graph);
}

void test__graph_edge_exists()
{
	struct graph_t * graph = graph__create_square(4);
	TESTCASE("- edge_exists | existing edge is found", graph__edge_exists(graph, 0, 1) == 1);
	TESTCASE("- edge_exists | non-existing edge is not found", graph__edge_exists(graph, 0, 2) == 0);
	gsl_spmatrix_uint_set(graph->t, 1, 0, 0);
	TESTCASE("- edge_exists | one-sided edge (first side) is not found", graph__edge_exists(graph, 0, 1) == 0);
	TESTCASE("- edge_exists | one-sided edge (other side) is not found", graph__edge_exists(graph, 1, 0) == 0);
	graph__free(graph);
}

void test__graph_add_ownership()
{
	struct graph_t * graph = graph__create_square(4);
	TESTCASE("- add_ownership | non-owned position is added", graph__add_ownership(graph, 5, 0) == 0);
	TESTCASE("- add_ownership | already-owned position is not added", graph__add_ownership(graph, 5, 0) == -1);
	TESTCASE("- add_ownership | added position is correct", gsl_spmatrix_uint_get(graph->o, 0, 5) == 1);
	graph__free(graph);
}

void test__graph_is_owned()
{
	struct graph_t * graph = graph__create_square(4);
	graph__add_ownership(graph, 3, 1);
	TESTCASE("- is_owned | non-owned position is not found", graph__is_owned(graph, 2, 1) == 0);
	TESTCASE("- is_owned | ennemy owned position is not found", graph__is_owned(graph, 3, 0) == 0);
	TESTCASE("- is_owned | owned position is found", graph__is_owned(graph, 3, 1) == 1);
	graph__free(graph);
}

void test__graph_list_ownership()
{
	struct graph_t * graph = graph__create_square(4);
	size_t l[4] = {0};
	int n = graph__list_ownership(graph, 4, 1, l);
	for (int i = 0; i < n; i++)
	{
		assert(l[i] == 0);
	}
	TESTCASE("- list_ownership | empty list found with no positions", 1);
	graph__add_ownership(graph, 3, 1);
	n = graph__list_ownership(graph, 4, 1, l);
	TESTCASE("- list_ownership | correct number of positions found", n == 1);
	for (int i = 0; i < n; i++)
	{
		assert(gsl_spmatrix_uint_get(graph->o, 1, l[i]) == 1);
	}
	TESTCASE("- list_ownership | every position found is owned", 1);
	for (size_t i = 0; i < 16; i++)
	{
		for(int j = 0; j < n; j++)
			if(l[j] != i)
				assert(gsl_spmatrix_uint_get(graph->o, 1, i) == 0);
	}
	TESTCASE("- list_ownership | no positions are missing", 1);
	graph__free(graph);
}

void test__graph_count_ownership()
{
	struct graph_t * graph = graph__create_square(4);
	graph__add_ownership(graph, 1, 0);
	graph__add_ownership(graph, 2, 0);
	graph__add_ownership(graph, 13, 1);
	graph__add_ownership(graph, 14, 1);
	graph__add_ownership(graph, 15, 1);
	TESTCASE("- count_ownership | accessible white pos are found", graph__count_ownership(graph, 4, 0) == 2);
	TESTCASE("- count_ownership | accessible black pos are found", graph__count_ownership(graph, 4, 1) == 3);
	graph__remove_edge(graph, 2, 6);
	graph__remove_edge(graph, 14, 10);
	TESTCASE("- count_ownership | non-accessible white pos are not found", graph__count_ownership(graph, 4, 0) == 1);
	TESTCASE("- count_ownership | non-accessible black pos are not found", graph__count_ownership(graph, 4, 1) == 2);
	graph__free(graph);
}

void test__graph_get_size()
{
	struct graph_t * graph = graph__create_square(4);
	TESTCASE("- get_size | correct position is found", graph__get_size(graph) == 4);
	graph__free(graph);
}

void test__graph_display()
{
	struct graph_t * graph = graph__create_chopped(6);
	graph__add_ownership(graph, 5, 0);
	graph__add_ownership(graph, 7, 1);
	graph__add_ownership(graph, 12, 0);
	graph__add_ownership(graph, 12, 1);
	graph__display(graph, 6,10,20);
	graph__free(graph);
}
