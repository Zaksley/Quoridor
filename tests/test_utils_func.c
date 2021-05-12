#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "../src/utils.h"
#include "test_utils_func.h"

#define TESTCASE(msg, test) printf("%-100s : %s\n", msg, test ? "\033[1;92mPassed\033[0m" : "\033[1;91mFailed\033[0m")


int is_in(size_t e, size_t n, size_t *t) 
{
	for(size_t i = 0; i < n; i++)
		if(t[i] == e)
			return 1;
	return 0;
}

/* Tests made for function valid_positions()
*
* Player in a corner
* Player in the center
* Player in the center in front of the ennemy player
* Player in the center in front of the ennemy player and wall in front
* Player in the center in front of the ennemy player and wall in front + wall on right side
* Player in the center in front of the ennemy player and wall in front + walls on sides
*
*/
void test__valid_positions(int v)
{
   	//	=== Initialize graph test ===
	size_t size = 5; 
	struct graph_t* graph = graph__create_square(size); 
	size_t pos_white = 0;
	size_t pos_black = 12; 

	for(size_t i=0; i<size; i++)
	{
		graph__add_ownership(graph, i, BLACK);
		graph__add_ownership(graph, graph->num_vertices - size + i, WHITE);
	}	

	struct player* p = initialize_test_player(graph, size, pos_white, pos_black, WHITE);
   struct moves_valids* moves = valid_positions(p);
	   //	=== Initialize graph test ===


      // Player in a corner
   printf("\033[2mPlayer in a corner \033[0m\n"); 
   if (v)
   {
      TESTCASE("Only 2 positions found in a corner", moves->number == 2);
      TESTCASE("Position 1 in valids moves", moves->valid[0].m == 1 || moves->valid[1].m == 1);
      TESTCASE("Position 5 in valids moves", moves->valid[0].m == 5 || moves->valid[1].m == 5);
   }
	
      // Player on a border
   printf("\033[2mPlayer on a border\033[0m\n"); 
   pos_white = 10; 
   p->pos = pos_white;
   free_moves_valids(moves);
	moves = valid_positions(p);
   if (v)
   {
      TESTCASE("Only 3 positions found on a border", moves->number == 3);
      TESTCASE("Position 5 in valids moves", moves->valid[0].m == 5 || moves->valid[1].m == 5 || moves->valid[2].m == 5);
      TESTCASE("Position 11 in valids moves", moves->valid[0].m == 11 || moves->valid[1].m == 11 || moves->valid[2].m == 11);
      TESTCASE("Position 15 in valids moves", moves->valid[0].m == 15 || moves->valid[1].m == 15 || moves->valid[2].m == 15);
   }

      // Player in the center
   pos_white = 12; 
   p->pos = pos_white;
   free_moves_valids(moves);
	moves = valid_positions(p);
   printf("\033[2mPlayer in the center\033[0m\n"); 
   if (v)
   {
      TESTCASE("4 positions found on a border", moves->number == 4);
      TESTCASE("Position 7 in valids moves", moves->valid[0].m == 7 || moves->valid[1].m == 7 || moves->valid[2].m == 7 || moves->valid[3].m == 7);
      TESTCASE("Position 11 in valids moves", moves->valid[0].m == 11 || moves->valid[1].m == 11 || moves->valid[2].m == 11 || moves->valid[3].m == 11);
      TESTCASE("Position 13 in valids moves", moves->valid[0].m == 13 || moves->valid[1].m == 13 || moves->valid[2].m == 13 || moves->valid[3].m == 13);
      TESTCASE("Position 17 in valids moves", moves->valid[0].m == 17 || moves->valid[1].m == 17 || moves->valid[2].m == 17 || moves->valid[3].m == 17);
   }

      // Player in the center in front of the ennemy player
   pos_white = 12; 
   pos_black = 17;
   p->pos = pos_white;
   p->ennemy_pos = pos_black; 
   free_moves_valids(moves);
	moves = valid_positions(p);
   printf("\033[2mPlayer in the center in front of the ennemy player\033[0m\n"); 
   if (v)
   {
      TESTCASE("4 positions found on a border", moves->number == 4);
      TESTCASE("Position 7 in valids moves", moves->valid[0].m == 7 || moves->valid[1].m == 7 || moves->valid[2].m == 7 || moves->valid[3].m == 7);
      TESTCASE("Position 11 in valids moves", moves->valid[0].m == 11 || moves->valid[1].m == 11 || moves->valid[2].m == 11 || moves->valid[3].m == 11);
      TESTCASE("Position 13 in valids moves", moves->valid[0].m == 13 || moves->valid[1].m == 13 || moves->valid[2].m == 13 || moves->valid[3].m == 13);
      TESTCASE("Position 22 in valids moves", moves->valid[0].m == 22 || moves->valid[1].m == 22 || moves->valid[2].m == 22 || moves->valid[3].m == 22);
   }

      // Player in the center in front of the ennemy player and wall in front
   struct move_t wall = {.t = WALL, .m = (size_t) -1, .c = WHITE, .e = {(struct edge_t) {17, 22}, (struct edge_t) {18, 23}} };
   put_wall(p, wall);
   free_moves_valids(moves);
	moves = valid_positions(p);
   printf("\033[2mPlayer in the center in front of the ennemy player and wall in front \033[0m\n"); 
   if (v)
   {
      TESTCASE("5 positions found on a border", moves->number == 5);
      TESTCASE("Position 7 in valids moves", moves->valid[0].m == 7 || moves->valid[1].m == 7 || moves->valid[2].m == 7 || moves->valid[3].m == 7 || moves->valid[4].m == 7);
      TESTCASE("Position 11 in valids moves", moves->valid[0].m == 11 || moves->valid[1].m == 11 || moves->valid[2].m == 11 || moves->valid[3].m == 11 || moves->valid[4].m == 11);
      TESTCASE("Position 13 in valids moves", moves->valid[0].m == 13 || moves->valid[1].m == 13 || moves->valid[2].m == 13 || moves->valid[3].m == 13 || moves->valid[4].m == 13);
      TESTCASE("Position 16 in valids moves", moves->valid[0].m == 16 || moves->valid[1].m == 16 || moves->valid[2].m == 16 || moves->valid[3].m == 16 || moves->valid[4].m == 16);
      TESTCASE("Position 18 in valids moves", moves->valid[0].m == 18 || moves->valid[1].m == 18 || moves->valid[2].m == 18 || moves->valid[3].m == 18 || moves->valid[4].m == 18);
   }

      // Player in the center in front of the ennemy player and wall in front + wall on right side
   wall.e[0] = (struct edge_t) {12, 13};
   wall.e[1] = (struct edge_t) {17, 18};
   put_wall(p, wall);
   free_moves_valids(moves);
	moves = valid_positions(p);
   printf("\033[2mPlayer in the center in front of the ennemy player and wall in front + wall on right side \033[0m\n"); 
   if (v)
   {
      TESTCASE("3 positions found", moves->number == 3);
      TESTCASE("Position 7 in valids moves", moves->valid[0].m == 7 || moves->valid[1].m == 7 || moves->valid[2].m == 7);
      TESTCASE("Position 11 in valids moves", moves->valid[0].m == 11 || moves->valid[1].m == 11 || moves->valid[2].m == 11);
      TESTCASE("Position 16 in valids moves", moves->valid[0].m == 16 || moves->valid[1].m == 16 || moves->valid[2].m == 16);
   }

      // Player in the center in front of the ennemy player and wall in front + walls on sides
   wall.e[0] = (struct edge_t) {11, 12};
   wall.e[1] = (struct edge_t) {16, 17};


   put_wall(p, wall);
   free_moves_valids(moves);
	moves = valid_positions(p);
   printf("\033[2mPlayer in the center in front of the ennemy player and wall in front + walls on sides \033[0m\n"); 
   if (v)
   {
      TESTCASE("Only 1 position found", moves->number == 1);
      TESTCASE("Position 7 in valids moves", moves->valid[0].m == 7);
   }

   //graph__display(graph, size, pos_white, pos_black);

	// === Free tests
	finalization_player(*p);
	free(p);
	free_moves_valids(moves);
	// ===
}

void test__valid_walls(int v)
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

	struct player* p = initialize_test_player(graph, size, pos_white, pos_black, WHITE);

	//	=== Initialize graph test ===

      // Basic test
	struct moves_valids* moves; 
	moves = valid_walls(p); 
   int walls_possible = 32; 
	if (v) TESTCASE("- valid_walls | Square 5*5 => 32 walls available", moves->number == walls_possible); 

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
	free_moves_valids(moves);
	moves = valid_walls(p);
   printf("\033[2mAdding Wall {%ld-%ld, %ld-%ld} \033[0m\n", wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to); 
   

   walls_possible -= 3; 
   if (v) TESTCASE("- valid_walls | New wall => -3 walls available", moves->number == walls_possible);
   if (v) TESTCASE("- valid_walls | New wall => Wall {0-1, 5-6} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   // same wall illegal
	e1_illegal.fr = 0;
   e1_illegal.to = 1;
   e2_illegal.fr = 5;
   e2_illegal.to = 6;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => same wall is now illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 1;
   e1_illegal.to = 6;
   e2_illegal.fr = 2;
   e2_illegal.to = 7;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {1-6, 2-7} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   // Adding wall { (12,13) - (17,18) }
   e1.fr = 12;
   e1.to = 13;
   e2.fr = 17;
   e2.to = 18;
	wall.e[0] = e1; 
	wall.e[1] = e2;

   put_wall(p, wall);
	free_moves_valids(moves);
	moves = valid_walls(p);
   printf("\033[2mAdding Wall {%ld-%ld, %ld-%ld} \033[0m\n", wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to); 
   walls_possible -= 4; 
   if (v) TESTCASE("- valid_walls | New wall => -4 walls available", moves->number == walls_possible);

   // same wall
   e1_illegal.fr = 12;
   e1_illegal.to = 13;
   e2_illegal.fr = 17;
   e2_illegal.to = 18;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => same wall is now illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 12;
   e1_illegal.to = 17;
   e2_illegal.fr = 13;
   e2_illegal.to = 18;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {12-17, 13-18} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 7;
   e1_illegal.to = 8;
   e2_illegal.fr = 12;
   e2_illegal.to = 13;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {7-8, 12-13} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 17;
   e1_illegal.to = 18;
   e2_illegal.fr = 22;
   e2_illegal.to = 23;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {17-18, 22-23} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

     // Adding wall { (17,22) - (18,23) }
   e1.fr = 17;
   e1.to = 22;
   e2.fr = 18;
   e2.to = 23;
	wall.e[0] = e1; 
	wall.e[1] = e2;

   put_wall(p, wall);
	free_moves_valids(moves);
	moves = valid_walls(p);
   printf("\033[2mAdding Wall {%ld-%ld, %ld-%ld} \033[0m\n", wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to); 
   walls_possible -= 3; 
   if (v) TESTCASE("- valid_walls | New wall => -3 walls available", moves->number == walls_possible);

   // same wall illegal
   e1_illegal.fr = 17;
   e1_illegal.to = 22;
   e2_illegal.fr = 18;
   e2_illegal.to = 23;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => same wall is now illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 16;
   e1_illegal.to = 21;
   e2_illegal.fr = 17;
   e2_illegal.to = 22;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {16-21, 17-22} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 18;
   e1_illegal.to = 23;
   e2_illegal.fr = 19;
   e2_illegal.to = 24;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {18-23, 19-24} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

       // Adding wall { (8,9) - (13,14) }
   e1.fr = 8;
   e1.to = 9;
   e2.fr = 13;
   e2.to = 14;
	wall.e[0] = e1; 
	wall.e[1] = e2;

   put_wall(p, wall);
	free_moves_valids(moves);
	moves = valid_walls(p);
   printf("\033[2mAdding Wall {%ld-%ld, %ld-%ld} \033[0m\n", wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to); 
   walls_possible -= 4; 
   if (v) TESTCASE("- valid_walls | New wall => -4 walls available", moves->number == walls_possible);

   // same wall illegal
   e1_illegal.fr = 8;
   e1_illegal.to = 9;
   e2_illegal.fr = 13;
   e2_illegal.to = 14;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => same wall is now illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 8;
   e1_illegal.to = 13;
   e2_illegal.fr = 9;
   e2_illegal.to = 14;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {8-13, 9-14} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 3;
   e1_illegal.to = 4;
   e2_illegal.fr = 8;
   e2_illegal.to = 9;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {3-4, 8-9} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 13;
   e1_illegal.to = 14;
   e2_illegal.fr = 18;
   e2_illegal.to = 19;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {13-14, 18-19} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));
   
       // Adding wall { (13,18) - (14,19) }
   e1.fr = 13;
   e1.to = 18;
   e2.fr = 14;
   e2.to = 19;
	wall.e[0] = e1; 
	wall.e[1] = e2;

   put_wall(p, wall);
	free_moves_valids(moves);
	moves = valid_walls(p);
   printf("\033[2mAdding Wall {%ld-%ld, %ld-%ld} \033[0m\n", wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to); 
   walls_possible -= 4; 
   if (v) TESTCASE("- valid_walls | New wall => -4 walls available", moves->number == walls_possible);
	
   // same wall illegal
   e1_illegal.fr = 13;
   e1_illegal.to = 18;
   e2_illegal.fr = 14;
   e2_illegal.to = 19;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Same wall is now illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   // wall => no more path illegal (White player)
   e1_illegal.fr = 15;
   e1_illegal.to = 20;
   e2_illegal.fr = 16;
   e2_illegal.to = 21;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {15-20, 16-21} illegal (no more path)", !wall_in_list(moves->number, moves->valid, wall_illegal));

   // wall => no more path illegal (black player)
   e1_illegal.fr = 2;
   e1_illegal.to = 7;
   e2_illegal.fr = 3;
   e2_illegal.to = 8;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {2-7, 3-8} illegal (no more path)", !wall_in_list(moves->number, moves->valid, wall_illegal));

	 // wall => no more path illegal (white player)
   e1_illegal.fr = 2;
   e1_illegal.to = 3;
   e2_illegal.fr = 7;
   e2_illegal.to = 8;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {2-3, 7-8} illegal (no more path)", !wall_in_list(moves->number, moves->valid, wall_illegal));

	// wall => moving player make the wall available again
	printf("\033[2mMoving black player{8 -> 7} \033[0m\n");
   p->ennemy_pos = 7;
	free_moves_valids(moves);
	moves = valid_walls(p);
   walls_possible += 1; 
   if (v) TESTCASE("- valid_walls | Move player => +1 walls available", moves->number == walls_possible);
   if (v) TESTCASE("- valid_walls | Move player => Wall {2-3, 7-8} legal again", wall_in_list(moves->number, moves->valid, wall_illegal));

   // Adding wall { (18,19) - (23,24) }
   e1.fr = 18;
   e1.to = 19;
   e2.fr = 23;
   e2.to = 24;
	wall.e[0] = e1; 
	wall.e[1] = e2;

   put_wall(p, wall);
	free_moves_valids(moves);
	moves = valid_walls(p);
   printf("\033[2mAdding Wall {%ld-%ld, %ld-%ld} \033[0m\n", wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to); 
   walls_possible -= 1; 
   if (v) TESTCASE("- valid_walls | New wall => -1 walls available", moves->number == walls_possible);

   // same wall illegal
   e1_illegal.fr = 18;
   e1_illegal.to = 19;
   e2_illegal.fr = 23;
   e2_illegal.to = 24;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Same wall is now illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

	// wall => moving back player make the wall illegal again
	e1_illegal.fr = 2;
   e1_illegal.to = 3;
   e2_illegal.fr = 7;
   e2_illegal.to = 8;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
	printf("\033[2mMoving black player{7 -> 8} \033[0m\n");
   p->ennemy_pos = 8;
	free_moves_valids(moves);
	moves = valid_walls(p);
   walls_possible -= 1; 
   if (v) TESTCASE("- valid_walls | Move player => -1 walls available", moves->number == walls_possible);
   if (v) TESTCASE("- valid_walls | Move player => Wall {2-3, 7-8} illegal again", !wall_in_list(moves->number, moves->valid, wall_illegal));

      // Adding wall { (6,7) - (11,12) }
   e1.fr = 6;
   e1.to = 7;
   e2.fr = 11;
   e2.to = 12;
	wall.e[0] = e1; 
	wall.e[1] = e2;

   put_wall(p, wall);
	free_moves_valids(moves);
	moves = valid_walls(p);
   printf("\033[2mAdding Wall {%ld-%ld, %ld-%ld} \033[0m\n", wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to); 
   walls_possible -= 9; 
   if (v) TESTCASE("- valid_walls | New wall => -9 walls available", moves->number == walls_possible);

   // same wall illegal
   e1_illegal.fr = 6;
   e1_illegal.to = 7;
   e2_illegal.fr = 11;
   e2_illegal.to = 12;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Same wall is now illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 1;
   e1_illegal.to = 2;
   e2_illegal.fr = 6;
   e2_illegal.to = 7;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {1-2, 6-7} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 11;
   e1_illegal.to = 12;
   e2_illegal.fr = 16;
   e2_illegal.to = 17;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {11-12, 16-17} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   e1_illegal.fr = 6;
   e1_illegal.to = 11;
   e2_illegal.fr = 7;
   e2_illegal.to = 12;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {6-11, 7-12} illegal", !wall_in_list(moves->number, moves->valid, wall_illegal));

   // wall => no more path illegal (White player)
   e1_illegal.fr = 5;
   e1_illegal.to = 10;
   e2_illegal.fr = 6;
   e2_illegal.to = 11;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {5-10, 6-11} illegal (no more path)", !wall_in_list(moves->number, moves->valid, wall_illegal));

   // wall => no more path illegal (White player)
   e1_illegal.fr = 10;
   e1_illegal.to = 15;
   e2_illegal.fr = 11;
   e2_illegal.to = 16;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {10-15, 11-16} illegal (no more path)", !wall_in_list(moves->number, moves->valid, wall_illegal));

	// wall => no more path illegal (Both player)
   e1_illegal.fr = 7;
   e1_illegal.to = 12;
   e2_illegal.fr = 8;
   e2_illegal.to = 13;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {7-12, 8-13} illegal (no more path)", !wall_in_list(moves->number, moves->valid, wall_illegal));

	// wall => no more path illegal (Both player)
   e1_illegal.fr = 11;
   e1_illegal.to = 16;
   e2_illegal.fr = 12;
   e2_illegal.to = 17;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {11-16, 12-17} illegal (no more path)", !wall_in_list(moves->number, moves->valid, wall_illegal));

	// wall => no more path illegal (Both player)
   e1_illegal.fr = 16;
   e1_illegal.to = 17;
   e2_illegal.fr = 21;
   e2_illegal.to = 22;
	wall_illegal.e[0] = e1_illegal; 
	wall_illegal.e[1] = e2_illegal; 
   if (v) TESTCASE("- valid_walls | New wall => Wall {16-17, 21-22} illegal (no more path)", !wall_in_list(moves->number, moves->valid, wall_illegal));

	//graph__display(graph, size, pos_white, pos_black);

		// === Free tests
	finalization_player(*p);
	free(p);
	free_moves_valids(moves);
		// ===

}

void test__put_wall(int v)
{
	//	=== Initialize graph test ===
	size_t size = 3; 
	size_t pos = 0; 
	size_t ennemy_pos = 8; 
	struct graph_t* graph = graph__create_square(size); 
	struct player* p = initialize_test_player(graph, size, pos, ennemy_pos, BLACK);
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

   if (v)
   {
      TESTCASE("- put_wall | putting initial wall", put_wall(p, wall));
      TESTCASE("- put_wall | edge 1 is destroyed", !graph__edge_exists(graph, n1, n2)); 
      TESTCASE("- put_wall | edge 2 is destroyed", !graph__edge_exists(graph, n3, n4)); 
      TESTCASE("- put_wall | put a wall if there is no edge isn't allowed", put_wall(p, wall) == -1);
   }

		// === Free tests
	finalization_player(*p);
	free(p);
		// ===
}

void test__destroy_wall(int v)
{
   if (v) TESTCASE("todo", 0);
}


void test__exist_path_player(int v)
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
	struct player* p = initialize_test_player(graph, size, pos_black, pos_white, BLACK);
	//	=== Initialize graph test ===

	//graph__display(graph, size, pos_white, pos_black);
	if (v) TESTCASE("- existPath | initially no walls, find a path for white player", existPath_Player(p, WHITE, pos_white));
	if (v) TESTCASE("- existPath | initially no walls, find a path	for black player", existPath_Player(p, BLACK, pos_black)); 
	
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
	if (v) TESTCASE("- existPath | 1 central wall, find a path for white player", existPath_Player(p, WHITE, pos_white));
	if (v) TESTCASE("- existPath | 1 central wall, find a path for black player", existPath_Player(p, BLACK, pos_black)); 

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
	if (v) TESTCASE("- existPath | 3 specific walls, find no path for white player", !existPath_Player(p, WHITE, pos_white));
	if (v) TESTCASE("- existPath | 3 specific walls, find path for black player", existPath_Player(p, BLACK, pos_black)); 

	/*
	*	Put a wall in position 3 direction NORTH and SOUTH
	*	==> No more path for black player
	*/
	struct edge_t e1_test4 = {7, 4};
	struct edge_t e2_test4 = {6, 3}; 
	wall.e[0] = e1_test4;
	wall.e[1] = e2_test4;
	put_wall(p, wall);
	
	if (v) TESTCASE("- existPath | 4 specific walls, find no path for black player", !existPath_Player(p, BLACK, pos_black));

	/*
	*	Putting a wall again
	*	==> Path is again available
	*/
	destroy_wall(p, wall, 1); 
	if (v) TESTCASE("- existPath | removing a wall, find a path for black player", existPath_Player(p, BLACK, pos_black));

		// === Free tests
	finalization_player(*p);
	free(p);
		// ===
}

void test__check_path(int v)
{
	if (v) TESTCASE("todo", 0);
}