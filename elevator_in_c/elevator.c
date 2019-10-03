
#include <stdio.h>
#include <stdlib.h>
#include "elevator.h"


struct elevator construct_elevator(int cf, int ap, int ip) {
	
	struct elevator e;
		
	e.direction = NA;
	e.state = IDLE;
	
	e.active_stopwatch = 0;
	e.idle_stopwatch = 0;
	
	e.current_floor = cf;
	e.active_period = ap;
	e.idle_period = ip;
	
	e.downward_inbound = construct_empty_priority_queue();
	e.upward_inbound = construct_empty_priority_queue();
	e.downward_outbound = construct_empty_priority_queue();
	e.upward_outbound = construct_empty_priority_queue();
	e.record = construct_empty_priority_queue();
	
	return e;
	
	
}

struct elevator construct_random_elevator(int cfr[2], int ap, int ip) {
	
	struct elevator e;
	
	e.direction = NA;
	e.state = IDLE;
	
	e.active_stopwatch = 0;
	e.idle_stopwatch = 0;
	
	e.current_floor = cfr[0] + (rand() % (cfr[1] - cfr[0] + 1));
	e.active_period = ap;
	e.idle_period = ip;
	
	e.downward_inbound = construct_empty_priority_queue();
	e.upward_inbound = construct_empty_priority_queue();
	e.downward_outbound = construct_empty_priority_queue();
	e.upward_outbound = construct_empty_priority_queue();
	e.record = construct_empty_priority_queue();
	
	return e;
	
}

int is_elevator_empty(struct elevator* e) {
	
	if (	
			e->downward_inbound.head == NULL &&
			e->upward_inbound.head == NULL &&
			e->downward_outbound.head == NULL &&
			e->upward_outbound.head == NULL
			
		)
			
		return 1;
		
	else
		
		return 0;
	
	
}

int are_empty_elevators(struct elevator* es, const int SIZE) {

	for (int i = 0; i < SIZE; ++i)
		if (!is_elevator_empty(&es[i]))
			return 0;

	return 1;

}

struct elevator* get_closest_elevators_node_c(struct elevator* es, const int SIZE, struct node n) {

	struct elevator* closest = &es[0];

	for (int i = 0; i < SIZE; ++i)
		if (get_distance_elevator_node_c(&es[i], n) < get_distance_elevator_node_c(closest, n))
			closest = &es[i];

	return closest;

}

int get_distance_elevator_node_c(struct elevator* e, struct node n) {



	if (is_elevator_empty(e)) {



		return abs(e->current_floor - n.c.current_floor);

	}
	else if (e->state == IDLE && e->current_floor == n.c.current_floor) {

		return 0;

	}
	else {
		// if the client's current floor is the same as the elevator's, the elevator is active
		// if the elevator is idle, then the elevator and client are NOT on the same floor

		if (e->direction == DOWN) {
			// either one or both of downward queues has a client waiting to be served


			if (is_empty_priority_queue(&e->downward_inbound) && is_empty_priority_queue(&e->downward_outbound)) {
				// special case where the elevator is IDLE with and is waiting to turn around
				
				return abs(e->current_floor - n.c.current_floor);

			}

			if (n.c.current_floor >= e->current_floor) {
				// elevator is passsed the client already going down

				if (!is_empty_priority_queue(&e->downward_inbound)) {
					// there is a client waiting to be dropped off in the downward inbound queue on their current floor

					return abs(e->current_floor - e->downward_inbound.head->c.current_floor)
							+ abs(e->current_floor - n.c.current_floor);



				}
				else {
					// there is a client waiting to be picked up in the downward outbound queue on their next floor

					return abs(e->current_floor - e->downward_outbound.head->c.next_floor)
							+ abs(e->current_floor - n.c.current_floor);

				}

			}
			else {

				return abs(e->current_floor - n.c.current_floor);

			}
			
		}
		else {

			if (is_empty_priority_queue(&e->upward_inbound) && is_empty_priority_queue(&e->upward_outbound)) {
				// special case where the elevator is IDLE with and is waiting to turn around
				
				return abs(e->current_floor - n.c.current_floor);

			}

			if (n.c.current_floor <= e->current_floor) {
				// elevator is passsed the client already going down

				if (!is_empty_priority_queue(&e->upward_inbound)) {
					// there is a client waiting to be dropped off in the upward inbound queue on their current floor

					return abs(e->current_floor - e->upward_inbound.head->c.current_floor)
							+ abs(e->current_floor - n.c.current_floor);

				}
				else {
					// there is a client waiting to be picked up in the upward outbound queue on their next floor

					return abs(e->current_floor - e->upward_outbound.head->c.next_floor)
							+ abs(e->current_floor - n.c.current_floor);

				}

			}
			else {

				return abs(e->current_floor - n.c.current_floor);

			}
		}
	}
}


void push_node_inbound_elevator(struct elevator* e, struct node n, int t) {
	// assumes the client's next floor is not the same as current floor
	
	if (n.c.current_floor < e->current_floor)
		push_priority_queue(&e->downward_inbound, n, cmp_node_c_current_floor, 1);
	else
		push_priority_queue(&e->upward_inbound, n, cmp_node_c_current_floor, 0);
}

void push_node_outbound_elevator(struct elevator* e, struct node n, int t) {
	
	n.c.time_punches[1] = t;

	if (n.c.next_floor < e->current_floor)
		push_priority_queue(&e->downward_outbound, n, cmp_node_c_next_floor, 1);
	else
		push_priority_queue(&e->upward_outbound, n, cmp_node_c_next_floor, 0);

	if (e->direction == DOWN)
		pop_priority_queue(&e->downward_inbound);
	else
		pop_priority_queue(&e->upward_inbound);

}

void push_node_record_elevator(struct elevator* e, struct node n, int t) {

	n.c.time_punches[2] = t;
	
	push_priority_queue(&e->record, n, cmp_node_c_time_punch_2, 1);

	if (e->direction == DOWN)
		pop_priority_queue(&e->downward_outbound);
	else
		pop_priority_queue(&e->upward_outbound);

}

int should_change_direction_elevator(struct elevator* e) {

	if (e->direction == DOWN) {
		if (is_empty_priority_queue(&e->downward_inbound) && is_empty_priority_queue(&e->downward_outbound))
			return 1;
	}
	else {
		if (is_empty_priority_queue(&e->upward_inbound) && is_empty_priority_queue(&e->upward_outbound))
			return 1;
	}

	return 0;


}
void change_direction_elevator(struct elevator* e) {

	if (e->direction == DOWN)
		e->direction = UP;
	else
		e->direction = DOWN;
}




void free_elevator(struct elevator* e) {
	
	free_priority_queue(&e->downward_inbound);
	free_priority_queue(&e->upward_inbound);
	free_priority_queue(&e->downward_outbound);
	free_priority_queue(&e->upward_outbound);
	free_priority_queue(&e->record);
	
	free(e);
	
}
