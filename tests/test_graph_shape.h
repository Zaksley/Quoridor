#ifndef _TEST_GRAPH_SHAPE_H_
#define _TEST_GRAPH_SHAPE_H_

#include "../src/graph_modif.h"

// Test : torus graph of size 3 has the correct shape
int test__torus_3();

// Test : torus graph of size 6 has the correct shape
int test__torus_6();

// Test : torus graph of size 9 has the correct shape
int test__torus_9();

// Test : chopped graph of size 3 has the correct shape
int test__chopped_3();

// Test : chopped graph of size 6 has the correct shape
int test__chopped_6();

// Test : chopped graph of size 9 has the correct shape
int test__chopped_9(); 

// Test : snake graph of size 5 has the correct shape
int test__snake_5();

// Test : snake graph of size 10 has the correct shape
int test__snake_10();

#endif //_TEST_GRAPH_SHAPE_H_