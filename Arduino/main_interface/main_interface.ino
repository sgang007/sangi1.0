#include<Sonar.h>
#include <Motor.h>
void setup()
{
  Sonar.initrialize();
  Motor.initialize();
  Serial.begin(9600);
  //other pin settings
}
void loop()
{
  char buffer[25];
  Sonar.getData(&buffer);
  Motor.getData(&buffer);
  Serial.println(buffer);
  char *data= Serial.read();
  Motor.setLVel(data[0]); //for setting linear velocity of robot
  Motor.setRVel(data[1]); //for setting rotational velocity of robot
  delay(10);
}
