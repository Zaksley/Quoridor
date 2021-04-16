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
	TESTCASE("todo", 0);
}

void test__exist_path_player()
{
	TESTCASE("todo", 0);
}

void test__check_path()
{
	TESTCASE("todo", 0);
}

void test__shift_left()
{
	TESTCASE("todo", 0);
}