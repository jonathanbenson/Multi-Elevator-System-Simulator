
// Main simulation

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "elevator.h"



int main() {
	
	srand(time(NULL));
	
	

	const int BOTTOM_FLOOR = 0, TOP_FLOOR = 50;
	const int NUM_ELEVATORS = 8;
	const int NUM_CLIENTS = 100;


	// Generation of elevators
	struct elevator* elevators = (struct elevator*)malloc(sizeof(struct elevator) * NUM_ELEVATORS);


	int cfre[] = {BOTTOM_FLOOR, TOP_FLOOR};
	int ap = 2, ip = 5;
	
	int i;

	for (i = 0; i < NUM_ELEVATORS; ++i) {

		elevators[i] = construct_random_elevator(cfre, ap, ip);

		printf("Elevator #%d: Starting floor = %d, Active Period = %d, Idle Period = %d\n", i + 1,
																							elevators[i].current_floor,
																								elevators[i].active_period,
																									elevators[i].idle_period);


	}

	printf("\n\n");
	
	// Generation of random clients

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
	
	printf("Incoming Clients (sorted by first time punch ascending): \n");

	write_priority_queue_console(&incoming_clients, get_node_c_time_punches_0);
	printf("\n\n");
	
	
	
	// start of simulation
	
	int time = 0;		
	
	struct node* current;
	struct elevator* e;

	while (!is_empty_priority_queue(&incoming_clients) || !are_empty_elevators(elevators, NUM_ELEVATORS)) {
		// while the incoming clients queue is not empty and all the elevators are not empty

		current = incoming_clients.head;

		while (current != NULL && current->c.time_punches[0] == time) {

		
			e = get_closest_elevators_node_c(elevators, NUM_ELEVATORS, *current); // selecting the closest elevator
		
			// e = &elevators[rand() % NUM_ELEVATORS]; // selecting a random elevator

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
					
					
				}
				else {
					// send inbound
					
					push_node_inbound_elevator(e, *current, time);
					
				}

				pop_priority_queue(&incoming_clients);

				
			}

			current = current->next;
		}


		for (i = 0; i < NUM_ELEVATORS; ++i) {

			e = &elevators[i];

			current = NULL;

			// iterate through inbound queues
		
			
				
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

			
		// Handling of state and direction via the elevator's stopwatches

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

	int total_time = 0;
	int avg_process_time = 0;

	for (i = 0; i < NUM_ELEVATORS; ++i) {
		// iterate through all the elevators

		e = &elevators[i];

		current = e->record.head;

		printf("Elevator #%d's record:\n", i + 1);
		printf("---------------------------------------------------\n");
		printf("starting floor | next floor | tp[0] | tp[1] | tp[2]\n");
		printf("---------------------------------------------------\n");

		while (current != NULL) {
			
			c = current->c;

			avg_process_time += (c.time_punches[2] - c.time_punches[0]);

			if (c.time_punches[2] > total_time)
				total_time = c.time_punches[2];
			
			// output all of the clients' starting floor, next floor, and time punches
			printf("%d | %d | %d | %d | %d\n", c.current_floor, c.next_floor, c.time_punches[0], c.time_punches[1], c.time_punches[2]);
			current = current->next;
		}

		printf("\n\n");
		
	}

	printf("\n\n");

	avg_process_time /= NUM_CLIENTS;

	int h, m, s;

	h = total_time / 3600;
	m = total_time % 3600 / 60;
	s = total_time % 3600 % 60;

	printf("Multi-Elevator System Simulation Summary\n");
	printf("%d Elevators, %d Clients\n", NUM_ELEVATORS, NUM_CLIENTS);
	printf("Bottom Floor: %d, Top Floor: %d\n", BOTTOM_FLOOR, TOP_FLOOR);
	printf("Elevator speed: %d seconds to traverse one floor\n", ap);
	printf("Elevator idle: %d seconds to pick up or drop off a client\n", ip);
	printf("Total Time: %d hours %d minutes %d seconds\n", h, m, s);
	printf("Average process time of clients: %d minutes %d seconds", avg_process_time / 60, avg_process_time % 60);
	

	free(elevators);
	
	return 0;
}


