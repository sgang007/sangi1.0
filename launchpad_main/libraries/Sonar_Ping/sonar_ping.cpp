#include "sonar_ping.h"

sonar_ping::sonar_ping()
{
	signal_pin = 10;
}

sonar_ping::sonar_ping(int sig)
{
	signal_pin = sig;
}

long sonar_ping::get_duration()
{
//sending pulse
  pinMode(signal_pin, OUTPUT);
  digitalWrite(signal_pin, LOW);
  delayMicroseconds(10);
  digitalWrite(signal_pin, HIGH);
  delayMicroseconds(25); //change this to change pulse duration
  digitalWrite(signal_pin, LOW);
  delayMicroseconds(10);
  //read pulse width from echo
  pinMode(signal_pin, INPUT);

  return pulseIn(signal_pin, HIGH);   
  	
}

long sonar_ping::get_distance_cm()
{
	return sonar_ping::get_duration() / 29.1 / 2;
}
long sonar_ping::get_distance_inches()
{
	return sonar_ping::get_duration() / 74 / 2;
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