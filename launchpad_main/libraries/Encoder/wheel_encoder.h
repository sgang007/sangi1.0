#ifndef __WHEEL_ENCODER__
#define __WHEEL_ENCODER__ 


#include <stdint.h>
#include "Energia.h"
#include "driverlib/sysctl.h"
#include "driverlib/qei.h"

class wheel_encoder
{
public:
	wheel_encoder(bool use_qei=true);
	//set the use_qei to true to use the inbuilt quadrature encoder interface
	wheel_encoder(int motor1_a, int motor1_b, int motor2_a, int motor2_b, bool use_qei);
	void init();

	//Here Motor1 is connected via external interrupts.
	uint32_t get_Motor1_velocity();
	uint32_t get_Motor1_position();
	int32_t get_Motor1_direction();

	//Here Motor2 is the connected to the QEI Module at PL_1 and PL_2 of TM4C1294
	uint32_t get_Motor2_velocity();
	uint32_t get_Motor2_position();
	int32_t get_Motor2_direction();

	float get_forward_velocity();
	float get_rotational_velocity();
	float get_cumulative_position(float prev_position, float dt);
	
	~wheel_encoder();

private:
	bool qei_mode_flag;
	


};

#endif