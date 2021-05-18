#include <getopt.h>
#include "test_graph_struct.h"
#include "test_graph_fonc.h"
#include "test_utils_func.h"
#include "test_strategy_func.h"
#include "test_graph_shape.h"

static int v = 0; 

#define TESTCASE(msg, test) printf("%-100s : %s\n", msg, test ? "\033[1;92mPassed\033[0m" : "\033[1;91mFailed\033[0m")

////////////////////////////////////////////////////////////////
// Function for parsing the options of the program
// Currently available options are :
// -v <gverbose> : explicit tests

void parse_opts_tests(int argc, char* argv[])
{
  int opt;
  while ((opt = getopt(argc, argv, "v::")) != -1)
   {
      switch (opt)
	   {
         case 'v':
               v = 1; 
            break;

         default: /* '?' */
            fprintf(stderr, "Usage: %s [-v verbose for tests] \n", argv[0]);
            exit(EXIT_FAILURE);
         break; 
      }
   }
}

////////////////////////////////////////////////////////////////


int main(int argc, char* argv[])
{  
   parse_opts_tests(argc, argv); 

   printf("\033[1m----- Graph_modif | Structural tests -----\033[0m\n");
   printf("\033[1mgraph__initialize :\033[0m\n");
   test__graph_initialize();
   printf("\033[1mgraph__copy :\033[0m\n");
   test__graph_copy();
   printf("\033[1mgraph__get_neighboor :\033[0m\n");
   test__graph_get_neighboor();
   printf("\033[1mgraph__add_edge :\033[0m\n");
   test__graph_add_edge();
   printf("\033[1mgraph__remove_edge :\033[0m\n");
   test__graph_remove_edge();
   printf("\033[1mgraph__edge_exists :\033[0m\n");
   test__graph_edge_exists();
   printf("\033[1mgraph__add_ownership :\033[0m\n");
   test__graph_add_ownership();
   printf("\033[1mgraph__is_owned :\033[0m\n");
   test__graph_is_owned();
   printf("\033[1mgraph__list_ownership :\033[0m\n");
   test__graph_list_ownership();
   printf("\033[1mgraph__count_ownership : \033[0m\n");
   test__graph_count_ownership();
   printf("\033[1mgraph__get_size :\033[0m\n");
   test__graph_get_size();
   printf("\033[1mgraph__display :\033[0m\n");
   test__graph_display();
   printf("\n");


   printf("\033[1m----- Graph_modif | Functionnal tests -----\033[0m\n");
   printf("\033[1mgraph__edge_exists :\033[0m\n");
   if (v) TESTCASE("- edge_exists | empty graph have no existing edge" ,test__empty_graph__edge_exists());
   if (v) TESTCASE("- edge_exists | added edge exists", test__added_edge__edge_exists());
   if (v) TESTCASE("- edge_exists | added then removed edge don't exists", test__removed_edge__edge_exists());
   printf("\033[1mgraph__get_neighboor :\033[0m\n");
   if (v) TESTCASE("- get_neighboor | non-existing neighboors are not found", test__empty_graph__get_neighboor());
   if (v) TESTCASE("- get_neighboor | added neighboor is found", test__added_vertex__get_neighboor());
   if (v) TESTCASE("- get_neighboor | removed neighboor is not found", test__removed_vertex__get_neighboor());
   printf("\033[1mgraph__copy :\033[0m\n");
   if (v) TESTCASE("- copy | copied graph has the same edges", test__same_edges__copy());
   if (v) TESTCASE("- copy | copied graph has the same neighboors", test__same_neighboors__copy());
   if (v) TESTCASE("- copy | adding egdes on copy don't modify the original", test__modify_add__copy());
   if (v) TESTCASE("- copy | removing edges on copy don't modify the original", test__modify_remove__copy());
   printf("\033[1mgraph__add_edge :\033[0m\n");
   if (v) TESTCASE("- add_edge | adding twice the same edge return error", test__twice__add_edge());
   if (v) TESTCASE("- add_edge | added edge can be removed", test__remove__add_edge());
   printf("\033[1mgraph__remove_edge :\033[0m\n");
   if (v) TESTCASE("- remove_edge | removing twice the same edge return error", test__twice__remove_edge());
   if (v) TESTCASE("- remove_edge | removed edge can be added again", test__add__remove_edge());
   printf("\n");


   printf("\033[1m----- Graph_modif | Shape tests -----\033[0m\n");
   printf("\033[1mgraph__create_square :\033[0m\n");
   test__graph_create_square(v);
   printf("\033[1mgraph__create_torus :\033[0m\n");
   if (v) TESTCASE("- graph__torus | torus graph has the right shape (size 3)", test__torus_3());
   if (v) TESTCASE("- graph__torus | torus graph has the right shape (size 6)", test__torus_6());
   if (v) TESTCASE("- graph__torus | torus graph has the right shape (size 9)", test__torus_9());
   printf("\033[1mgraph__create_chopped :\033[0m\n");
   if (v) TESTCASE("- graph__chopped | chopped graph has the right shape (size 3)", test__chopped_3());
   if (v) TESTCASE("- graph__chopped | chopped graph has the right shape (size 6)", test__chopped_6());
   if (v) TESTCASE("- graph__chopped | chopped graph has the right shape (size 9)", test__chopped_9());
   printf("\033[1mgraph__create_snake :\033[0m\n");
   if (v) TESTCASE("- graph__snake | snake graph has the right shape (size 5)", test__snake_5());
   if (v) TESTCASE("- graph__snake | snake graph has the right shape (size 10)", test__snake_10());


   printf("\033[1m----- Utils | Functionnal tests -----\033[0m\n");
	printf("\033[1mvalid_positions :\033[0m\n");
	test__valid_positions(v);
	printf("\033[1mvalid_walls :\033[0m\n");
	test__valid_walls(v);
	printf("\033[1mput_wall :\033[0m\n");
	test__put_wall(v);
   printf("\033[1mdestroy_wall :\033[0m\n");
	test__destroy_wall(v);
	printf("\033[1mexist_path_player :\033[0m\n");
	test__exist_path_player(v);


   printf("\033[1m\n----- Strategy | Functionnal tests -----\033[0m\n");
   printf("\033[1mrushing_path :\033[0m\n");
   test__rushing_path(v);
   printf("\033[1mdijkstra :\033[0m\n");
   test__dijkstra(v);  
   printf("\033[1mcut_ennemy_path :\033[0m\n");
   test__cut_ennemy_path(v);   
   printf("\033[1mdouble_dijkstra_strategy :\033[0m\n");
   test__double_dijkstra(v);  
}
