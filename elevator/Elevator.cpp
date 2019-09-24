
#include <iostream>
#include "Elevator.h"
#include "Client.h"
using namespace std;

unsigned int Elevator::time;

void Elevator::simulation(forward_list<Elevator>& elevators, forward_list<Client>& clients) {
	// Assumes the elevators all have randomly generated:
	// floor, active_period, idle_period
	// ... and all clients have randomly generated:
	// current_floor, next_floor, time_punches[0]
	
	// This simulation function should pop all clients from the given list
	// ... send them through elevators' inbound and outbound queues
	// ... and eventually push them to the elevators' Elevator::record queues with all three of their time punches recorded


	// refer to Algorithm.txt

	clients.sort(Client::cmp_first_time_punch_ascending);

	time = 0;

	while (!clients.empty() && !are_empty(elevators)) {

		// code

		time++;
	}


	cout << "FIXME: Elevator::simulation" << endl;

}

bool Elevator::are_empty(const forward_list<Elevator>& elevators) {

	forward_list<Elevator>::const_iterator iteri;

	for (iteri = elevators.begin(); iteri != elevators.end(); ++iteri) {

		const Elevator& e = *iteri;

		if (!e.inbound[0].empty() || !e.inbound[1].empty() || !e.outbound[0].empty() || !e.outbound[1].empty())
			return false;

	}

	return true;
}

Elevator& Elevator::closest(forward_list<Elevator>& elevators, Client& c) {
	// returns a reference to the closest elevator in a list of elevators to a given client
	// may need a better algorithm...
	// ... right now it just computes the distance of the elevator's and client's current floor

	Elevator& closest_e = elevators.front();

	forward_list<Elevator>::iterator iteri;

	for (iteri = elevators.begin(); iteri != elevators.end(); iteri++) {

		if (dist(*iteri, c) > dist(closest_e, c)) {

			closest_e = *iteri;

		}
	}

	return closest_e;

}

int Elevator::dist(const Elevator& e, const Client& c) {
	// used in the Elevator::closest function

	return fabs(e.floor - c.get_current_floor());

}

void Elevator::test_flow(Elevator& elevator, forward_list<Client>& clients) {
	// general testing of Elevator::send_inbound, Elevator::send_outbound, and Elevator::ship
	// ... with one elevator and a list of clients

	while (!clients.empty()) {

		elevator.send_inbound(clients.front());
		clients.pop_front();
		
	}

	elevator.output_inbound();

	while (!elevator.inbound[0].empty()) {

		elevator.send_outbound(elevator.inbound[0].front());
		elevator.inbound[0].pop_front();

	}

	while (!elevator.inbound[1].empty()) {

		elevator.send_outbound(elevator.inbound[1].front());
		elevator.inbound[1].pop_front();

	}

	elevator.output_outbound();



	while (!elevator.outbound[0].empty()) {

		elevator.ship(elevator.outbound[0].front());
		elevator.outbound[0].pop_front();

	}

	while (!elevator.outbound[1].empty()) {

		elevator.ship(elevator.outbound[1].front());
		elevator.outbound[1].pop_front();

	}


	elevator.output_record();


}

Elevator::Elevator(int identification, int starting_floor, int ap, int ip) {

	id = identification;
	floor = starting_floor;
	active_period = ap; // seconds it takes to travel one floor
	idle_period = ip; // seconds it takes stopped at one floor

}

void Elevator::send_inbound(Client& c) {
	// sends a client inbound
	// inbound[0] if the client's current floor is below the elevator's current floor
	// inbound[1] if the client's current floor is above the elevator's current floor

	if (c.get_current_floor() == floor) {
		// in this case, the client pressed the button when the elevator was already on their current floor

		send_outbound(c);

	}
	else if (c.get_current_floor() < floor) {
		// the client's current floor is below the elevator's current floor

		// push the client to the downward inbound queue
		inbound[0].push_front(c);
		inbound[0].sort(Client::cmp_current_floor_descending);

	}
	else {
		// the client's current floor is above the elevator's current floor

		// push the client to the upward inbound queue
		inbound[1].push_front(c);
		inbound[1].sort(Client::cmp_current_floor_ascending);

	}

}

void Elevator::send_outbound(Client& c) {
	// sends a client inbound
	// outbound[0] if the client's next floor is below the elevator's current floor
	// outbound[1] if the client's next floor is above the elevator's current floor

	// record the client's second time punch for getting on the elevator
	c.time_punch(1, time);

	if (c.get_next_floor() == floor) {

		// this is the special case...
		// the client pressed the button and got on the elevator on the same floor they wanted to go to.
		ship(c);

	}
	else if (c.get_next_floor() < floor) {
		// the client's next floor is below the elevator's current floor

		// push the client to the downward outbound queue
		outbound[0].push_front(c);
		outbound[0].sort(Client::cmp_next_floor_descending);

	}
	else {
		// the client's next floor is above the elevator's current floor

		// push the client to the upward outbound queue
		outbound[1].push_front(c);
		outbound[1].sort(Client::cmp_next_floor_ascending);

	}

	

}

void Elevator::ship(Client& c) {
	// drops off a client

	// record the client's time punch for getting off the elevator
	c.time_punch(2, time);

	// push the client to the record
	record.push(c);

}

// setters below

void Elevator::active() {

	state = ACTIVE;

}

void Elevator::idle() {

	state = IDLE;

}

void Elevator::up() {

	dir = UP;

}

void Elevator::down() {

	dir = DOWN;

}

const int Elevator::get_id() const {

	return id;

}


// tester functions below

void Elevator::output_inbound() const {
	// outputs the two inbound queues of the elevator
	// ... in terms of the clients' current floors where they are waiting to be picked up

	forward_list<Client>::const_iterator iter;

	cout << "Inbound (sorted by current floor)" << endl << endl;

	cout << "Current floor: " << floor << endl << endl;

	cout << "Priority queue for going up:" << endl;
	for (iter = inbound[1].begin(); iter != inbound[1].end(); iter++) {

		cout << (*iter).get_current_floor() << endl;

	}

	cout << endl;
	cout << "Priority queue for going down:" << endl;
	for (iter = inbound[0].begin(); iter != inbound[0].end(); iter++) {
		
		cout << (*iter).get_current_floor() << endl;

	}

	cout << endl;

}

void Elevator::output_outbound() const {
	// outputs the two outbound queues of the elevator
	// ... in terms of the clients' next floors where they are waiting to be dropped off

	forward_list<Client>::const_iterator iter;

	cout << "Outbound (sorted by next floor)" << endl << endl;

	cout << "Current floor of elevator: " << floor << endl << endl;

	cout << "Priority queue for going up:" << endl;
	for (iter = outbound[1].begin(); iter != outbound[1].end(); iter++) {

		cout << (*iter).get_next_floor() << endl;

	}

	cout << endl;
	cout << "Priority queue for going down:" << endl;
	for (iter = outbound[0].begin(); iter != outbound[0].end(); iter++) {

		cout << (*iter).get_next_floor() << endl;

	}

	cout << endl;


}

void Elevator::output_record() {

	cout << "Record:" << endl;

	while (!record.empty()) {

		cout << record.front().get_time_punch(0) << ',' << record.front().get_time_punch(1) << ',' << record.front().get_time_punch(2) << endl;

		record.pop();

	}

	cout << endl;


}