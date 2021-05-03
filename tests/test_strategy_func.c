#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "../src/strategy.h"
#include "test_strategy_func.h"

#define TESTCASE(msg, test) printf("%-60s : %s\n", msg, test ? "\033[1;92mPassed\033[0m" : "\033[1;91mFailed\033[0m")


/*
*
*/
void test__rushing_path()
{
	
		//	=== Initialize graph test ===
	size_t size = 3; 
	size_t pos_white = 8;
	size_t pos_black = 0; 
   struct graph_t* graph = graph__create_square(size);
   struct graph_t* graph2 = graph__copy(graph, size);

	struct player* p = initialize_test_player(graph, size, pos_black, pos_white);
	struct player* p2 = initialize_test_player(graph2, size, pos_white, pos_black);
	for(size_t i=0; i<size; i++)
	{
		graph__add_ownership(graph, i, BLACK);
		graph__add_ownership(graph, p->graph->num_vertices - size + i, WHITE);

		graph__add_ownership(graph2, i, BLACK);
		graph__add_ownership(graph2, p->graph->num_vertices - size + i, WHITE);
	}	
	
	size_t* list_WIN_BLACK = malloc(sizeof(size_t) * size); 
	graph__list_ownership(p->graph, size, WHITE, list_WIN_BLACK); 
	p->winning_nodes = list_WIN_BLACK; 
	p->numb_win = size;
	
	size_t* list_WIN_WHITE = malloc(sizeof(size_t) * size); 
	graph__list_ownership(p->graph, size, BLACK, list_WIN_WHITE); 
	p2->winning_nodes = list_WIN_WHITE; 
	p2->numb_win = size;

	//	=== Initialize graph test ===

	// ===== TESTS =====

		// === Test Black player rushing down
	struct moves_valids* moves = valid_positions(p); 
	
	size_t rush = rushing_path(pos_black, p->winning_nodes, p->numb_win, moves); 
	TESTCASE("- rushingPath | Black player rushing straight down (1)", rush == (size_t) 3); 

	pos_black = rush; 
	p->pos = rush; 
	moves = valid_positions(p);
	rush = rushing_path(pos_black, p->winning_nodes, p->numb_win, moves);
	TESTCASE("- rushingPath | Black player rushing straight down (2)", rush == (size_t) 6); 
	pos_black = rush; 
	p->pos = rush; 
		// === Test Black player rushing down

		// === Test White player go around wall
	struct move_t wall = {.t = WALL, .c = WHITE, .m = 4};
	struct edge_t e1 = {7, 4};
	struct edge_t e2 = {5, 8}; 
	wall.e[0] = e1; 
	wall.e[1] = e2; 
	pos_white = 8;
	pos_black = 0; 


	put_wall(p2, wall); 
	moves = valid_positions(p2); 
	rush = rushing_path(p2->pos, p2->winning_nodes, p2->numb_win, moves);
	p2->pos = rush;
	TESTCASE("- rushingPath | White player goes around wall (1)", rush == (size_t) 7);

	moves = valid_positions(p2); 
	rush = rushing_path(p2->pos, p2->winning_nodes, p2->numb_win, moves);
	p2->pos = rush;
	TESTCASE("- rushingPath | White player goes around wall (2)", rush == (size_t) 6);

	moves = valid_positions(p2); 
	rush = rushing_path(p2->pos, p2->winning_nodes, p2->numb_win, moves);
	p2->pos = rush;
	TESTCASE("- rushingPath | White player rushing up", rush == (size_t) 3);


	moves = valid_positions(p2); 
	rush = rushing_path(p2->pos, p2->winning_nodes, p2->numb_win, moves);
	p2->pos = rush;
	TESTCASE("- rushingPath | White player jump onto black player to reach winning", rush == (size_t) 1);

	//graph__display(p2->graph, size, p2->pos, pos_black);

	free(list_WIN_WHITE);
	free(list_WIN_BLACK);
	free(moves->valid);
	free(moves);
	//p->finalize();
	//p2->finalize();
}

/*
*
*/
void test__dijkstra_path() 
{
	//	=== Initialize graph test ===
	size_t size = 3; 
	size_t pos_black = 0; 
   struct graph_t* graph = graph__create_square(size);
	struct player* p = initialize_test_player(graph, size, pos_black, 8);
	for(size_t i=0; i<size; i++)
	{
		graph__add_ownership(p->graph, i, BLACK);
		graph__add_ownership(p->graph, p->graph->num_vertices - size + i, WHITE);
	}	
	
	size_t* list_WIN_BLACK = malloc(sizeof(size_t) * size); 
	graph__list_ownership(p->graph, size, WHITE, list_WIN_BLACK); 
	p->winning_nodes = list_WIN_BLACK; 
	p->numb_win = size;
	
	//	=== Initialize graph test ===

	TESTCASE("- dijkstra path | Square graph at position 0", path_dijkstra(p) == 3);
}