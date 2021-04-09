#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "../src/utils.h"

#define TESTCASE(msg, test) printf("%-60s : %s\n", msg, test ? "\033[1;92mPassed\033[0m" : "\033[1;91mFailed\033[0m")

void test__valid_positions()
{
	TESTCASE("todo", 0);
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

int main()
{
	printf("\033[1m----- Utils | Functionnal tests -----\033[0m\n");
	printf("\033[1mvalid_positions :\033[0m\n");
	test__valid_positions();
	printf("\033[1mvalid_walls :\033[0m\n");
	test__valid_positions();
	printf("\033[1mput_wall :\033[0m\n");
	test__valid_positions();
	printf("\033[1mexist_path_player :\033[0m\n");
	test__valid_positions();
	printf("\033[1mcheck_path :\033[0m\n");
	test__valid_positions();
	printf("\033[1mshift_left :\033[0m\n");
	test__valid_positions();
	printf("\n");
	return 0;
}