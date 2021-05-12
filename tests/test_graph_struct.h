#ifndef _TEST_GRAPH_STRUCT_H_
#define _TEST_GRAPH_STRUCT_H_

/* Tests : 
 *   - no vertices in the graph
 *   - graph has no edge
 */
void test__graph_initialize();

/* Tests : 
 *   - graph with size 1 has 1 vertex
 *   - graph with size 1 has no edges
 *   - graph with size 4 has 16 vertices
 *   - graph with size 4 has all of its edges
 */
void test__graph_create_square(int v);

/* Tests : 
 *   - same nb of vertices as the original
 *   - same edges as the original
 */
void test__graph_copy();

/* Tests :
 *   - existing neighboor is found
 *   - non-existing neighboor is not found
 *   - out-of-bounds neighboor is not found
 */
void test__graph_get_neighboor();

/* Tests :
 *   - impossible direction is not added
 *   - non-existing edge is added
 *   - already-existing edge is not added
 *   - first vertex has the correct directionnal link
 *   - second vertex has the opposite link
 *   - all 4 directions are working
 */ 
void test__graph_add_edge();

/* Tests :
 *   - existing edge is removed
 *   - non-existing edge is not removed
 *   - one-sided edge is still removed
 *   - first vertex has lost directionnal link
 *   - second vertex has lost the opposite link
 */ 
void test__graph_remove_edge();

/* Tests : 
 *   - existing edge is found
 *   - non-existing edge is not found
 *   - one-sided edge (first side) is not found
 *   - one-sided edge (other side) is not found
 */
void test__graph_edge_exists();

/* Tests :
 *   - non-owned position is added
 *   - already-owned position is not added
 *   - added position is correct
 */ 
void test__graph_add_ownership();

/* Tests :
 *	 - non-owned position is not found
 * 	 - ennemy owned position is not found
 *   - owned position is found
 */ 
void test__graph_is_owned();

/* Tests :
 *   - empty list is found with no owned positions
 *   - every position found is a winning pos
 *   - no winning positions missed
 */ 
void test__graph_list_ownership();

/* Tests :
 *	 - accessible white pos is found
 *   - non-accessible white pos is not found
 *   - accessible black pos is found
 *   - non-accessible black pos is nof found
 */
void test__graph_count_ownership();

/* Test :
 * 	 - correct size is returned
 */
void test__graph_get_size();

/* Display a graph (visual tests)
 */
void test__graph_display();

#endif //_TEST_GRAPH_STRUCT_H_

