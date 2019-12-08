#include <stdio.h>
#include "polygon.h"
#include "node.h"
#include <math.h>
#include <stdbool.h> 
#include <stdlib.h>

Node * HEAD = NULL;
Node * TAIL = NULL;

// function_ptr_arr is an array of function pointers 
void(*function_ptr_arr[])(polygon *) = { add_polygon, print_polygon, print_perimeter, print_area, print_diagonals };

void execute_add_new_polygon(polygon * polygon_to_add) {
	function_ptr_arr[0](polygon_to_add); //call to add_polygon function
}

//add new polygon to tail and update tail to this polygon
void add_polygon(polygon*new_polygon) {
	Node * new_node = (Node*)malloc(sizeof(Node));
	new_node->poly = new_polygon;
	new_node->next = NULL;

	if (HEAD== NULL) {
		HEAD=new_node;
		TAIL= new_node;
	}

	else {
		TAIL->next = new_node;
		TAIL = new_node;
	}
}

//print polygon details (name and vertices)
void print_polygon(polygon*polygon) {
	int num_of_vertices=(int)polygon->type;
	switch (polygon->type) {
	case 6:
		printf("hexagon");
		break;
	case 7:
		printf("heptagon");
		break;
	case 8:
		printf("octagon");
		break;
	}

	print_vertices(polygon, num_of_vertices);
}

// calculate and print the perimeter
void print_perimeter(polygon*polygon) {
	uint8_t vertices_amount = (uint8_t)polygon->type;
	double perimeter = 0;
	int i;

	for (i = 0; i <vertices_amount ; i++){
		perimeter += calc_edge_size(polygon->points + (i% vertices_amount)  ,  polygon->points + ((i + 1) % vertices_amount));
	}

	printf("perimeter = %.1f\n", perimeter);
}

//calculate and print diagonals perimeter
void print_diagonals(polygon*polygon) {
	double diagonals_perimeters = 0;
	int vertices_amount = (int)polygon->type;
	int i, j;

	for (i = 0; i < vertices_amount; i++) {
		for (j = 0; j < vertices_amount - 3; j++) {
			diagonals_perimeters += calc_edge_size(polygon->points + i, polygon->points + (i + j + 2) % vertices_amount);
		}
	}

	diagonals_perimeters /= 2;
	printf("diagonals = %.1f\n", diagonals_perimeters);
}

// calculate and print the area
void print_area(polygon*polygon) {
	double area = 0;
	int vetices_number = (int)polygon->type;
	int j = vetices_number - 1;
	int i;

	for (i = 0; i < vetices_number; i++) {
		area += ((polygon->points + j)->x + (polygon->points + i)->x) *  ((polygon->points + j)->y - (polygon->points + i)->y);
		j = i;
	}

	area /= 2;
	printf("area = %.1f\n", fabs(area));
}

double calc_edge_size(Point * vertices1, Point *vertices2) {
	double x_distance = vertices1->x - vertices2->x;
	double y_distance = vertices1->y - vertices2->y;


	return sqrt(x_distance*x_distance + y_distance*y_distance);
}

//get number of vertices and take input (vertices) from user and create new polygon
polygon * get_and_create_polygon_details(int polygon_type) {
	polygon * res = (polygon*)malloc(sizeof(polygon));
	res->points = (Point*)malloc(sizeof(Point)*polygon_type);
	res->type = (int8_t)polygon_type;
	Point point_to_add;
	long long vertices;
	int i;
	int8_t coordinate = 255;
	scanf("%llx", &vertices);

	for (i = 0; i < polygon_type; i++) {

		//we moved all the first 64 bit
		if (i == 4) {
			scanf("%llx", &vertices);
		}

		point_to_add = create_point_by_two_coordinate(coordinate & vertices, coordinate & (vertices >> 8));
		res->points[i] = point_to_add;



		coordinate = 255;
		vertices >>= 16;
	}

	return res;
}

void get_input_and_operations(long long int instruction, bool * keep_going, int8_t * added_polygons) {
	bool new_polygon;
	int polygon_type = 15; //0x1111
	polygon * polygon_to_add = NULL;

	*keep_going = !(1 & instruction);
	new_polygon = 1 & (instruction >> 1);

	if (new_polygon) {
		(*added_polygons)++;
		polygon_type &= (instruction >> 2);
		polygon_to_add = get_and_create_polygon_details(polygon_type);
		execute_add_new_polygon(polygon_to_add);
	}

	handle_operations(instruction >> 6); //Decode which commands and who to execute
}

int8_t * create_array_of_functions_indexes(long long instruction, int8_t * array_size) {
	int8_t look_up_table[] = { 0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4 };
	int8_t functions_bits_mask = 15 & ((instruction << 4) >> 4);
	*array_size = look_up_table[functions_bits_mask];
	int8_t * indexes = (int8_t*)malloc(sizeof(int8_t)*(*array_size));
	int logic_size = 0;
	int8_t i;

	for (i = 0; i < 4; i++) {
		if (((instruction << 4) >> (4 + i)) & 1 ){
			indexes[logic_size] = i+1;
			logic_size++;
		}
	}

	return indexes;
}

//check which operations to execute and send the bits that represent to who execute
void handle_operations(long long instruction) {
	int8_t array_size;
	int8_t * indexes_to_execute = create_array_of_functions_indexes(instruction, &array_size);
	Node * curr = HEAD;
	int8_t i;
	switch (instruction >> 4) {
	
		case 0:
			for (i = 0; i < array_size; i++){
				function_ptr_arr[indexes_to_execute[i]](TAIL->poly); //the new polygon at the end of the list
			}

			break;

		default:
			while (curr != NULL) {
				for (i = 0; i < array_size; i++){
				
					if ((int8_t)(curr->poly->type) == (instruction >> 4) || (instruction>>4)==15) { //if current polygon type equal to type from input or need to execute for all polygon  
						function_ptr_arr[indexes_to_execute[i]](curr->poly);
					}
				}

				curr = curr->next;
			}
	}

	free(indexes_to_execute);
}

Point create_point_by_two_coordinate(int8_t x_coordinate, int8_t y_coordinate) {
	Point res;
	res.x = x_coordinate;
	res.y = y_coordinate;

	return res;
}

void print_vertices(polygon * polygon, int num_of_vertices) {
	int i;
	
	for  (i = 0; i < num_of_vertices; i++){
		printf(" {%d, %d}", polygon->points[i].x, polygon->points[i].y);
	}

	printf("\n");
}

void clean_memory() {
	Node * curr = HEAD;
	Node * temp;

	while (curr != NULL) {
		temp = curr;
		curr = curr->next;
		free(temp->poly->points);
		free(temp->poly);
	}
}