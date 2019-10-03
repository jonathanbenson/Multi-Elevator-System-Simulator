
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "priority_queue.h"


int main() {
	
	srand(time(NULL));
	
	struct node n;
	struct priority_queue pc = construct_empty_priority_queue();
	
	for (int i = 0; i < 10; ++i) {
		
		n.c.time_punches[0] = rand() % 100;
		
		push_priority_queue(&pc, n, cmp_node_c_time_punch_0, 1);
		
	}
	
	write_priority_queue_console(&pc, get_node_c_time_punches_0);
	printf("\n\n");
	
	pop_priority_queue(&pc);
	
	write_priority_queue_console(&pc, get_node_c_time_punches_0);
	
	free_priority_queue(&pc);
	
	
	return 0;
}