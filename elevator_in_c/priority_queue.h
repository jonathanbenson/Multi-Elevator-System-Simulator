
#include "client.h"

struct node {

	struct client c;
	struct node* next;

};

struct priority_queue {

	struct node* head;

};

int cmp_node_c_time_punch_0(struct node* a, struct node* b, int descending);
int cmp_node_c_time_punch_2(struct node* a, struct node* b, int descending);
int cmp_node_c_current_floor(struct node* a, struct node* b, int descending);
int cmp_node_c_next_floor(struct node* a, struct node* b, int descending);

// for writing to the console (what to display)
int get_node_c_time_punches_0(struct node* n);
int get_node_c_time_punches_2(struct node* n);
int get_node_c_current_floor(struct node* n);
int get_node_c_next_floor(struct node* n);


struct priority_queue construct_empty_priority_queue();

void push_front_priority_queue(struct priority_queue* q, struct node n);

void push_priority_queue(struct priority_queue* q, struct node n, int (*cmp)(struct node*, struct node*, int), int descending);

void pop_priority_queue(struct priority_queue* pq);

int is_empty_priority_queue(struct priority_queue* pq);

void free_priority_queue(struct priority_queue* pq);

void write_priority_queue_console(struct priority_queue* pc, int (*get_val)(struct node*));



