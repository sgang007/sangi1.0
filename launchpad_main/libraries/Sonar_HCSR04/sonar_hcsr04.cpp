#include "sonar_hcsr04.h"


sr04_sonar::sr04_sonar()
{
	echo_pin = 9;
	trig_pin = 10;
}

sr04_sonar::sr04_sonar(int echo,int trigger)
{
	echo_pin = echo;
	trig_pin = trigger;
}

void sr04_sonar::init()
{
	pinMode(trig_pin , OUTPUT);
  	pinMode(echo_pin , INPUT);
}

long sr04_sonar::get_duration()
{
	digitalWrite(Trig, LOW);
	delayMicroseconds(2);
  	digitalWrite(Trig, HIGH);
  	delayMicroseconds(10);
  	digitalWrite(Trig, LOW);
  	
  	//duration of the return pulse is returned
  	return pulseIn(echo, HIGH);
  	
}
long sr04_sonar::get_distance_cm()
{
	return sr04_sonar::get_duration() / 29.1 / 2;
}
long sr04_sonar::get_distance_inches()
{
	return sr04_sonar::get_duration() / 74 / 2;
}

float apply_filter(long dist)
{
	if(dist <= 4)
		return -1;
	if(dist >= 200)
		return -1;

#ifndef REG_EQN
	#define REG_EQN 1.0347*dist + 0.7794
#endif
	return REG_EQN;
}