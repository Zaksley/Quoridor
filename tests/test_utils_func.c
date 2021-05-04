#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "../src/utils.h"
#include "test_utils_func.h"

#define TESTCASE(msg, test) printf("%-60s : %s\n", msg, test ? "\033[1;92mPassed\033[0m" : "\033[1;91mFailed\033[0m")


int is_in(size_t e, size_t n, size_t *t) 
{
	for(size_t i = 0; i < n; i++)
		if(t[i] == e)
			return 1;
	return 0;
}

void test__valid_positions()
{
   struct graph_t* graph = graph__create_square(5);
	struct player *player = initialize_test_player(graph, 5, 0, 24);
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
	
	//player->finalize();
}

void test__valid_walls()
{
	//	=== Initialize graph test ===
	size_t size = 5; 
	struct graph_t* graph = graph__create_square(size); 
	size_t pos_white = 5;
	size_t pos_black = 8; 

	for(size_t i=0; i<size; i++)
	{
		graph__add_ownership(graph, i, BLACK);
		graph__add_ownership(graph, graph->num_vertices - size + i, WHITE);
	}	

	struct player* p = initialize_test_player(graph, size, pos_white, pos_black);

	//	=== Initialize graph test ===

      // Basic test
	struct moves_valids* moves = valid_walls(p); 
   int walls_possible = 32; 
	TESTCASE("- valid_walls | Square 5*5 => 32 walls available", moves->number == walls_possible); 

			// Initialize wall
	struct move_t wall = {.t = WALL, .c = WHITE, .m = 4}; 
	struct edge_t e1 = {0, 5};
	struct edge_t e2 = {1, 6}; 
	wall.e[0] = e1; 
	wall.e[1] = e2;

	struct move_t wall_illegal = {.t = WALL, .c = WHITE, .m = 4};
	struct edge_t e1_illegal = {0, 1}; 
	struct edge_t e2_illegal = {5, 6}; 
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 

   // Adding wall { (0,4) - (1,5) }

   put_wall(p, wall);
	moves = valid_walls(p);
   printf("\033[2mAdding Wall {%ld-%ld, %ld-%ld} \033[0m\n", wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to); 
   
   /*
   for(int i=0; i< (p->n-1) * (p->n-1); i++)
	{
		printf("wall installed : [%d] : %d \n", i, p->wall_installed[i]);
	}
   */
   /*
	for(int i=0; i<moves->number; i++)
	{
		printf("Mur available {%ld-%ld, %ld-%ld}\n", moves->valid[i].e[0].fr, moves->valid[i].e[0].to, moves->valid[i].e[1].fr, moves->valid[i].e[1].to);
	}
   */
   
   //printf("nobmre %d\n", moves->number);

   walls_possible -= 3; 
   TESTCASE("- valid_walls | New wall => -3 walls available", moves->number == walls_possible);
   TESTCASE("- valid_walls | New wall => Wall {1-5, 2-6} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   // same wall illegal
	e1_illegal.fr = 0;
   e1_illegal.to = 1;
   e2_illegal.fr = 5;
   e2_illegal.to = 6;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => same wall is now illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 1;
   e1_illegal.to = 6;
   e2_illegal.fr = 2;
   e2_illegal.to = 7;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => Wall {1-6, 2-7} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   // Adding wall { (12,13) - (17,18) }
   e1.fr = 12;
   e1.to = 13;
   e2.fr = 17;
   e2.to = 18;
	wall.e[0] = e1; 
	wall.e[1] = e2;

   put_wall(p, wall);
	moves = valid_walls(p);
   printf("\033[2mAdding Wall {%ld-%ld, %ld-%ld} \033[0m\n", wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to); 
   walls_possible -= 4; 
   TESTCASE("- valid_walls | New wall => -4 walls available", moves->number == walls_possible);

   // same wall
   e1_illegal.fr = 12;
   e1_illegal.to = 13;
   e2_illegal.fr = 17;
   e2_illegal.to = 18;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => same wall is now illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 12;
   e1_illegal.to = 17;
   e2_illegal.fr = 13;
   e2_illegal.to = 18;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => Wall {12-17, 13-18} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 7;
   e1_illegal.to = 8;
   e2_illegal.fr = 12;
   e2_illegal.to = 13;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => Wall {7-8, 12-13} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 17;
   e1_illegal.to = 18;
   e2_illegal.fr = 22;
   e2_illegal.to = 23;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => Wall {17-18, 22-23} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

     // Adding wall { (17,22) - (18,23) }
   e1.fr = 17;
   e1.to = 22;
   e2.fr = 18;
   e2.to = 23;
	wall.e[0] = e1; 
	wall.e[1] = e2;

   put_wall(p, wall);
	moves = valid_walls(p);
   printf("\033[2mAdding Wall {%ld-%ld, %ld-%ld} \033[0m\n", wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to); 
   walls_possible -= 3; 
   TESTCASE("- valid_walls | New wall => -3 walls available", moves->number == walls_possible);

   // same wall illegal
   e1_illegal.fr = 17;
   e1_illegal.to = 22;
   e2_illegal.fr = 18;
   e2_illegal.to = 23;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => same wall is now illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 16;
   e1_illegal.to = 21;
   e2_illegal.fr = 17;
   e2_illegal.to = 22;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => Wall {16-21, 17-22} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 18;
   e1_illegal.to = 23;
   e2_illegal.fr = 19;
   e2_illegal.to = 24;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => Wall {18-23, 19-24} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

       // Adding wall { (8,9) - (13,14) }
   e1.fr = 8;
   e1.to = 9;
   e2.fr = 13;
   e2.to = 14;
	wall.e[0] = e1; 
	wall.e[1] = e2;

   put_wall(p, wall);
	moves = valid_walls(p);
   printf("\033[2mAdding Wall {%ld-%ld, %ld-%ld} \033[0m\n", wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to); 
   walls_possible -= 4; 
   TESTCASE("- valid_walls | New wall => -4 walls available", moves->number == walls_possible);

   // same wall illegal
   e1_illegal.fr = 8;
   e1_illegal.to = 9;
   e2_illegal.fr = 13;
   e2_illegal.to = 14;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => same wall is now illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 8;
   e1_illegal.to = 13;
   e2_illegal.fr = 9;
   e2_illegal.to = 14;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => Wall {8-13, 9-14} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 3;
   e1_illegal.to = 4;
   e2_illegal.fr = 8;
   e2_illegal.to = 9;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => Wall {3-4, 8-9} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 13;
   e1_illegal.to = 14;
   e2_illegal.fr = 18;
   e2_illegal.to = 19;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => Wall {8-13, 9-14} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));
   
       // Adding wall { (13,18) - (14,19) }
   e1.fr = 13;
   e1.to = 18;
   e2.fr = 14;
   e2.to = 19;
	wall.e[0] = e1; 
	wall.e[1] = e2;

   put_wall(p, wall);
	moves = valid_walls(p);
   printf("\033[2mAdding Wall {%ld-%ld, %ld-%ld} \033[0m\n", wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to); 
   walls_possible -= 3; 
   TESTCASE("- valid_walls | New wall => -3 walls available", moves->number == walls_possible);

   // same wall illegal
   e1_illegal.fr = 13;
   e1_illegal.to = 18;
   e2_illegal.fr = 14;
   e2_illegal.to = 19;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => Same wall is now illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   // wall => no more path illegal (White player)
   e1_illegal.fr = 15;
   e1_illegal.to = 20;
   e2_illegal.fr = 16;
   e2_illegal.to = 21;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => Wall {15-20, 16-21} illegal (no more path)", !wall_in_list(moves->number, moves->valid, wall_illegal));

   // wall => no more path illegal (black player)
   e1_illegal.fr = 2;
   e1_illegal.to = 7;
   e2_illegal.fr = 3;
   e2_illegal.to = 8;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => Wall {2-7, 3-8} illegal (no more path)", !wall_in_list(moves->number, moves->valid, wall_illegal));

   // Adding wall { (18,19) - (23,24) }
   e1.fr = 18;
   e1.to = 19;
   e2.fr = 23;
   e2.to = 24;
	wall.e[0] = e1; 
	wall.e[1] = e2;

   put_wall(p, wall);
	moves = valid_walls(p);
   printf("\033[2mAdding Wall {%ld-%ld, %ld-%ld} \033[0m\n", wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to); 
   walls_possible -= 1; 
   TESTCASE("- valid_walls | New wall => -1 walls available", moves->number == walls_possible);

   // same wall illegal
   e1_illegal.fr = 18;
   e1_illegal.to = 19;
   e2_illegal.fr = 23;
   e2_illegal.to = 24;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => Same wall is now illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

      // Adding wall { (6,7) - (11,12) }
   e1.fr = 6;
   e1.to = 7;
   e2.fr = 11;
   e2.to = 12;
	wall.e[0] = e1; 
	wall.e[1] = e2;

   put_wall(p, wall);
	moves = valid_walls(p);
   printf("\033[2mAdding Wall {%ld-%ld, %ld-%ld} \033[0m\n", wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to); 
   walls_possible -= 6; 
   TESTCASE("- valid_walls | New wall => -1 walls available", moves->number == walls_possible);

   // same wall illegal
   e1_illegal.fr = 6;
   e1_illegal.to = 7;
   e2_illegal.fr = 11;
   e2_illegal.to = 12;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => Same wall is now illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 1;
   e1_illegal.to = 2;
   e2_illegal.fr = 6;
   e2_illegal.to = 7;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => Wall {1-2, 6-7} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 11;
   e1_illegal.to = 12;
   e2_illegal.fr = 16;
   e2_illegal.to = 17;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => Wall {11-12, 16-17} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 6;
   e1_illegal.to = 11;
   e2_illegal.fr = 7;
   e2_illegal.to = 12;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => Wall {6-11, 7-12} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   // wall => no more path illegal (White player)
   e1_illegal.fr = 5;
   e1_illegal.to = 10;
   e2_illegal.fr = 6;
   e2_illegal.to = 11;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => Wall {5-10, 6-11} illegal (no more path)", !wall_in_list(moves->number, moves->valid, wall_illegal));

   // wall => no more path illegal (White player)
   e1_illegal.fr = 10;
   e1_illegal.to = 11;
   e2_illegal.fr = 11;
   e2_illegal.to = 16;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   TESTCASE("- valid_walls | New wall => Wall {10-11, 11-16} illegal (no more path)", !wall_in_list(moves->number, moves->valid, wall_illegal));

	//graph__display(graph, size, pos_white, pos_black);

}

void test__put_wall()
{
	//	=== Initialize graph test ===
	size_t size = 3; 
	size_t pos = 0; 
	size_t ennemy_pos = 8; 
	struct graph_t* graph = graph__create_square(size); 
	struct player* p = initialize_test_player(graph, size, pos, ennemy_pos);
	p->graph = graph; 

	for(size_t i=0; i<size; i++)
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

	TESTCASE("- put_wall | putting initial wall", put_wall(p, wall));
	TESTCASE("- put_wall | edge 1 is destroyed", !graph__edge_exists(graph, n1, n2)); 
	TESTCASE("- put_wall | edge 2 is destroyed", !graph__edge_exists(graph, n3, n4)); 
	TESTCASE("- put_wall | put a wall if there is no edge isn't allowed", put_wall(p, wall) == -1);
}

void test__destroy_wall()
{
	TESTCASE("todo", 0);
}


void test__exist_path_player()
{	

	//	=== Initialize graph test ===
	size_t size = 3; 
	struct graph_t* graph = graph__create_square(size); 
	size_t pos_white = 8;
	size_t pos_black = 0; 
	for(size_t i=0; i<size; i++)
	{
		graph__add_ownership(graph, i, BLACK);
		graph__add_ownership(graph, graph->num_vertices - size + i, WHITE);
	}	
	struct player* p = initialize_test_player(graph, size, pos_black, pos_white);
	//	=== Initialize graph test ===

	//graph__display(graph, size, pos_white, pos_black);
	TESTCASE("- existPath | initially no walls, find a path for white player", existPath_Player(p, WHITE, pos_white));
	TESTCASE("- existPath | initially no walls, find a path	for black player", existPath_Player(p, BLACK, pos_black)); 
	
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
	put_wall(p, wall);

	//graph__display(graph, size, pos_white, pos_black);
	TESTCASE("- existPath | 1 central wall, find a path for white player", existPath_Player(p, WHITE, pos_white));
	TESTCASE("- existPath | 1 central wall, find a path for black player", existPath_Player(p, BLACK, pos_black)); 

	/*
	*	Put a wall in position 7 direction EAST and WEST
	*	Put a wall in position 5 direction NORTH and SOUTH
	* 	==> No more path for white player
	*/

	struct edge_t e1_test2 = {7, 6};
	struct edge_t e2_test2 = {4, 3}; 
	wall.e[0] = e1_test2;
	wall.e[1] = e2_test2;
	put_wall(p, wall);

	struct edge_t e1_test3 = {4, 7};
	struct edge_t e2_test3 = {5, 8}; 
	wall.e[0] = e1_test3;
	wall.e[1] = e2_test3;
	put_wall(p, wall);
	
	//graph__display(graph, size, pos_white, pos_black);
	TESTCASE("- existPath | 3 specific walls, find no path for white player", !existPath_Player(p, WHITE, pos_white));
	TESTCASE("- existPath | 3 specific walls, find path for black player", existPath_Player(p, BLACK, pos_black)); 

	/*
	*	Put a wall in position 3 direction NORTH and SOUTH
	*	==> No more path for black player
	*/
	struct edge_t e1_test4 = {7, 4};
	struct edge_t e2_test4 = {6, 3}; 
	wall.e[0] = e1_test4;
	wall.e[1] = e2_test4;
	put_wall(p, wall);
	
	TESTCASE("- existPath | 4 specific walls, find no path for black player", !existPath_Player(p, BLACK, pos_black));

	/*
	*	Putting a wall again
	*	==> Path is again available
	*/
	destroy_wall(p, wall, 1); 
	TESTCASE("- existPath | removing a wall, find a path for black player", existPath_Player(p, BLACK, pos_black));
}

void test__check_path()
{
	TESTCASE("todo", 0);
}