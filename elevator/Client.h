#pragma once


class Client {

private:

	bool aboard = false;
	int current_floor, next_floor;

	int elevator_id = -1;

	// 0 -> press button
	// 1 -> board elevator
	// 2 -> get off elevator
	int time_punches[3] = { -1,-1,-1 };

public:

	// comparative functions for forward_list::sort
	static bool cmp_current_floor_ascending(Client a, Client b);
	static bool cmp_current_floor_descending(Client a, Client b);

	static bool cmp_next_floor_ascending(Client a, Client b);
	static bool cmp_next_floor_descending(Client a, Client b);

	static bool cmp_first_time_punch_ascending(Client a, Client b);


	// constructor
	Client(int cf, int nf, int tp0);

	void time_punch(unsigned int i, unsigned int time);
	int get_time_punch(unsigned int i) const;

	int get_elevator_id() const;
	int get_current_floor() const;
	int get_next_floor() const;

	bool is_aboard() const;

	void board(int e_id);

};



