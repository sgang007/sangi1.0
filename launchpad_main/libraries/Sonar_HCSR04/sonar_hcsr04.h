#ifndef HC_SR04
#define HC_SR04

#include "Energia.h"


class sr04_sonar
{
	public:
		sr04_sonar();
		sr04_sonar(int echo,int trigger);
		void init();
		float apply_filter(int dist);
		int get_distance_cm();
		int get_distance_inches();

	private:
		const int echo_pin;
		const int trig_pin;
}

#endif
