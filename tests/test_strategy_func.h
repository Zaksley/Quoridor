#ifndef _TEST_STRATEGY_FUNC_H_
#define _TEST_STRATEGY_FUNC_H_


/* Tests made for function rushing_path()
*
*  Without walls - rushing straight down
*  With wall in front of player - going on sides
*  Jump on player to get faster
*
*/
void test__rushing_path(int v); 

/* Tests made for function dijkstra
*  All tests are used on both players
*
* Normal Dijkstra 
* Dijkstra with wall in front
* Dijkstra with jump on player
*
*/
void test__dijkstra(int v);


/* Tests made for function cutting_path()
*
* Put wall (from base), not disturbing own path
* Put wall on side, not disturbing own path
* Put wall (square), not cutting path
*/
void test__cut_ennemy_path(int v);

/* Tests made for function double_dijkstra()
*
* Same distance - Checking movement
* Greater distance - Putting correct wall
* Next move is movement (because wall => 2 movements win)
* Greater distance - Putting correct wall
* If no choice: Moving
*
*/
void test__double_dijkstra(int v); 

#endif 