
#include <stdlib.h>

struct client {

	int current_floor, next_floor, time_punches[3];
	
	struct client* next;

};

// cfr = current floor range
// nfr = next floor range
// tp0r = time punches [0] range
struct client construct_random_client(int cfr[2], int nfr[2], int tp0r[2]);
