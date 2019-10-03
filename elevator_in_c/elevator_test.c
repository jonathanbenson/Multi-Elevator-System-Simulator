
#include <stdio.h>
#include <time.h>
#include "elevator.h"


int main() {
	
	srand(time(NULL));
	
	// testing construct_random_elevator
	struct elevator e;
	
	e = construct_elevator(50, 2, 7);
	
	printf("Randomly generated elevator: %d, %d, %d\n\n", e.current_floor, e.active_period, e.idle_period);
	
	
	// testing construct_random_client
	struct client c;
	
	int cfrc[2] = {0,5}, nfr[2] = {0,5}, tp0r[2] = {0,3600};
	
	c = construct_random_client(cfrc, nfr, tp0r);
	printf("Randomly generated client: %d, %d, %d\n\n", c.current_floor, c.next_floor, c.time_punches[0]);
	
	
	
	
	
	// testing flow of the program
	int time = 0;
	
	struct priority_queue incoming_clients = construct_empty_priority_queue();
	
	const int NUM_CLIENTS = 100;
	
	int i;
	
	
	struct node new_node;
	struct client random_client;
	
	int cfrc2[2] = {0,100}, nfr2[2] = {0,100}, tp0r2[2] = {0,3600};
	
	// pushing randomly generated clients to incoming_clients
	printf("Priority queue of incoming clients (sorted by first time punch): \n");
	for (i = 0; i < NUM_CLIENTS; ++i) {
		
		new_node.c = construct_random_client(cfrc2, nfr2, tp0r2);
		
		push_priority_queue(&incoming_clients, new_node, cmp_node_c_time_punch_0, 0);
		
	}
	
	write_priority_queue_console(&incoming_clients, get_node_c_time_punches_0);
	printf("\n\n");
	
	// using the randomly generated elevator e above...
	// push all nodes from incoming clients to the inbound queues of the elevator
	
	
	struct node* current = incoming_clients.head;
	
	while (current != NULL) {
		
		if (current->c.current_floor == e.current_floor) {
			// recursive case
			
			// push to outbound queues depending on the next floor of the client
			// pop from incoming clients
			// no need to go further because client's next floor != client's current floor
			
			if (current->c.next_floor < e.current_floor)
				push_priority_queue(&e.downward_outbound, *current, cmp_node_c_current_floor, 1);
			else
				push_priority_queue(&e.upward_outbound, *current, cmp_node_c_current_floor, 0);
			
			
		}
		else if (current->c.current_floor < e.current_floor) {
			
			// push to downward inbound queue
			// pop from incoming clients
			
			push_priority_queue(&e.downward_inbound, *current, cmp_node_c_current_floor, 1);

			
		}
		else {
			
			// push to upward inbound queue
			// pop from incoming clients
			push_priority_queue(&e.upward_inbound, *current, cmp_node_c_current_floor, 0);
			
			
		}
		
		current = current->next;
		pop_priority_queue(&incoming_clients);
		
	}
	

	printf("Upward Inbound Queue (sorted by current floor): \n");
	write_priority_queue_console(&e.upward_inbound, get_node_c_current_floor);
	
	printf("\n\nDownward Inbound Queue (sorted by current floor): \n");
	write_priority_queue_console(&e.downward_inbound, get_node_c_current_floor);
	
	printf("\n\n");
	
	
	// pushing clients to outbound queues from inbound queues
	// downward inbound queue first
	
	current = e.downward_inbound.head;
	
	while (current != NULL) {
		
		if (current->c.next_floor < e.current_floor) {
			
			// push to downward outbound queue
			
			push_priority_queue(&e.downward_outbound, *current, cmp_node_c_next_floor, 1);

			
		}
		else {
			
			// push to upward outbound queue
			
			push_priority_queue(&e.upward_inbound, *current, cmp_node_c_next_floor, 0);
			
			
		}
		
		current = current->next;
		pop_priority_queue(&e.downward_inbound);
		
	}
	
	// upward inbound
	
	current = e.upward_inbound.head;
	
	while (current != NULL) {
		
		if (current->c.next_floor < e.current_floor) {
			
			// push to downward outbound queue
			
			push_priority_queue(&e.downward_outbound, *current, cmp_node_c_next_floor, 1);

			
		}
		else {
			
			// push to upward outbound queue
			
			push_priority_queue(&e.upward_outbound, *current, cmp_node_c_next_floor, 0);
			
			
		}
		
		current = current->next;
		pop_priority_queue(&e.upward_inbound);
		
	}
	
	
	printf("Upward Outbound Queue (sorted by next floor): \n");
	write_priority_queue_console(&e.upward_outbound, get_node_c_next_floor);
	
	printf("\n\nDownward Outbound Queue (sorted by next floor): \n");
	write_priority_queue_console(&e.downward_outbound, get_node_c_next_floor);
	
	printf("\n\n");
	
	
	
	// pushing all clients from outbound queues to record
	// popping them all from outbound queues
	
	// downward outbound first
	
	current = e.downward_outbound.head;
	
	while (current != NULL) {
		
		push_priority_queue(&e.record, *current, cmp_node_c_time_punch_0, 1);
		
		current = current->next;
		pop_priority_queue(&e.downward_outbound);
		
	}
	
	current = e.upward_outbound.head;
	
	while (current != NULL) {
		
		push_priority_queue(&e.record, *current, cmp_node_c_time_punch_0, 1);
		
		current = current->next;
		pop_priority_queue(&e.upward_outbound);
		
	}
	
	
	printf("Record (sorted descending by time punches [0]): ");
	write_priority_queue_console(&e.record, get_node_c_time_punches_0);
	
	printf("\n\n");
	
	free_priority_queue(&incoming_clients);
	
	
	
	
	
	return 0;
}