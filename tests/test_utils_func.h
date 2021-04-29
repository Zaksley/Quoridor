#ifndef _TEST_UTILS_FUNC_H_
#define _TEST_UTILS_FUNC_H_

/*
 * Only 2 positions found in a corner
 * 4 positions found in the middle of the board
 * Found positions are neighboors
 * Position with enemy on it is not found
 * Positions accessible with leaps are found
 */
void test__valid_positions();

/*
 * Wall over an edge is not found
 * Wall exactly on another wall is not found
 * Wall overriding another wall is not found
 * Wall making a cross with another wall is not found
 * Wall blocking a player is not found
 * Valid walls are found
 */
void test__valid_walls();

/*
 * Valid wall is added
 * Wall with missing first edge is not added
 * Wall with missing second edge is not added 
 * Can't add the same wall
 */
void test__put_wall();

void test__destroy_wall(); 

/*
 * 
 */
void test__exist_path_player();

/*
 * 
 */
void test__check_path();

/*
 * 
 */
void test__rushing_path(); 

#endif //_TEST_UTILS_FUNC_H_