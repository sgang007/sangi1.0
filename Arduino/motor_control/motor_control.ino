
//#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <DualVNH5019MotorShield.h>

#define COUNTS_PER_REV 1200*2 
//As per Pololu website 1200 ticks is to one revolution of motor shaft but experiments shows 2400

//Left Motor Pin Definitions

#define ENDIAG1     6
#define InA1        7
#define InB1        8
#define PWM1        9
#define CS1         A0
#define encA1       5
#define encB1       3

//Right Motor Pin Definitions

#define ENDIAG2     11
#define InA2        13  
#define InB2        12
#define PWM2        10
#define CS2         A1
#define encA2       4
#define encB2       2


DualVNH5019MotorShield md(InA1,InB1,ENDIAG1,CS1,InA2,InB2,ENDIAG2,CS2);
Encoder en1(encA1,encB1);
Encoder en2(encA2,encB2);
int enc1_rpm,enc2_rpm;

void setup() {
    md.init();
  
    Serial.begin(115200);
    Serial.println("Motor Driver and Encoder Test \n --------------------------");

}
void stopIfFault()
{
  if (md.getM1Fault())
  {
    Serial.println("M1 fault");
    while(1);
  }
  if (md.getM2Fault())
  {
    Serial.println("M2 fault");
    while(1);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

      md.setSpeeds(10,10 );
      //md.setM1Speed(50);
      //md.setM2Speed(200);
  
  if (en1.read()>COUNTS_PER_REV|| en1.read()<-COUNTS_PER_REV)
  {
    //1 Revoultion complete
    enc1_rpm++;
    Serial.print("Left Motor(Revs): ");
    Serial.print(enc1_rpm);
    
    if (en1.read()<0)
    Serial.print(" anti-clockwise");
    else
    Serial.print("clockwise");
    
    en1.write(0);
    
    Serial.println();
  }
  
  
  if (en2.read()>COUNTS_PER_REV|| en2.read()<-COUNTS_PER_REV)
  {
    //1 Revoultion complete
    enc2_rpm++;
    Serial.print("\t Right Motor(Revs): ");
    Serial.print(enc2_rpm);
    
    if (en2.read()<0)
    Serial.print(" anti-clockwise");
    else
    Serial.print("clockwise");
    
    en2.write(0);
    
    Serial.println();
  }  
  

}
