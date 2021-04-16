#include "test_graph_struct.h"
#include "test_graph_fonc.h"

#define TESTCASE(msg, test) printf("%-60s : %s\n", msg, test ? "\033[1;92mPassed\033[0m" : "\033[1;91mFailed\033[0m")

int main()
{
   printf("\033[1m----- Graph_modif | Structural tests -----\033[0m\n");
   printf("\033[1mgraph__initialize :\033[0m\n");
   test__graph_initialize();
   printf("\033[1mgraph__create_square :\033[0m\n");
   test__graph_create_square();
   printf("\033[1mgraph__create_torus :\033[0m\n");
   test__graph_create_torus();
   printf("\033[1mgraph__create_chopped :\033[0m\n");
   test__graph_create_chopped();
   printf("\033[1mgraph__create_snake :\033[0m\n");
   test__graph_create_snake();
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
   printf("\033[1mgraph__display :\033[0m\n");
   test__graph_display();
   printf("\n");


   printf("\033[1m----- Graph_modif | Functionnal tests -----\033[0m\n");
   printf("\033[1mgraph__edge_exists :\033[0m\n");
   TESTCASE("- edge_exists | empty graph have no existing edge" ,test__empty_graph__edge_exists());
   TESTCASE("- edge_exists | added edge exists", test__added_edge__edge_exists());
   TESTCASE("- edge_exists | added then removed edge don't exists", test__removed_edge__edge_exists());
   printf("\033[1mgraph__get_neighboor :\033[0m\n");
   TESTCASE("- get_neighboor | non-existing neighboors are not found", test__empty_graph__get_neighboor());
   TESTCASE("- get_neighboor | added neighboor is found", test__added_vertex__get_neighboor());
   TESTCASE("- get_neighboor | removed neighboor is not found", test__removed_vertex__get_neighboor());
   printf("\033[1mgraph__copy :\033[0m\n");
   TESTCASE("- copy | copied graph has the same edges", test__same_edges__copy());
   TESTCASE("- copy | copied graph has the same neighboors", test__same_neighboors__copy());
   TESTCASE("- copy | adding egdes on copy don't modify the original", test__modify_add__copy());
   TESTCASE("- copy | removing edges on copy don't modify the original", test__modify_remove__copy());
   printf("\033[1mgraph__add_edge :\033[0m\n");
   TESTCASE("- add_edge | adding twice the same edge return error", test__twice__add_edge());
   TESTCASE("- add_edge | added edge can be removed", test__remove__add_edge());
   printf("\033[1mgraph__remove_edge :\033[0m\n");
   TESTCASE("- remove_edge | removing twice the same edge return error", test__twice__remove_edge());
   TESTCASE("- remove_edge | removed edge can be added again", test__add__remove_edge());
   printf("\n");
}