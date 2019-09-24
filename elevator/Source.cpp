
#include "Client.h"
#include "Elevator.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

int main() {

	srand(time(0));

	forward_list<Elevator> elevators;
	forward_list<Client> clients;


	
	// testing Elevator::send_inbound, Elevator::send_outbound, Elevator::ship (basically the general flow of the program)
	// ... for a single elevator with a list of clients

	elevators.push_front(Elevator(100, 5, 10, 20));


	for (int i = 0; i < 10; i++) {

		clients.push_front(Client(rand() % 10, rand() % 10, rand() % 1000));
	}

	Elevator::test_flow(elevators.front(), clients);

	// end of test
	

	/*
	// start of simulation

	int num_elevators, num_clients;

	
	int i;

	for (i = 0; i < num_elevators; ++i) {
		// random generation of elevators
		// assuming all the elevators have the same speed

		elevators.push_front(Elevator(i + 1, rand() % 10, 5, 15));

	}

	for (i = 0; i < num_clients; ++i) {
		// random generation of clients

		clients.push_front(Client(rand() % 10, rand() % 10, rand() % 3600));

	}

	// main simulation
	Elevator::simulation(elevators, clients);
	*/
	


	return 0;
}

