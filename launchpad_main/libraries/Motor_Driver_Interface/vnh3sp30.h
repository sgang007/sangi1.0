#ifndef __MOTOR_DRIVER__
#define __MOTOR_DRIVER__ 

#include "Energia.h"
#define PWM_FREQUENCY 20000

class motor_driver
{
public:
	motor_driver(bool input_mode=false);
	motor_driver(char* pinset, bool input_mode);
	void set_speed_motorA(uint8_t speed);
	void set_speed_motorB(uint8_t speed);
	uint8_t read_speed_motorA();
	uint8_t read_speed_motorB();
	check_fault();

	~motor_driver();

private:
	char* pinset;
	bool input_mode;
	void set_PWM(uint8_t pin,uint8_t duty_cycle);
	char* read_pin_definition();
	
};
#endif