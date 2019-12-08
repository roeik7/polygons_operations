#ifndef POLYGON_H
#define POLYGON_H
#include "point.h"
#include <stdbool.h> 


enum POLYGON { HEXAGON = 6, HEPTAGON = 7, OCTAGON = 8 };


struct _polygon {
	enum POLYGON type;
	Point * points;
};

typedef struct _polygon polygon;


void execute_add_new_polygon(polygon * polygon_to_add);
void get_input_and_operations(long long int instruction, bool * keep_going, int8_t*added_polygons);
void handle_operations(long long instruction);
int8_t * create_array_of_functions_indexes(long long instruction, int8_t * array_size);
Point create_point_by_two_coordinate(int8_t x_coordinate, int8_t y_coordinate);
void add_polygon(polygon*polygon);
void print_vertices(polygon * polygon, int num_of_vertices);
void print_perimeter(polygon*polygon);
void print_polygon(polygon*polygon);
void print_area(polygon*polygon);
void print_diagonals(polygon*polygon);
polygon * get_and_create_polygon_details(int polygon_type);
double calc_edge_size(Point  * vertices1, Point * vertices2);
void clean_memory();


#endif