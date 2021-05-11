#ifndef _TEST_UTILS_FUNC_H_
#define _TEST_UTILS_FUNC_H_

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
void test__valid_positions(int v);

/*
 * Wall over an edge is not found
 * Wall exactly on another wall is not found
 * Wall overriding another wall is not found
 * Wall making a cross with another wall is not found
 * Wall blocking a player is not found
 * Valid walls are found
 */
void test__valid_walls(int v);

/*
 * Valid wall is added
 * Wall with missing first edge is not added
 * Wall with missing second edge is not added 
 * Can't add the same wall
 */
void test__put_wall(int v);

void test__destroy_wall(int v); 

/*
 * 
 */
void test__exist_path_player(int v);

/*
 * 
 */
void test__check_path(int v);



#endif //_TEST_UTILS_FUNC_H_