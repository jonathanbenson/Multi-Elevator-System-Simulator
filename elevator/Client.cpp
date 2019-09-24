
#include "Elevator.h"
#include "Client.h"

bool Client::cmp_current_floor_ascending(Client a, Client b) {

	if (a.get_current_floor() < b.get_current_floor())
		return true;
	else
		return false;

}

bool Client::cmp_current_floor_descending(Client a, Client b) {

	if (a.get_current_floor() > b.get_current_floor())
		return true;
	else
		return false;

}

// comparative functions for forward_list::sort below

bool Client::cmp_next_floor_ascending(Client a, Client b) {

	if (a.get_next_floor() < b.get_next_floor())
		return true;
	else
		return false;

}

bool Client::cmp_next_floor_descending(Client a, Client b) {

	if (a.get_next_floor() > b.get_next_floor())
		return true;
	else
		return false;

}

bool Client::cmp_first_time_punch_ascending(Client a, Client b) {

	if (a.get_time_punch(0) < b.get_time_punch(0))
		return true;
	else
		return false;

}

Client::Client(int cf, int nf, int tp0) {
	// client constructor

	current_floor = cf;
	next_floor = nf;

	time_punch(0, tp0);

}


void Client::time_punch(unsigned int i, unsigned int time) {

	time_punches[i] = time;

}

int Client::get_time_punch(unsigned int i) const {

	return time_punches[i];

}

int Client::get_elevator_id() const {

	return elevator_id;

}

int Client::get_current_floor() const {

	return current_floor;

}

int Client::get_next_floor() const {

	return next_floor;

}

bool Client::is_aboard() const {

	return aboard;

}

void Client::board(int e_id) {

	aboard = true;
	elevator_id = e_id;

}


