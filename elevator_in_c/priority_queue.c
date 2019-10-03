
#include <stdio.h>
#include "priority_queue.h"

int cmp_node_c_time_punch_0(struct node* a, struct node* b, int descending) {
	// compares the first time punch of a client

	if (descending) {
		if (a->c.time_punches[0] > b->c.time_punches[0])
			return 1;
		else
			return 0;
	}
	else {
		if (a->c.time_punches[0] < b->c.time_punches[0])
			return 1;
		else
			return 0;
	}
	
}

int cmp_node_c_time_punch_2(struct node* a, struct node* b, int descending) {
	// compares the third time punch of a client

	if (descending) {
		if (a->c.time_punches[2] > b->c.time_punches[2])
			return 1;
		else
			return 0;
	}
	else {
		if (a->c.time_punches[2] < b->c.time_punches[2])
			return 1;
		else
			return 0;
	}
	
}

int cmp_node_c_current_floor(struct node* a, struct node* b, int descending) {
	// compares the current floor of two clients

	if (descending) {
		if (a->c.current_floor > b->c.current_floor)
			return 1;
		else
			return 0;
	}
	else {
		if (a->c.current_floor < b->c.current_floor)
			return 1;
		else
			return 0;
	}
	
}

int cmp_node_c_next_floor(struct node* a, struct node* b, int descending) {
	// compares the next floors of two clients

	if (descending) {
		if (a->c.next_floor > b->c.next_floor)
			return 1;
		else
			return 0;
	}
	else {
		if (a->c.next_floor < b->c.next_floor)
			return 1;
		else
			return 0;
	}
	
}

int get_node_c_time_punches_0(struct node* n) {
	// returns a clients first time punch

	return n->c.time_punches[0];
	
}

int get_node_c_time_punches_2(struct node* n) {
	// returns a client's second time punch

	return n->c.time_punches[2];
	
}

int get_node_c_current_floor(struct node* n) {
	// returns a client's current floor

	return n->c.current_floor;
	
}

int get_node_c_next_floor(struct node* n) {
	// returns a client's next floor

	return n->c.next_floor;
	
}

// priority queue functions

struct priority_queue construct_empty_priority_queue() {
	// constructs an empty priority queue

	struct priority_queue pq;

	pq.head = NULL;
	
	return pq;

}

void push_front_priority_queue(struct priority_queue* q, struct node n) {
	// pushes a client to the front of a priority queue

	struct node* nn = (struct node*)malloc(sizeof(struct node));
	
	*nn = n;
	
	nn->next = q->head;
	q->head = nn;
	
}

void push_priority_queue(struct priority_queue* q, struct node n, int (*cmp)(struct node*, struct node*, int), int descending) {
	// pushes a client to a priority queue (sorted) by what the comparative function returns

	struct node* nn = (struct node*)malloc(sizeof(struct node));
	*nn = n;
	
	
	struct node* current = q->head;

	if (q->head == NULL || cmp(nn, q->head, descending)) {
		
		push_front_priority_queue(q, *nn);
		
	}
	else {

		
		while (current->next != NULL && !cmp(nn, current->next, descending)) {
			
			current = current->next;
			
		}

		if (current->next == NULL) {
			
			nn->next = NULL;
			current->next = nn;

		}
		else {
			
			nn->next = current->next;
			current->next = nn;

		}
	}
}

int is_empty_priority_queue(struct priority_queue* pq) {
	// checks if the priority queue is empty

	if (pq->head == NULL)
		return 1;
	return 0;

}

void pop_priority_queue(struct priority_queue* pq) {
	// pops a priority queue

	struct node* temp;
	
	if (pq->head != NULL) {
		
		temp = pq->head->next;
		
		free(pq->head);
		pq->head = temp;
		
	}
	
}

void free_priority_queue(struct priority_queue* pq) {
	// frees the memory of a priority queue

	while (pq->head != NULL) {
		pop_priority_queue(pq);
		
	}

	
	
}

void write_priority_queue_console(struct priority_queue* pc, int (*get_val)(struct node*)) {
	// writes a priority queue to the console

	struct node* current = pc->head;
	
	while (current != NULL) {
		
		printf("%d->", get_val(current));
		
		current = current->next;
		
	}
	
	printf("NULL");
	
	
	
}
