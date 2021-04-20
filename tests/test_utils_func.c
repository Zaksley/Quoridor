#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "../src/utils.h"
#include "test_utils_func.h"

#define TESTCASE(msg, test) printf("%-60s : %s\n", msg, test ? "\033[1;92mPassed\033[0m" : "\033[1;91mFailed\033[0m")

struct player* initialize_test_player(size_t n, size_t pos)
{
	struct player* test_player = get_functions("./install/libplayer_move_random.so");
	test_player->pos = pos;
   	test_player->graph = graph__create_square(n);
	test_player->n = n;
	return test_player;
}

int is_in(size_t e, size_t n, size_t *t) 
{
	for(size_t i = 0; i < n; i++)
		if(t[i] == e)
			return 1;
	return 0;
}

void test__valid_positions()
{
	struct player *player = initialize_test_player(5, 0);
	struct moves_valids* moves = valid_positions(player);
	TESTCASE("Only 2 positions found in a corner", moves->number == 2);
	player->pos = 12;
	moves = valid_positions(player);
	TESTCASE("4 positions found in the middle of the board", moves->number== 4);
	size_t neighboors[4] = {7, 11, 13, 17};
	int test = 1;
	if (moves->number < 4)
		test = 0;
	for(int i = 0; test && i < 4; i++)  
		if (!is_in(moves->valid[i].m, 4, neighboors))
			test = 0;
	TESTCASE("Found positions are neighboors", test);

}

void test__valid_walls()
{
	TESTCASE("todo", 0);
}

void test__put_wall()
{
	//	=== Initialize graph test ===
	size_t size = 3; 
	struct graph_t* graph = graph__create_square(size); 
	size_t pos_white = 8;
	size_t pos_black = 0; 
	for(int i=0; i<size; i++)
	{
		graph__add_ownership(graph, i, BLACK);
		graph__add_ownership(graph, graph->num_vertices - size + i, WHITE);
	}	
	
	struct move_t wall = {.t = WALL, .c = WHITE, .m = 4};
	size_t n1, n2, n3, n4;
	n1 = 0; 
	n2 = 1;
	n3 = 3;
	n4 = 4; 

	struct edge_t e1 = {n1, n2};
	struct edge_t e2 = {n3, n4}; 
	wall.e[0] = e1; 
	wall.e[1] = e2; 

	TESTCASE("- put_wall | putting initial wall", put_wall(graph, wall));
	TESTCASE("- put_wall | edge 1 is destroyed", !graph__edge_exists(graph, n1, n2)); 
	TESTCASE("- put_wall | edge 2 is destroyed", !graph__edge_exists(graph, n3, n4)); 
	TESTCASE("- put_wall | put a wall if there is no edge isn't allowed", put_wall(graph, wall) == -1);
}

void test__destroy_wall()
{
	TESTCASE("todo", 0);
}

void test__shift_left()
{

	TESTCASE("todo", 0);
}

//int existPath_Player(struct graph_t* graph, size_t n, size_t color, size_t pos)
void test__exist_path_player()
{	

	//	=== Initialize graph test ===
	size_t size = 3; 
	struct graph_t* graph = graph__create_square(size); 
	size_t pos_white = 8;
	size_t pos_black = 0; 
	for(int i=0; i<size; i++)
	{
		graph__add_ownership(graph, i, BLACK);
		graph__add_ownership(graph, graph->num_vertices - size + i, WHITE);
	}	
	//	=== Initialize graph test ===

	//graph__display(graph, size, pos_white, pos_black);
	TESTCASE("- existPath | initially no walls, find a path for white player", existPath_Player(graph, size, WHITE, pos_white));
	TESTCASE("- existPath | initially no walls, find a path	for black player", existPath_Player(graph, size, BLACK, pos_black)); 
	
	/*
	*	Put a mur in position 4 direction NORTH and SOUTH
	* 	==> No problem for both players
	*/
		// Initialize wall
	struct move_t wall = {.t = WALL, .c = WHITE, .m = 4};

	struct edge_t e1_test1 = {4, 3}; 
	struct edge_t e2_test1 = {0, 1}; 
	wall.e[0] = e1_test1;
	wall.e[1] = e2_test1; 
	put_wall(graph, wall);

	//graph__display(graph, size, pos_white, pos_black);
	TESTCASE("- existPath | 1 central wall, find a path for white player", existPath_Player(graph, size, WHITE, pos_white));
	TESTCASE("- existPath | 1 central wall, find a path for black player", existPath_Player(graph, size, BLACK, pos_black)); 

	/*
	*	Put a wall in position 7 direction EAST and WEST
	*	Put a wall in position 5 direction NORTH and SOUTH
	* 	==> No more path for black player
	*/

	struct edge_t e1_test2 = {7, 6};
	struct edge_t e2_test2 = {4, 3}; 
	wall.e[0] = e1_test2;
	wall.e[1] = e2_test2;
	put_wall(graph, wall);

	struct edge_t e1_test3 = {4, 7};
	struct edge_t e2_test3 = {5, 8}; 
	wall.e[0] = e1_test3;
	wall.e[1] = e2_test3;
	put_wall(graph, wall);
	
	//graph__display(graph, size, pos_white, pos_black);
	TESTCASE("- existPath | 3 specific walls, find no path for white player", !existPath_Player(graph, size, WHITE, pos_white));
	TESTCASE("- existPath | 3 specific walls, find path for black player", existPath_Player(graph, size, BLACK, pos_black)); 

	/*
	*	Put a wall in position 3 direction NORTH and SOUTH
	*	==> No more path for white player
	*/
	struct edge_t e1_test4 = {7, 4};
	struct edge_t e2_test4 = {6, 3}; 
	wall.e[0] = e1_test4;
	wall.e[1] = e2_test4;
	put_wall(graph, wall);
	
	TESTCASE("- existPath | 4 specific walls, find no path for black player", !existPath_Player(graph, size, BLACK, pos_black));
	destroy_wall(graph, wall, 1); 
	TESTCASE("- existPath | removing a wall, find a path for black player", existPath_Player(graph, size, BLACK, pos_black));
}

void test__check_path()
{
	TESTCASE("todo", 0);
}
