#ifndef HC_SR04
#define HC_SR04

#include "Energia.h"


class sr04_sonar
{
	public:
		sr04_sonar();
		sr04_sonar(int echo,int trigger);
		void init();
		float apply_filter(long dist);
		long get_distance_cm();
		long get_distance_inches();

	private:
		int echo_pin;
		int trig_pin;
		long get_duration();
}

#endif
