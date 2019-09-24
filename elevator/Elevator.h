#pragma once

#include <queue>
#include <forward_list>
#include "Client.h"
using std::forward_list;
using std::queue;


class Elevator {

private :

	// id is the elevator's special id
	// floor is the elevator's current floor
	int id, floor;

	enum state_control { ACTIVE, IDLE } state = IDLE;
	enum direction { UP, DOWN } dir = DOWN;

	// active_period the time it takes the elevator to travel one floor
	// idle_period is the time it takes the elevator to wait on each floor
	// active_time is a time counter (seconds) for each time it moves up a floor
	// idle_time is a time counter (seconds) for each time it stops at a floor
	// ... it resets to 0 when it begins moving again or a person presses a button on that floor when it is idle
	int active_period, idle_period, active_time = 0, idle_time = 0;

public :

	// master simulation
	static void simulation(forward_list<Elevator>& elevators, forward_list<Client>& clients);

	// helper functions of the master simulation
	static bool are_empty(const forward_list<Elevator>& elevators);
	static Elevator& closest(forward_list<Elevator>& elevators, Client& c);
	static int dist(const Elevator& e, const Client& c);

	// tests Elevator::send_inbound, Elevator::send_outbound, and Elevator::ship
	// testing of the general flow of the program
	static void test_flow(Elevator& elevator, forward_list<Client>& clients);
	

	// initialized to 0 at the start of the simulation
	// the master clock for the simulation (seconds)
	static unsigned int time;

	// constructor for elevator
	Elevator(int identification, int starting_floor, int ap, int ip);

	// changing state
	void active();
	void idle();

	// changing direction
	void up();
	void down();

	// getter
	const int get_id() const;


	// singly linked lists
	// [0] -> priority queue going down
	// [1] -> priority queue going up

	// clients that have pushed a button and are waiting to be picked up
	forward_list<Client> inbound[2];

	// clients that are on the elevator and waiting to be dropped off
	forward_list<Client> outbound[2];

	// sends a client inbound/outbound depending on the direction and state of the elevator
	// ...sorts it depending on the direction of elevator
	void send_inbound(Client& c);
	void send_outbound(Client& c);
	
	// drops a client off, popping them from outbound
	// ...and pushes them to a record
	void ship(Client& c);

	// a record of all the clients' time punches in the order in when they were dropped off
	queue<Client> record;

	// tester functions
	void output_inbound() const;
	void output_outbound() const;
	void output_record();
	

};

