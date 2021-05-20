#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "../src/strategy.h"
#include "test_strategy_func.h"

#define TESTCASE(msg, test) printf("%-100s : %s\n", msg, test ? "\033[1;92mPassed\033[0m" : "\033[1;91mFailed\033[0m")


/* Tests made for function rushing_path()
*
*  Without walls - rushing straight down
*  With wall in front of player - going on sides
*  Jump on player to get faster
*
*/
void test__rushing_path(int v)
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
   struct player* p = initialize_test_player(graph, size, pos_black, pos_white, BLACK);
	struct player* p2 = initialize_test_player(graph2, size, pos_white, pos_black, WHITE);

	//	=== Initialize graph test ===

	// ===== TESTS =====

		// === Test Black player rushing down
	struct moves_valids* moves = valid_positions(p);

	size_t rush = rushing_path(pos_black, p->winning_nodes, p->numb_win, moves);
	if (v) TESTCASE("- rushingPath | Black player rushing straight down (1)", rush == (size_t) 3);

	pos_black = rush;
	p->pos = rush;
   free_moves_valids(moves);
	moves = valid_positions(p);
	rush = rushing_path(pos_black, p->winning_nodes, p->numb_win, moves);
	if (v) TESTCASE("- rushingPath | Black player rushing straight down (2)", rush == (size_t) 6);
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
   free_moves_valids(moves);
	moves = valid_positions(p2);
	rush = rushing_path(p2->pos, p2->winning_nodes, p2->numb_win, moves);
	p2->pos = rush;
	if (v) TESTCASE("- rushingPath | White player goes around wall (1)", rush == (size_t) 7);

   free_moves_valids(moves);
	moves = valid_positions(p2);
	rush = rushing_path(p2->pos, p2->winning_nodes, p2->numb_win, moves);
	p2->pos = rush;
	if (v) TESTCASE("- rushingPath | White player goes around wall (2)", rush == (size_t) 6);

   free_moves_valids(moves);
	moves = valid_positions(p2);
	rush = rushing_path(p2->pos, p2->winning_nodes, p2->numb_win, moves);
	p2->pos = rush;
	if (v) TESTCASE("- rushingPath | White player rushing up", rush == (size_t) 3);

   free_moves_valids(moves);
	moves = valid_positions(p2);
	rush = rushing_path(p2->pos, p2->winning_nodes, p2->numb_win, moves);
	p2->pos = rush;
	if (v) TESTCASE("- rushingPath | White player jump onto black player to reach winning", rush == (size_t) 1);

	//graph__display(p2->graph, size, p2->pos, pos_black);

		// === Free tests
	finalization_player(*p);
	finalization_player(*p2);
   free(p);
   free(p2);
   free_moves_valids(moves);
   // ===
}

/* Tests made for function dijkstra
*  All tests are used on both players
*
* Normal Dijkstra
* Dijkstra with wall in front
* Dijkstra with jump on player
*
*/
void test__dijkstra(int v)
{
   		//	=== Initialize graph test ===
	size_t size = 4;
	size_t pos_white = 15;
	size_t pos_black = 0;
   struct graph_t* graph = graph__create_square(size);

	for(size_t i=0; i<size; i++)
	{
		graph__add_ownership(graph, i, BLACK);
		graph__add_ownership(graph, graph->num_vertices - size + i, WHITE);
	}
   struct player* p = initialize_test_player(graph, size, pos_black, pos_white, BLACK);

   struct moves_valids* path = dijkstra(p, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   if (v)
   {
      TESTCASE("- dijkstra | length path for black player = 3", path->number == 4);
      TESTCASE("- dijkstra | move 0 is the position of the black player", path->valid[0].m == pos_black);
      TESTCASE("- dijkstra | move 1 is the position 4", path->valid[1].m == 4);
      TESTCASE("- dijkstra | move 2 is the position 8", path->valid[2].m == 8);
      TESTCASE("- dijkstra | move 3 is the position 12", path->valid[3].m == 12);
   }

   free_moves_valids(path);
   path = dijkstra(p, p->ennemy_pos, p->pos, other_player(p->id), p->owned_nodes, p->numb_win);
   if (v)
   {
      TESTCASE("- dijkstra | length path = 3", path->number == 4);
      TESTCASE("- dijkstra | move 0 is the position of the white player", path->valid[0].m == pos_white);
      TESTCASE("- dijkstra | move 1 is the position 11", path->valid[1].m == 11);
      TESTCASE("- dijkstra | move 2 is the position 7", path->valid[2].m == 7);
      TESTCASE("- dijkstra | move 3 is the position 3", path->valid[3].m == 3);
   }


		// Initialize wall
	struct move_t wall = {.t = WALL, .c = WHITE, .m = 4};
	struct edge_t e1_t1 = {0, 4};
	struct edge_t e2_t1 = {1, 5};
	wall.e[0] = e1_t1;
	wall.e[1] = e2_t1;

	put_wall(p, wall);

   printf("\033[2mAdding Wall {%ld-%ld, %ld-%ld} \033[0m\n", wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to);
   free_moves_valids(path);
	path = dijkstra(p, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   if (v)
   {
      TESTCASE("- dijkstra | length path for black player = 5", path->number == 6);
      TESTCASE("- dijkstra | move 0 is the position of the black player", path->valid[0].m == pos_black);
      TESTCASE("- dijkstra | move 1 is the position 1", path->valid[1].m == 1);
      TESTCASE("- dijkstra | move 2 is the position 2", path->valid[2].m == 2);
      TESTCASE("- dijkstra | move 3 is the position 6", path->valid[3].m == 6);
      TESTCASE("- dijkstra | move 4 is the position 10", path->valid[4].m == 10);
      TESTCASE("- dijkstra | move 5 is the position 14", path->valid[5].m == 14);
   }

   struct edge_t e1_t2 = {6, 10};
	struct edge_t e2_t2 = {7, 11};
	wall.e[0] = e1_t2;
	wall.e[1] = e2_t2;

	put_wall(p, wall);

   printf("\033[2mAdding Wall {%ld-%ld, %ld-%ld} \033[0m\n", wall.e[0].fr, wall.e[0].to, wall.e[1].fr, wall.e[1].to);
   free_moves_valids(path);
	path = dijkstra(p, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   if (v)
   {
      TESTCASE("- dijkstra | length path for black player = 6", path->number == 7);
      TESTCASE("- dijkstra | move 0 is the position of the black player", path->valid[0].m == pos_black);
      TESTCASE("- dijkstra | move 1 is the position 1", path->valid[1].m == 1);
      TESTCASE("- dijkstra | move 2 is the position 2", path->valid[2].m == 2);
      TESTCASE("- dijkstra | move 3 is the position 6", path->valid[3].m == 6);
      TESTCASE("- dijkstra | move 4 is the position 5", path->valid[4].m == 5);
      TESTCASE("- dijkstra | move 5 is the position 5", path->valid[5].m == 9);
      TESTCASE("- dijkstra | move 6 is the position 14", path->valid[6].m == 13);
   }

   free_moves_valids(path);
   path = dijkstra(p, p->ennemy_pos, p->pos, other_player(p->id), p->owned_nodes, p->numb_win);
   if (v)
   {
      TESTCASE("- dijkstra | length path for white player = 6", path->number == 7);
      TESTCASE("- dijkstra | move 0 is the position of the white player", path->valid[0].m == pos_white);
      TESTCASE("- dijkstra | move 1 is the position 14 or 11", path->valid[1].m == 14 || path->valid[1].m == 11);
      TESTCASE("- dijkstra | move 2 is the position 13 or 10", path->valid[2].m == 13 || path->valid[2].m == 10);
      TESTCASE("- dijkstra | move 3 is the position 9", path->valid[3].m == 9);
      TESTCASE("- dijkstra | move 4 is the position 5", path->valid[4].m == 5);
      TESTCASE("- dijkstra | move 5 is the position 6", path->valid[5].m == 6);
      TESTCASE("- dijkstra | move 6 is the position 2", path->valid[6].m == 2);
   }

   printf("\033[2mPlayer black - 8 / Player white - 12 [Double jump] \033[0m\n");
   p->pos = 8;
   p->ennemy_pos = 12;

   free_moves_valids(path);
   path = dijkstra(p, p->ennemy_pos, p->pos, other_player(p->id), p->owned_nodes, p->numb_win);
   if (v)
   {
      TESTCASE("- dijkstra | length path for white player = 4", path->number == 5);
      TESTCASE("- dijkstra | white player jumps from 12 to 4 (double jump)", path->valid[1].m == 4);
      TESTCASE("- dijkstra | move 2 is the position 5", path->valid[2].m == 5);
      TESTCASE("- dijkstra | move 3 is the position 6", path->valid[3].m == 6);
      TESTCASE("- dijkstra | move 4 is the position 2", path->valid[4].m == 2);
   }

   printf("\033[2mPlayer black - 4 / Player white - 8 [Jump on side] \033[0m\n");
   p->pos = 4;
   p->ennemy_pos = 8;

   free_moves_valids(path);
   path = dijkstra(p, p->ennemy_pos, p->pos, other_player(p->id), p->owned_nodes, p->numb_win);
   if (v)
   {
      TESTCASE("- dijkstra | length path for white player = 3", path->number == 4);
      TESTCASE("- dijkstra | white player jumps from 8 to 5 (double jump)", path->valid[1].m == 5);
      TESTCASE("- dijkstra | move 2 is the position 5", path->valid[2].m == 6);
      TESTCASE("- dijkstra | move 3 is the position 6", path->valid[3].m == 2);
   }

   //graph__display(graph, p->n, p->ennemy_pos, p->pos);

   // === Free tests
   free_moves_valids(path);
	finalization_player(*p);
   free(p);
   // ===

}

/* Tests made for function cutting_path()
*
* Put wall (from base), not disturbing own path
* Put wall on side, not disturbing own path
* Put wall (square), not cutting path
*/
void test__cut_ennemy_path(int v)
{
      //	=== Initialize graph test ===
	size_t size = 5;
	size_t pos_white = 13;
	size_t pos_black = 0;
   struct graph_t* graph = graph__create_square(size);

      // Wall test
   struct move_t wall_test = {.c = BLACK, .t = WALL, .m = -1};

	for(size_t i=0; i<size; i++)
	{
		graph__add_ownership(graph, i, BLACK);
		graph__add_ownership(graph, graph->num_vertices - size + i, WHITE);
	}
   struct player* p = initialize_test_player(graph, size, pos_black, pos_white, BLACK);


   // Wall {3-8, 4-9}
      // Calculs
   struct moves_valids* player_path = dijkstra(p, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   struct moves_valids* ennemy_path = dijkstra(p, p->ennemy_pos, p->pos, other_player(p->id), p->owned_nodes, p->numb_win);
   struct move_t wall = cut_ennemy_path(p, player_path, ennemy_path);
   wall_test.e[0] = (struct edge_t) {3, 8};
   wall_test.e[1] = (struct edge_t) {4, 9};

      // Tests
   printf("\033[2mAdd wall to cut path \033[0m\n");
   if (v) TESTCASE("- cut ennemy path | move is a wall", wall.t == WALL);
   if (v) TESTCASE("- cut ennemy path | wall chosed is {3-8, 4-9}", compare_walls(wall, wall_test));
   put_wall(p, wall);

   // Wall {1-6, 2-7}
      // Calculs
   free_moves_valids(player_path);
   free_moves_valids(ennemy_path);
   player_path = dijkstra(p, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   ennemy_path = dijkstra(p, p->ennemy_pos, p->pos, other_player(p->id), p->owned_nodes, p->numb_win);
   wall = cut_ennemy_path(p, player_path, ennemy_path);
   wall_test.e[0] = (struct edge_t) {1, 6};
   wall_test.e[1] = (struct edge_t) {2, 7};

      //Tests
   printf("\033[2mAdd wall to cut path \033[0m\n");
   if (v) TESTCASE("- cut ennemy path | move is a wall", wall.t == WALL);
   if (v) TESTCASE("- cut ennemy path | wall chosed is {5-6, 10-11}", compare_walls(wall, wall_test));
   put_wall(p, wall);

   // Wall {5-6, 10-11}
      // Calculs
   free_moves_valids(player_path);
   free_moves_valids(ennemy_path);
   player_path = dijkstra(p, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   ennemy_path = dijkstra(p, p->ennemy_pos, p->pos, other_player(p->id), p->owned_nodes, p->numb_win);
   wall = cut_ennemy_path(p, player_path, ennemy_path);
   wall_test.e[0] = (struct edge_t) {5, 6};
   wall_test.e[1] = (struct edge_t) {10, 11};

      // Tests
   printf("\033[2mAdd wall to cut path \033[0m\n");
   if (v) TESTCASE("- cut ennemy path | move is a wall", wall.t == WALL);
   if (v) TESTCASE("- cut ennemy path | wall chosed is {5-6, 10-11}", compare_walls(wall, wall_test));
   put_wall(p, wall);

   // Wall {11-12, 16-17}
      // Calculs
   free_moves_valids(player_path);
   free_moves_valids(ennemy_path);
   player_path = dijkstra(p, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   ennemy_path = dijkstra(p, p->ennemy_pos, p->pos, other_player(p->id), p->owned_nodes, p->numb_win);
   wall = cut_ennemy_path(p, player_path, ennemy_path);
   wall_test.e[0] = (struct edge_t) {11, 12};
   wall_test.e[1] = (struct edge_t) {16, 17};

      // Tests
   printf("\033[2mAdd wall to cut path \033[0m\n");
   if (v) TESTCASE("- cut ennemy path | move is a wall", wall.t == WALL);
   if (v) TESTCASE("- cut ennemy path | wall chosed is {11-12, 16-17}", compare_walls(wall, wall_test));
   put_wall(p, wall);

      // Wall {7-8, 12-13}
      // Calculs
   free_moves_valids(player_path);
   free_moves_valids(ennemy_path);
   player_path = dijkstra(p, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   ennemy_path = dijkstra(p, p->ennemy_pos, p->pos, other_player(p->id), p->owned_nodes, p->numb_win);
   wall = cut_ennemy_path(p, player_path, ennemy_path);
   wall_test.e[0] = (struct edge_t) {11, 6};
   wall_test.e[1] = (struct edge_t) {12, 7};

      // Tests
   printf("\033[2mAdd wall to cut path \033[0m\n");
   if (v) TESTCASE("- cut ennemy path | move is a wall", wall.t == WALL);
   if (v) TESTCASE("- cut ennemy path | wall chosed is {7-8, 12-13}", compare_walls(wall, wall_test));
   put_wall(p, wall);

   free_moves_valids(player_path);
   free_moves_valids(ennemy_path);
   finalization_player(*p);
   free(p);

}

/* Tests made for function double_dijkstra()
*
* Same distance - Checking movement
* Greater distance - Putting correct wall
* Next move is movement (because wall => 2 movements win)
* Greater distance - Putting correct wall
* If no choice: Moving
*
*/
void test__double_dijkstra(int v)
{
      //	=== Initialize graph test ===
	size_t size = 5;
	size_t pos_white = 23;
	size_t pos_black = 2;
   struct graph_t* graph = graph__create_square(size);

      // Wall test
   struct move_t wall_test = {.c = BLACK, .t = WALL, .m = -1};

	for(size_t i=0; i<size; i++)
	{
		graph__add_ownership(graph, i, BLACK);
		graph__add_ownership(graph, graph->num_vertices - size + i, WHITE);
	}
   struct player* p = initialize_test_player(graph, size, pos_black, pos_white, BLACK);

   // === Test 1
   struct moves_valids* player_path = dijkstra(p, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   struct moves_valids* ennemy_path = dijkstra(p, p->ennemy_pos, p->pos, other_player(p->id), p->owned_nodes, p->numb_win);
   struct move_t chosed = double_dijkstra_strategy(p);

   if (v) TESTCASE("- double_dijkstra | same distance => chosed to move", chosed.t == MOVE);
   if (v) TESTCASE("- double_dijkstra | move to positon 7", chosed.m == 7);
   pos_black = chosed.m;
   p->pos = pos_black;
   // ===

   // === Test 2
   printf("\033[2mPlayer white teleported to positon 14 \033[0m\n");
   pos_white = 14;
   p->ennemy_pos = pos_white;

   free_moves_valids(player_path);
   free_moves_valids(ennemy_path);
   player_path = dijkstra(p, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   ennemy_path = dijkstra(p, p->ennemy_pos, p->pos, other_player(p->id), p->owned_nodes, p->numb_win);
   chosed = double_dijkstra_strategy(p);
   wall_test.e[0] = (struct edge_t) {3, 8};
   wall_test.e[1] = (struct edge_t) {4, 9};
   put_wall(p, chosed);


   if (v) TESTCASE("- double_dijkstra | white player closer => chosed to put a wall", chosed.t == WALL);
   if (v) TESTCASE("- double_dijkstra | put wall {3-8, 4-9}", compare_walls(wall_test, chosed));
   // ===

   // === Test 3
   free_moves_valids(player_path);
   free_moves_valids(ennemy_path);
   player_path = dijkstra(p, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   ennemy_path = dijkstra(p, p->ennemy_pos, p->pos, other_player(p->id), p->owned_nodes, p->numb_win);
   chosed = double_dijkstra_strategy(p);

   if (v) TESTCASE("- double_dijkstra | same distance => chosed to move", chosed.t == MOVE);
   if (v) TESTCASE("- double_dijkstra | move to positon 7", chosed.m == 12);
   pos_black = chosed.m;
   p->pos = pos_black;
   // ===

   // === Test 4
   printf("\033[2mPlayer white teleported to positon 7\033[0m\n");
   pos_white = 7;
   p->ennemy_pos = pos_white;

   free_moves_valids(player_path);
   free_moves_valids(ennemy_path);
   player_path = dijkstra(p, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   ennemy_path = dijkstra(p, p->ennemy_pos, p->pos, other_player(p->id), p->owned_nodes, p->numb_win);
   chosed = double_dijkstra_strategy(p);

   wall_test.e[0] = (struct edge_t) {1, 6};
   wall_test.e[1] = (struct edge_t) {2, 7};
   put_wall(p, chosed);

   if (v) TESTCASE("- double_dijkstra | white player closer => chosed to put a wall", chosed.t == WALL);
   if (v) TESTCASE("- double_dijkstra | put wall {1-6, 2-7}", compare_walls(wall_test, chosed));
   // ===

   printf("\033[2mPlayer white teleported to positon 5\033[0m\n");
   pos_white = 5;
   p->ennemy_pos = pos_white;

   free_moves_valids(player_path);
   free_moves_valids(ennemy_path);
   player_path = dijkstra(p, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   ennemy_path = dijkstra(p, p->ennemy_pos, p->pos, other_player(p->id), p->owned_nodes, p->numb_win);
   chosed = double_dijkstra_strategy(p);

   if (v) TESTCASE("- double_dijkstra | desesperate situation => chosed to still move", chosed.t == MOVE);
   if (v) TESTCASE("- double_dijkstra | move to position 17", chosed.m == 17);
   pos_black = chosed.m;
   p->pos = pos_black;

   //graph__display(p->graph, p->n, pos_white, pos_black);


      // === Free tests
   free_moves_valids(player_path);
   free_moves_valids(ennemy_path);
	finalization_player(*p);
   free(p);
   // ===

}

/* Tests made for function fill_wall_array()
*
* Testing if a wall legit is in there
* Checking number
* Adding a wall - Decrease of the number (by 2)
*/
void test__fill_wall_array(int v)
{
      //	=== Initialize graph test ===
	size_t size = 5;
	size_t pos_white = 23;
	size_t pos_black = 2;
   struct graph_t* graph = graph__create_square(size);

      // Wall test
   struct move_t wall_test = {.c = BLACK, .t = WALL, .m = -1};

	for(size_t i=0; i<size; i++)
	{
		graph__add_ownership(graph, i, BLACK);
		graph__add_ownership(graph, graph->num_vertices - size + i, WHITE);
	}
   struct player* p = initialize_test_player(graph, size, pos_black, pos_white, BLACK);

   struct moves_valids* ennemy_path = dijkstra(p, p->ennemy_pos, p->pos, other_player(BLACK), p->owned_nodes, p->numb_win);
   struct moves_valids* walls = valid_walls(p);
   struct moves_valids* filled_array = init_moves_valids(ennemy_path->number * 2);

   // === Test 1
   filled_array = fill_wall_array(p, ennemy_path, walls, filled_array);
   wall_test.e[0] = (struct edge_t) {3, 8};
   wall_test.e[1] = (struct edge_t) {2, 7};

   if (v) TESTCASE("- filled_array | wall {3-8, 2-7} in filled_array", wall_in_list(filled_array->number, filled_array->valid, wall_test));
   if (v) TESTCASE("- filled_array | number of walls in array = 8", filled_array->number == 8);

   free_moves_valids(filled_array);
   free_moves_valids(walls);
   // ===


   // === Test 2
   filled_array = init_moves_valids(ennemy_path->number * 2);

   put_wall(p, wall_test);
   walls = valid_walls(p);
   filled_array = fill_wall_array(p, ennemy_path, walls, filled_array);
   printf("\033[2mAdd wall\033[0m\n");
   if (v) TESTCASE("- filled_array | number of walls in array = 6", filled_array->number == 6);
   // ===


   free_moves_valids(walls);
   free_moves_valids(filled_array);
   free_moves_valids(ennemy_path);
   finalization_player(*p);
   free(p);
}

/* Tests made for super_study_gap()
*
*     General test 1
* Putting wall in front of path + at the end
* Moving because no interesting gap
* Putting wall in front of path + at the end + behind player
* Putting wall cutting the shortest path because of double jump
* Moving because no interesting gap (too much paths with same length)
*
*     General test 2
* Specific problem : no move for Dijkstra => still send legal move
*
*     General test 3
* Specific dumb wall
*
*/
void test__super_study_gap(int v)
{
    //	=== Initialize graph test ===
	size_t size = 5;
	size_t pos_white = 23;
	size_t pos_black = 2;
   struct graph_t* graph = graph__create_square(size);

      // Wall test
   struct move_t wall_test = {.c = BLACK, .t = WALL, .m = -1};

	for(size_t i=0; i<size; i++)
	{
		graph__add_ownership(graph, i, BLACK);
		graph__add_ownership(graph, graph->num_vertices - size + i, WHITE);
	}

   struct player* p = initialize_test_player(graph, size, pos_black, pos_white, BLACK);

   // ============================ General Test 1 ============================
   // === Test 1
   struct moves_valids* ennemy_path = dijkstra(p, p->ennemy_pos, p->pos, other_player(BLACK), p->owned_nodes, p->numb_win);
   struct moves_valids* walls = valid_walls(p);
   struct moves_valids* filled_array = init_moves_valids(ennemy_path->number * 2);
   filled_array = fill_wall_array(p, ennemy_path, walls, filled_array);
   struct move_t wall = super_study_gap(p, filled_array);

      // == Checking special test
   struct moves_valids* player_path = dijkstra(p, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   int old_length_path = player_path->number;
   free_moves_valids(player_path);
   put_wall(p, wall);
   player_path = dijkstra(p, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   int new_length_path = player_path->number;
   free_moves_valids(player_path);
      // ==

   free_moves_valids(ennemy_path);
   free_moves_valids(walls);
   free_moves_valids(filled_array);

   if (v)
   {
      TESTCASE("- super_study_gap | best gap return a wall", wall.t == WALL);
      TESTCASE("- super_study_gap | chosed a wall not cutting our path", old_length_path == new_length_path);
   }
   // ===

   // === Test 2
   ennemy_path = dijkstra(p, p->ennemy_pos, p->pos, other_player(p->id), p->owned_nodes, p->numb_win);
   walls = valid_walls(p);
   filled_array = init_moves_valids(ennemy_path->number * 2);
   filled_array = fill_wall_array(p, ennemy_path, walls, filled_array);
   wall = super_study_gap(p, filled_array);

   free_moves_valids(ennemy_path);
   free_moves_valids(walls);
   free_moves_valids(filled_array);


   if (v) TESTCASE("- super_study_gap | chosed a move because no good gap", wall.t == MOVE);
   // ===

   printf("\033[2mMoving player to the chosen move\033[0m\n");
   pos_black = wall.m;
   p->pos = wall.m;


   // === Test 3
   ennemy_path = dijkstra(p, p->ennemy_pos, p->pos, other_player(p->id), p->owned_nodes, p->numb_win);
   walls = valid_walls(p);
   filled_array = init_moves_valids(ennemy_path->number * 2);
   filled_array = fill_wall_array(p, ennemy_path, walls, filled_array);
   wall = super_study_gap(p, filled_array);

   free_moves_valids(ennemy_path);
   free_moves_valids(walls);
   free_moves_valids(filled_array);

      // == Checking special test
   player_path = dijkstra(p, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   old_length_path = player_path->number;
   free_moves_valids(player_path);
   put_wall(p, wall);
   player_path = dijkstra(p, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   new_length_path = player_path->number;
   free_moves_valids(player_path);
      // ==

   wall_test.e[0] = (struct edge_t) {1, 6};
   wall_test.e[1] = (struct edge_t) {2, 7};
   if (v)
   {
      TESTCASE("- super_study_gap | best gap return a wall", wall.t == WALL);
      TESTCASE("- super_study_gap | chosed a wall not cutting our path", old_length_path == new_length_path);
      TESTCASE("- super_study_gap | chosed wall {1-6, 2-7}", compare_walls(wall, wall_test));
   }
   // ===


   // === Test 4
   ennemy_path = dijkstra(p, p->ennemy_pos, p->pos, other_player(p->id), p->owned_nodes, p->numb_win);
   walls = valid_walls(p);
   filled_array = init_moves_valids(ennemy_path->number * 2);
   filled_array = fill_wall_array(p, ennemy_path, walls, filled_array);
   wall = super_study_gap(p, filled_array);

   free_moves_valids(ennemy_path);
   free_moves_valids(walls);
   free_moves_valids(filled_array);

      // == Checking special test
   player_path = dijkstra(p, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   old_length_path = player_path->number;
   free_moves_valids(player_path);
   put_wall(p, wall);
   player_path = dijkstra(p, p->pos, p->ennemy_pos, p->id, p->winning_nodes, p->numb_win);
   new_length_path = player_path->number;
   free_moves_valids(player_path);
      // ==

   wall_test.e[0] = (struct edge_t) {5, 6};
   wall_test.e[1] = (struct edge_t) {10, 11};
   struct move_t wall_test2 = {.c = BLACK, .t = WALL, .m = -1, .e = {(struct edge_t) {0, 1}, (struct edge_t) {5, 6}}}; 
   if (v)
   {
      TESTCASE("- super_study_gap | best gap return a wall", wall.t == WALL);
      TESTCASE("- super_study_gap | chosed a wall not cutting our path", old_length_path == new_length_path);
      TESTCASE("- super_study_gap | chosed wall {5-6, 10-11} or {0-1, 5-6}", compare_walls(wall, wall_test) || compare_walls(wall, wall_test2));
   }
   // ===


   // === Test 5
   ennemy_path = dijkstra(p, p->ennemy_pos, p->pos, other_player(p->id), p->owned_nodes, p->numb_win);
   walls = valid_walls(p);
   filled_array = init_moves_valids(ennemy_path->number * 2);
   filled_array = fill_wall_array(p, ennemy_path, walls, filled_array);
   wall = super_study_gap(p, filled_array);

   free_moves_valids(ennemy_path);
   free_moves_valids(walls);
   free_moves_valids(filled_array);

   if (v) TESTCASE("- super_study_gap | chosed a move because no good gap", wall.t == MOVE);
   if (v) TESTCASE("- super_study_gap | move chosed is next path of dijkstra", wall.m == (size_t) 12);

   finalization_player(*p);
   free(p);
   // ===
   // ========================================================================


   // ============================ General Test 2 ============================

       //	=== Initialize graph test ===
	size = 5;
	pos_white = 16;
	pos_black = 17;
   struct graph_t* graph_2 = graph__create_square(size);

	for(size_t i=0; i<size; i++)
	{
		graph__add_ownership(graph_2, i, BLACK);
		graph__add_ownership(graph_2, graph_2->num_vertices - size + i, WHITE);
	}

   struct player* p2 = initialize_test_player(graph_2, size, pos_black, pos_white, WHITE);
   p2->pos = pos_white;
   p2->ennemy_pos = pos_black; 
   struct move_t special_wall = {.c = p2->id, .t = WALL, .m = -1}; 

   // === Put special walls for a particular situation 
   special_wall.e[0] = (struct edge_t) {15, 20};
   special_wall.e[1] = (struct edge_t) {16, 21}; 
   put_wall(p2, special_wall); 
   special_wall.e[0] = (struct edge_t) {17, 22};
   special_wall.e[1] = (struct edge_t) {18, 23}; 
   put_wall(p2, special_wall);
   special_wall.e[0] = (struct edge_t) {10, 15};
   special_wall.e[1] = (struct edge_t) {11, 16}; 
   put_wall(p2, special_wall); 
   special_wall.e[0] = (struct edge_t) {13, 18};
   special_wall.e[1] = (struct edge_t) {14, 19}; 
   put_wall(p2, special_wall); 
   // ===
   
   ennemy_path = dijkstra(p2, p2->ennemy_pos, p2->pos, other_player(p2->id), p2->owned_nodes, p2->numb_win);
   walls = valid_walls(p2);
   filled_array = init_moves_valids(ennemy_path->number * 2);
   filled_array = fill_wall_array(p2, ennemy_path, walls, filled_array);
   wall = super_study_gap(p2, filled_array);


   if (v)
   {
      //graph__display(p2->graph, p2->n, pos_white, pos_black);
      TESTCASE("- super_study_gap | no interesting walls", filled_array->number == 0);
      TESTCASE("- super_study_gap | chose a move", wall.t == MOVE);
      TESTCASE("- super_study_gap | chosed move is legal", wall.m == (size_t) 15 || wall.m == (size_t) 18);
   }

   free_moves_valids(ennemy_path);
   free_moves_valids(walls);
   free_moves_valids(filled_array);

   finalization_player(*p2);
   free(p2);
   // ========================================================================


   // ============================ General Test 3 ============================

       //	=== Initialize graph test ===
      /*
	size = 10;
	pos_white = 85;
	pos_black = 25;
   struct graph_t* graph_3 = graph__create_square(size);

	for(size_t i=0; i<size; i++)
	{
		graph__add_ownership(graph_3, i, BLACK);
		graph__add_ownership(graph_3, graph_3->num_vertices - size + i, WHITE);
	}

   struct player* p3 = initialize_test_player(graph_3, size, pos_black, pos_white, WHITE);
   p3->pos = pos_white;
   p3->ennemy_pos = pos_black; 
   struct move_t special_wall = {.c = p3->id, .t = WALL, .m = -1}; 

   ennemy_path = dijkstra(p2->graph, p2->n, p2->ennemy_pos, p2->pos, other_player(p2->id), p2->owned_nodes, p2->numb_win);
   walls = valid_walls(p2);
   filled_array = init_moves_valids(ennemy_path->number * 2);
   filled_array = fill_wall_array(p2, ennemy_path, walls, filled_array);
   wall = super_study_gap(p2, filled_array);

   if (v)
   {
      graph__display(p2->graph, p2->n, pos_white, pos_black);
      TESTCASE("- super_study_gap | no interesting walls", filled_array->number == 0);
      TESTCASE("- super_study_gap | chose a move", wall.t == MOVE);
      TESTCASE("- super_study_gap | chosed move is legal", wall.m == (size_t) 15 || wall.m == (size_t) 18);
   }

   free_moves_valids(ennemy_path);
   free_moves_valids(walls);
   free_moves_valids(filled_array);

   finalization_player(*p2);
   free(p2);
   // ========================================================================
      */
}