//#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <EEPROM.h>
#include <PID_v1.h>
#include <FlexiTimer2.h>
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
#define INPUT_SIZE 30
#define FVEL_STEP 10
#define AVEL_STEP 1
#define WHEEL_DIST 30.0
#define WHEEL_DIA 12
#define MAX_SPEED 120.0
#define SPEED_FACTOR 400/MAX_SPEED
#define ENCODER_POLL 10              //updates the Encoder RPM count 10 times per second
#define POLL_RESOLUTION 1000         //accuracy is 1/1000 of a second
#define V_KP 1.0
#define V_KD 0.0
#define V_KI 0.0
#define A_KP 1.0
#define A_KD 0.0
#define A_KI 0.0



float enc1_rpm,enc2_rpm;
double current_fvel,current_avel,forward_vel,angular_vel,target_fvel,target_avel;

struct design_params{
    float wheel_dist;
    float speed_factor;
};

struct control_params{
    float Kp;
    float Kd;
    float Ki;
}fvel,avel;


DualVNH5019MotorShield md(InA1,InB1,ENDIAG1,CS1,InA2,InB2,ENDIAG2,CS2);
Encoder en1(encA1,encB1);
Encoder en2(encA2,encB2);
PID control_fvel(&current_fvel, &target_fvel, &forward_vel, double(fvel.Kp), double(fvel.Ki), double(fvel.Kd), DIRECT);
PID control_avel(&current_avel, &target_avel, &angular_vel, double(avel.Kp), double(avel.Ki), double(avel.Kd), DIRECT);



void writeParams()
{  
    //load parameters into an object of design_params
    design_params design_data;
    control_params control_data;
    int eeAddress =0;
    EEPROM.put(eeAddress, design_data);
    eeAddress += sizeof(design_params);
    EEPROM.put(eeAddress, control_data);
        
}

void loadParams()
{
    int eeAddress =0;
    design_params design_data;
    control_params control_data;
    EEPROM.get(eeAddress, design_data);
    eeAddress += sizeof(design_params);
    EEPROM.get(eeAddress, control_data);
    
    //Combine design_data and control_data into parameters and return
    
    
}

void setup() {
  
    md.init();
  
    Serial.begin(115200);
    Serial.println("Motor Driver and Encoder Test \n --------------------------");
    
    current_fvel=current_avel=forward_vel=angular_vel=target_fvel=target_avel=0.0;

    //loadParams();

    fvel.Kp = V_KP;
    fvel.Kd = V_KD;
    fvel.Ki = V_KI;
    avel.Kp = A_KP;
    avel.Kd = A_KD;
    avel.Ki = A_KI;
    
    control_fvel.SetTunings(double(fvel.Kp), double(fvel.Kd), double(fvel.Ki));
    control_fvel.SetOutputLimits(-MAX_SPEED, MAX_SPEED);
    control_fvel.SetMode(AUTOMATIC);
    
    control_avel.SetTunings(double(avel.Kp), double(avel.Kd), double(avel.Ki));
    control_avel.SetOutputLimits(-(2*MAX_SPEED)/WHEEL_DIST, (2*MAX_SPEED)/WHEEL_DIST);
    control_avel.SetMode(AUTOMATIC); 
    
    FlexiTimer2::set(POLL_RESOLUTION/ENCODER_POLL, 1.0/POLL_RESOLUTION, updateRPM);
    FlexiTimer2::start();

}


void updateRPM()
{
  //update RPMs
  enc1_rpm = float((en1.read() * 60 * ENCODER_POLL)) / COUNTS_PER_REV;
  enc2_rpm = float((en2.read() * 60 * ENCODER_POLL)) / COUNTS_PER_REV;

  //reset encoder counts
  en1.write(0);
  en2.write(0);
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
  return int(3.1776*(v - 6*w));
}



void printMotorRPMs()
{
  Serial.print("Motor 1 (RPM):");
  Serial.print(enc1_rpm);
  Serial.print("\t\t\t");
  Serial.print("Motor 2 (RPM):");
  Serial.print(enc2_rpm);
  Serial.println();  
}


void teleoperateFromSerial()
{
  char c;
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
      
      
    }
  
}


void getRobotState()
{
    char input[INPUT_SIZE + 1];
    //while(!Serial.available());
    byte size = Serial.readBytes(input, INPUT_SIZE);
    // Add the final 0 to end the C string
    input[size] = 0;
    
    // Read each command pair
    const char * delim = ","; 
    char* data;
    data = strtok(input, delim);
    forward_vel = atof(data);
    
    data = strtok(NULL, delim);
    angular_vel = atof(data); 
}


void updateRobotState()
{
  float speed1, speed2;
  
  speed1 = -(enc1_rpm * WHEEL_DIA * PI ) / 60.0; 
  speed2 = (enc2_rpm * WHEEL_DIA * PI ) / 60.0;
  
  current_fvel = (speed1 + speed2) / 2.0 ;
  current_avel = (speed1 - speed2) / WHEEL_DIST;
  
}

void setRobotState()
{
  //md.setSpeeds( motor1(forward_vel,angular_vel) , motor2(forward_vel,angular_vel) );
  md.setSpeeds( motor1(target_fvel, target_avel), motor2(target_fvel, target_avel) );
}

void loop() {
  // put your main code here, to run repeatedly:

  stopIfFault();    
  
  //printMotorRPMs();
  updateRobotState();
  Serial.print("Current:  ");
  Serial.print(current_fvel);
  Serial.print(",");
  Serial.print(current_avel);
  Serial.print("\t\t\t");
  
  
  teleoperateFromSerial();
  //getRobotState();
  Serial.print("Target:  ");
  Serial.print(target_fvel);
  Serial.print(":");
  Serial.println(target_avel);

  control_fvel.Compute();
  control_avel.Compute();
  
  setRobotState();
  
   
}
  

