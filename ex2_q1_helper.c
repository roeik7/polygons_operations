#include <stdio.h>
#include "node.h"
#include "polygon.h"
#include <stdbool.h> 
#include <stdlib.h>
#include "polygon.c"

int main() {
	int8_t added_polygons = 0;
	long long instruction;
	bool keep_going = true;

	while (1) {
		scanf("%llx", &instruction);
		get_input_and_operations(instruction, &keep_going, &added_polygons);

		if (!keep_going) {
			break;
		}
	}

	clean_memory();

	
	return(added_polygons);

}