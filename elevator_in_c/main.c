
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "elevator.h"



int main() {
	
	srand(time(NULL));
	
	// generation of elevator

	const int BOTTOM_FLOOR = 0, TOP_FLOOR = 10;
	const int NUM_ELEVATORS = 5;
	const int NUM_CLIENTS = 100;

	struct elevator* elevators = (struct elevator*)malloc(sizeof(struct elevator) * NUM_ELEVATORS);


	int cfre[] = {BOTTOM_FLOOR, TOP_FLOOR};
	int ap = 2, ip = 10;
	
	int i;

	for (i = 0; i < NUM_ELEVATORS; ++i) {

		elevators[i] = construct_random_elevator(cfre, ap, ip);

	}
	

	


	struct priority_queue incoming_clients = construct_empty_priority_queue();
	
	
	
	struct node new_node;
	struct client random_client;
	
	// cfrc = current floor range of a client
	// nfr = next floor range
	// tp0r = time punch range
	int cfrc[2] = {BOTTOM_FLOOR, TOP_FLOOR}, nfr[2] = {BOTTOM_FLOOR, TOP_FLOOR}, tp0r[2] = {0,3600};
	
	// pushing randomly generated clients to incoming_clients
	printf("Priority queue of incoming clients (sorted by first time punch): \n");
	for (i = 0; i < NUM_CLIENTS; ++i) {
		
		random_client = construct_random_client(cfrc, nfr, tp0r);
		
		new_node.c = random_client;
		
		push_priority_queue(&incoming_clients, new_node, cmp_node_c_time_punch_0, 0);
		
	}
	
	write_priority_queue_console(&incoming_clients, get_node_c_time_punches_0);
	printf("\n\n");
	
	
	
	// start of simulation
	
	int time = 0;		
	
	struct node* current;
	struct elevator* e;

	while (!is_empty_priority_queue(&incoming_clients) || !are_empty_elevators(elevators, NUM_ELEVATORS)) {

		current = incoming_clients.head;

		while (current != NULL && current->c.time_punches[0] == time) {

			// choose a random elevator
			e = get_closest_elevators_node_c(elevators, NUM_ELEVATORS, *current);
		
			if (e->state == IDLE) {
				
				if (e->direction == NA) {
					// elevator is waiting for a client
					
					
					if (e->current_floor == current->c.current_floor) {
						// send outbound
						// elevator becomes active
						// ...and in the direction of the client's next floor
						
						e->state = IDLE;
						e->idle_stopwatch = e->idle_period;
						
						push_node_outbound_elevator(e, *current, time);
						pop_priority_queue(&incoming_clients);

						if (current->c.next_floor < e->current_floor)
							e->direction = DOWN;
						else
							e->direction = UP;

					}
					else {
						// send inbound
						// elevator becomes active
						// ...and in the direction of the client's current floor
						
						e->state = ACTIVE;
						e->active_stopwatch = e->active_period;

						push_node_inbound_elevator(e, *current, time);
						pop_priority_queue(&incoming_clients);

						if (current->c.current_floor < e->current_floor)
							e->direction = DOWN;
						else
							e->direction = UP;
					}


				}
				else {
					if (e->current_floor == current->c.current_floor) {
						// send outbound
						
						push_node_outbound_elevator(e, *current, time);
					}
					else {
						// send inbound
						
						push_node_inbound_elevator(e, *current, time);
					}

					pop_priority_queue(&incoming_clients);
				}
			}
			else {

				if (e->current_floor == current->c.current_floor) {
					// send inbound of opposite-ward
					
					current->c.time_punches[1] = time;

					if (e->direction == DOWN)
						push_priority_queue(&e->upward_inbound, *current, cmp_node_c_current_floor, 0);
					else
						push_priority_queue(&e->downward_inbound, *current, cmp_node_c_current_floor, 1);
					
					pop_priority_queue(&incoming_clients);
					
				}
				else {
					// send inbound
					
					push_node_inbound_elevator(e, *current, time);
					pop_priority_queue(&incoming_clients);
				}
			}

			current = current->next;
		}


		for (i = 0; i < NUM_ELEVATORS; ++i) {
			// handle state and direction with stopwatches

			e = &elevators[i];

			current = NULL;

			// iterate through inbound queues
		
			e->idle_stopwatch = 0;	
				
			if (e->direction != NA) {
				
				// inbound queues

				if (e->direction == DOWN)
					current = e->downward_inbound.head;
				else
					current = e->upward_inbound.head;
				
					
				while (current != NULL && current->c.current_floor == e->current_floor) {

					push_node_outbound_elevator(e, *current, time);
					e->idle_stopwatch = e->idle_period;
					current = current->next;
				}

				current = NULL;

				// outbound queues
				
				if (e->direction == DOWN)
					current = e->downward_outbound.head;
				else
					current = e->upward_outbound.head;

				while (current != NULL && current->c.next_floor == e->current_floor) {

					push_node_record_elevator(e, *current, time);
					e->idle_stopwatch = e->idle_period;
					current = current->next;
				}
			}

			
			if (is_elevator_empty(e)) {
				
				e->state = IDLE;
				e->direction = NA;
				
			}


		//

			if (e->direction != NA) {
				if (e->state == IDLE) {
					if (e->idle_stopwatch == 0) {

						if (should_change_direction_elevator(e))
							change_direction_elevator(e);

						e->state = ACTIVE;
						e->active_stopwatch = e->active_period;	

					}
					else {

						e->idle_stopwatch--;

					}
				}
				else {
					if (e->active_stopwatch == 0) {
						
						e->state = IDLE;
						e->idle_stopwatch = e->idle_period;
						
						if (e->direction == DOWN)
							e->current_floor--;
						else
							e->current_floor++;
					}
					else {
						
						e->active_stopwatch--;

					}
				}
			}
		}

		time++;
	}

	struct client c;

	for (i = 0; i < NUM_ELEVATORS; ++i) {

		e = &elevators[i];

		current = e->record.head;

		printf("Elevator #%d's record:\n", i + 1);

		while (current != NULL) {
			
			c = current->c;
			
			printf("%d %d %d %d %d\n", c.current_floor, c.next_floor, c.time_punches[0], c.time_punches[1], c.time_punches[2]);
			current = current->next;
		}


		printf("\n\n");
		
		free_elevator(e);
	}

	free(elevators);
	
	return 0;
}


