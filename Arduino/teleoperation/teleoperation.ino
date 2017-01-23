//#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <DualVNH5019MotorShield.h>

//#define COUNTS_PER_REV 1200*2 
#define COUNTS_PER_REV 3200


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


//TODO: Read all tuning parameters from EEPROM
#define FVEL_STEP 10
#define AVEL_STEP 1
#define WHEEL_DIST 30
#define MAX_SPEED 100
#define SPEED_FACTOR 400/MAX_SPEED

DualVNH5019MotorShield md(InA1,InB1,ENDIAG1,CS1,InA2,InB2,ENDIAG2,CS2);
Encoder en1(encA1,encB1);
Encoder en2(encA2,encB2);
int enc1_rpm,enc2_rpm;
int speed1,speed2,forward_vel,angular_vel;
char c;

void setup() {
    md.init();
  
    Serial.begin(115200);
    Serial.println("Motor Driver and Encoder Test \n --------------------------");
    speed1=speed2=forward_vel=angular_vel=0;

}
void stopIfFault()
{
  if (md.getM1Fault())
  {
    Serial.println("M1 fault");
    md.setSpeeds(0,0);
    while(1);
  }
  if (md.getM2Fault())
  {
    Serial.println("M2 fault");
    md.setSpeeds(0,0);
    while(1);
  }
}

int motor1(float v, float w)
{
  return int(3.1776*(v + 6*w));
}

int motor2(float v, float w)
{
  return int(3.1776*(v+6*w));
}

void loop() {
  // put your main code here, to run repeatedly:

  stopIfFault();
  
  // Read encoder1 outputs and display on screen    
  
  if (en1.read()>COUNTS_PER_REV|| en1.read()<-COUNTS_PER_REV)
  {
    //1 Revoultion complete
    enc1_rpm++;
    
    if (en1.read()<0)
    Serial.print(-enc1_rpm);
    else
    Serial.print(enc1_rpm);
    
    en1.write(0);
    
    Serial.print("\t");
  }
  
  //Read encoder 2 outputs and display on screen
  
  if (en2.read()>COUNTS_PER_REV|| en2.read()<-COUNTS_PER_REV)
  {
    
    enc2_rpm++;
    
    if (en2.read()<0)
    Serial.print(enc2_rpm);
    else
    Serial.print(-enc1_rpm);
    
    en2.write(0);
    Serial.println();
    
  }  

//================================================================================
 //Check for Serial Input to control the robot
  if(Serial.available()){
    c = Serial.read();
    switch(c)
    {
      case 'w':
      {
        forward_vel += FVEL_STEP;
        break;
      }
      case 's':
      {
        forward_vel -= FVEL_STEP;
        break;
      }
      case 'a':
      {
        angular_vel += AVEL_STEP;
        break;
      }
      case 'd':
      {
        angular_vel -= AVEL_STEP;
        break;
      }
      case ' ':
      {
         angular_vel=0;
         forward_vel=0;
         break;
      }
      default:
      Serial.println("Invalid Input. Press w/a/s/d");
      
      
      }
      md.setSpeeds( motor1(forward_vel,angular_vel) , motor2(forward_vel,angular_vel) );
      
    }
    
}
  

