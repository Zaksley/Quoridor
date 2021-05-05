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

	for(size_t i=0; i<size; i++)
	{
		graph__add_ownership(graph, i, BLACK);
		graph__add_ownership(graph, graph->num_vertices - size + i, WHITE);

		graph__add_ownership(graph2, i, BLACK);
		graph__add_ownership(graph2, graph->num_vertices - size + i, WHITE);
	}	
   struct player* p = initialize_test_player(graph, size, pos_black, pos_white);
	struct player* p2 = initialize_test_player(graph2, size, pos_white, pos_black);
	
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
void test__dijkstra()
{
   		//	=== Initialize graph test ===
	size_t size = 4; 
	size_t pos_white = 15;
	size_t pos_black = 0; 
   struct graph_t* graph = graph__create_square(size);
   struct graph_t* graph2 = graph__copy(graph, size);

	for(size_t i=0; i<size; i++)
	{
		graph__add_ownership(graph, i, BLACK);
		graph__add_ownership(graph, graph->num_vertices - size + i, WHITE);

		graph__add_ownership(graph2, i, BLACK);
		graph__add_ownership(graph2, graph->num_vertices - size + i, WHITE);
	}	
   struct player* p = initialize_test_player(graph, size, pos_black, pos_white);

   struct moves_valids* path = dijkstra(p->graph, p->n, p->pos, p->id, p->winning_nodes, p->numb_win);
   TESTCASE("- dijkstra | length path for black player = 3", path->number == 4); 
   TESTCASE("- dijkstra | move 0 is the position of the black player", path->valid[0].m == pos_black);
   TESTCASE("- dijkstra | move 1 is the position 4", path->valid[1].m == 4);
   TESTCASE("- dijkstra | move 2 is the position 8", path->valid[2].m == 8); 
   TESTCASE("- dijkstra | move 3 is the position 12", path->valid[3].m == 12);

   path = dijkstra(p->graph, p->n, p->ennemy_pos, other_player(p->id), p->owned_nodes, p->numb_win);
   TESTCASE("- dijkstra | length path = 3", path->number == 4); 
   TESTCASE("- dijkstra | move 0 is the position of the white player", path->valid[0].m == pos_white);
   TESTCASE("- dijkstra | move 1 is the position 11", path->valid[1].m == 11);
   TESTCASE("- dijkstra | move 2 is the position 7", path->valid[2].m == 7); 
   TESTCASE("- dijkstra | move 3 is the position 3", path->valid[3].m == 3);

   
		// Initialize wall
	struct move_t wall = {.t = WALL, .c = WHITE, .m = 4}; 
	struct edge_t e1_t1 = {0, 4};
	struct edge_t e2_t1 = {1, 5}; 
	wall.e[0] = e1_t1; 
	wall.e[1] = e2_t1;

	put_wall(p, wall);

   printf("\033[2mAdding Wall {%ld-%ld, %ld-%ld} \033[0m\n", wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to); 
	path = dijkstra(p->graph, p->n, p->pos, p->id, p->winning_nodes, p->numb_win);
	TESTCASE("- dijkstra | length path for black player = 5", path->number == 6); 
   TESTCASE("- dijkstra | move 0 is the position of the black player", path->valid[0].m == pos_black);
   TESTCASE("- dijkstra | move 1 is the position 1", path->valid[1].m == 1);
   TESTCASE("- dijkstra | move 2 is the position 2", path->valid[2].m == 2); 
   TESTCASE("- dijkstra | move 3 is the position 6", path->valid[3].m == 6);
   TESTCASE("- dijkstra | move 4 is the position 10", path->valid[4].m == 10); 
   TESTCASE("- dijkstra | move 5 is the position 14", path->valid[5].m == 14);

   struct edge_t e1_t2 = {6, 10};
	struct edge_t e2_t2 = {7, 11}; 
	wall.e[0] = e1_t2; 
	wall.e[1] = e2_t2;

	put_wall(p, wall);

   printf("\033[2mAdding Wall {%ld-%ld, %ld-%ld} \033[0m\n", wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to); 
	path = dijkstra(p->graph, p->n, p->pos, p->id, p->winning_nodes, p->numb_win);
	TESTCASE("- dijkstra | length path for black player = 6", path->number == 7); 
   TESTCASE("- dijkstra | move 0 is the position of the black player", path->valid[0].m == pos_black);
   TESTCASE("- dijkstra | move 1 is the position 1", path->valid[1].m == 1);
   TESTCASE("- dijkstra | move 2 is the position 2", path->valid[2].m == 2); 
   TESTCASE("- dijkstra | move 3 is the position 6", path->valid[3].m == 6);
   TESTCASE("- dijkstra | move 4 is the position 5", path->valid[4].m == 5); 
   TESTCASE("- dijkstra | move 5 is the position 5", path->valid[5].m == 9); 
   TESTCASE("- dijkstra | move 6 is the position 14", path->valid[6].m == 13);

   path = dijkstra(p->graph, p->n, p->ennemy_pos, other_player(p->id), p->owned_nodes, p->numb_win);

   TESTCASE("- dijkstra | length path for white player = 6", path->number == 7); 
   TESTCASE("- dijkstra | move 0 is the position of the white player", path->valid[0].m == pos_white);
   TESTCASE("- dijkstra | move 1 is the position 14 or 11", path->valid[1].m == 14 || path->valid[1].m == 11);
   TESTCASE("- dijkstra | move 2 is the position 13 or 10", path->valid[2].m == 13 || path->valid[2].m == 10); 
   TESTCASE("- dijkstra | move 3 is the position 9", path->valid[3].m == 9);
   TESTCASE("- dijkstra | move 4 is the position 5", path->valid[4].m == 5); 
   TESTCASE("- dijkstra | move 5 is the position 6", path->valid[5].m == 6); 
   TESTCASE("- dijkstra | move 6 is the position 2", path->valid[6].m == 2);

   graph__display(graph, p->n, pos_white, pos_black);

   

   free(path->valid);
   free(path);

}



















//==========================================

/*
*
*/
/*
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
*/