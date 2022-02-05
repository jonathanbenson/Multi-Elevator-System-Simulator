
#include "priority_queue.h"

struct elevator {
	
	int id, current_floor, active_period, idle_period, active_stopwatch, idle_stopwatch;
	
	enum state_control { ACTIVE, IDLE } state;
	enum direction_control { UP, DOWN, NA } direction;
	
	struct priority_queue downward_inbound;
	struct priority_queue upward_inbound;
	
	struct priority_queue downward_outbound;
	struct priority_queue upward_outbound;
	
	struct priority_queue record;
	
	
};

struct elevator construct_elevator(int cf, int ap, int ip);

struct elevator construct_random_elevator(int cfr[2], int ap, int ip);

int is_elevator_empty(struct elevator* e);
int are_empty_elevators(struct elevator* es, const int SIZE);

struct elevator* get_closest_elevators_node_c(struct elevator* es, const int SIZE, struct node n);
int get_distance_elevator_node_c(struct elevator* e, struct node n);

void push_node_inbound_elevator(struct elevator* e, struct node n, int t);
void push_node_outbound_elevator(struct elevator* e, struct node n, int t);
void push_node_record_elevator(struct elevator* e, struct node n, int t);

int should_change_direction_elevator(struct elevator* e);
void change_direction_elevator(struct elevator* e);

void free_elevator(struct elevator* e);
