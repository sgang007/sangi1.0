#ifndef __SONAR_PING__
#define __SONAR_PING__ 

#include "Energia.h"

class sonar_ping
{
public:
	sonar_ping();
	sonar_ping(int sig);
	long get_distance_cm();
	long get_distance_inches();
	~sonar_ping();

private:
	int signal_pin;
	long get_duration();
	
};

#endif