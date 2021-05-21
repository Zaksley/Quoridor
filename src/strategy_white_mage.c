#include <stdlib.h>
#include <dlfcn.h>
#include "strategy_white_mage.h"


enum direction find_closest_side(struct graph_t* graph, size_t n, size_t pos)
{
   size_t left_distance = 0;
   while (graph__get_neighboor(graph, n, (pos - left_distance), WEST) != -1)
      left_distance++;

   size_t right_distance = 0;
   while (graph__get_neighboor(graph, n, (pos + right_distance), EAST) != -1)
      right_distance++;

   return (left_distance > right_distance) ? EAST : WEST;
}


struct move_t find_black_wall(struct graph_t* graph, size_t n, enum direction side, enum color_t color, size_t pos)
{
   struct move_t res;

   enum direction forward_direction = (color == BLACK) ? SOUTH : NORTH;
   res.e[0] = (struct edge_t){pos,
                              graph__get_neighboor(graph, n, pos, side)};
   res.e[1] = (struct edge_t){graph__get_neighboor(graph, n, pos, forward_direction),
                              graph__get_neighboor(graph, n, graph__get_neighboor(graph, n, pos, forward_direction), side)};
   return res;
}


struct move_t protect_line(struct graph_t* graph, size_t n, size_t pos, enum direction d)
{
   enum direction opposite_d = opposite_dir(d);
   struct move_t wall_res;

   int set_wall_behind(struct graph_t* graph, size_t n, size_t pos, enum direction first_d, enum direction second_d, struct move_t* m)
   {
      int neigh_1 = graph__get_neighboor(graph, n, pos, first_d);
      int neigh_2 = -1;

      if (neigh_1 != -1)
      {
         neigh_2 = graph__get_neighboor(graph, n, neigh_1, second_d);
         if (neigh_2 != -1)
         {
            m->e[0] = (struct edge_t){pos,
                                      graph__get_neighboor(graph, n, pos, second_d)};
            m->e[1] = (struct edge_t){neigh_1,
                                      neigh_2};

            return 1;
         }
      }
      return 0;
   }
   
   size_t actual = pos;
   int next = graph__get_neighboor(graph, n, actual, d);

   while (next != -1)
   {
      actual = (size_t) next;
      next = graph__get_neighboor(graph, n, actual, d);
   }


   int wall_1_valid = set_wall_behind(graph, n, actual, ((d == NORTH || d == SOUTH) ? WEST : NORTH), opposite_d, &wall_res);
   if (wall_1_valid == 1)
      return wall_res;

   int wall_2_valid = set_wall_behind(graph, n, actual, ((d == NORTH || d == SOUTH) ? EAST : SOUTH), opposite_d, &wall_res);
   if (wall_2_valid == 1)
      return wall_res;

   wall_res.e[0] = (struct edge_t){-1, -1};
   wall_res.e[1] = (struct edge_t){-1, -1};

   return wall_res;
}
