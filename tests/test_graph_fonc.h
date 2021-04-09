#ifndef _TEST_GRAPH_FONC_H_
#define _TEST_GRAPH_FONC_H_

#include "../src/graph_modif.h"

// Test : empty graph have no existing edge
int test__empty_graph__edge_exists();

// Test : added edge exists
int test__added_edge__edge_exists();

// Test : added then removed edge don't exists
int test__removed_edge__edge_exists();

// Test : non-existing neighboors are not found
int test__empty_graph__get_neighboor();

// Test : added neighboor is found
int test__added_vertex__get_neighboor();

// Test : removed neighboor is not found
int test__removed_vertex__get_neighboor();

// Test : copied graph has the same edges
int test__same_edges__copy();

// Test : copied graph has the same neighboors
int test__same_neighboors__copy();

// Test : adding egdes on copy don't modify the original
int test__modify_add__copy();

// Test : removing edges on copy don't modify the original
int test__modify_remove__copy();

// Test : adding twice the same edge return error
int test__twice__add_edge();

// Test : added edge can be removed
int test__remove__add_edge();

// Test : removing twice the same edge return error
int test__twice__remove_edge();

// Test : removed edge can be added again
int test__add__remove_edge();

#endif //_TEST_GRAPH_FONC_H_
