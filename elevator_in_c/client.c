
#include "client.h"

struct client construct_random_client(int cfr[2], int nfr[2], int tp0r[2]) {
	// construct a random client
	
	struct client c;
	
	c.current_floor = cfr[0] + (rand() % (cfr[1] - cfr[0] + 1));
	
	do {
		c.next_floor = nfr[0] + (rand() % (nfr[1] - nfr[0] + 1));
		
	} while (c.next_floor == c.current_floor);
	
	
	
	c.time_punches[0] = tp0r[0] + (rand() % (tp0r[1] - tp0r[0] + 1));
	
	return c;
	
	
}