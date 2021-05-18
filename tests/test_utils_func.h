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
*/
void test__valid_positions(int v);

/* Tests made for function valid_walls()
* For every wall: Check number of legal walls + checking every wall illegal
* 
* Number of walls available with naked graph
* Put special wall close to border
* Put normal wall in center
* Put intersection wall
* Put normal wall to prepare cutting path
* Put corner wall (-1 wall logic / -3 walls because of path)
* Moving player, getting more walls available
* Moving player back, getting less walls available
* Put special wall (cutting 9 walls)
*
*/
void test__valid_walls(int v);

/* Tests made for function put_wall()
*
* Add a wall
* Checking edges are destroyed
* Add a wall on same position => error 
*/
void test__put_wall(int v);

/* Tests made for function destroy_wall()
*  
*  Destroy wall
*  Check if edges exists now
*  Put a wall on same place is ok
*/
void test__destroy_wall(int v); 

/* Tests made for function exist_path_player()
*
* Initial graph - checking both players have path
* Putting non cutting wall - checking both players have path
* Putting cutting path only for white
* Putting cutting path for black
* Destroying wall
*
*/
void test__exist_path_player(int v);



#endif //_TEST_UTILS_FUNC_H_