#include <stdint.h>
#include "driverlib/sysctl.h"
#include "driverlib/qei.h"

// On TM4C129, connect phase A to PL_1 and phase B to PL_2

int pos;


//setup function calls setup for individual components
void setup() {
  Serial.begin(9600);
  encoder_setup();
 
}


void encoder_setup()
{
   // Setup QEI Module
  SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
  GPIOPinConfigure(0x000A0406); //GPIO_PL1_PHA0
  GPIOPinConfigure(0x000A0806); //GPIO_PL2_PHB0
  GPIOPinTypeQEI(GPIO_PORTL_BASE, GPIO_PIN_1 |  GPIO_PIN_2);

  // Configure and Enable QEI
  QEIConfigure(QEI0_BASE, (QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP), 20000);
  QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_1, SysCtlClockGet()); // Divide by clock speed to get counts/sec
  QEIEnable(QEI0_BASE);
  QEIVelocityEnable(QEI0_BASE);
  
  //TODO: External interrup setup for 2nd Encoder
}


//Function to print floating numbers on the serial terminal
void printFloat(float value, int places) {
  // this is used to cast digits 
  int digit;
  float tens = 0.1;
  int tenscount = 0;
  int i;
  float tempfloat = value;

    // make sure we round properly. this could use pow from <math.h>, but doesn't seem worth the import
  // if this rounding step isn't here, the value  54.321 prints as 54.3209

  // calculate rounding term d:   0.5/pow(10,places)  
  float d = 0.5;
  if (value < 0)
    d *= -1.0;
  // divide by ten for each decimal place
  for (i = 0; i < places; i++)
    d/= 10.0;    
  // this small addition, combined with truncation will round our values properly 
  tempfloat +=  d;

  // first get value tens to be the large power of ten less than value
  // tenscount isn't necessary but it would be useful if you wanted to know after this how many chars the number will take

  if (value < 0)
    tempfloat *= -1.0;
  while ((tens * 10.0) <= tempfloat) {
    tens *= 10.0;
    tenscount += 1;
  }


  // write out the negative if needed
  if (value < 0)
    Serial.print('-');

  if (tenscount == 0)
    Serial.print(0, DEC);

  for (i=0; i< tenscount; i++) {
    digit = (int) (tempfloat/tens);
    Serial.print(digit, DEC);
    tempfloat = tempfloat - ((float)digit * tens);
    tens /= 10.0;
  }

  // if no places after decimal, stop now and return
  if (places <= 0)
    return;

  // otherwise, write the point and continue on
  Serial.print('.');  

  // now write out each decimal place by shifting digits one by one into the ones place and writing the truncated value
  for (i = 0; i < places; i++) {
    tempfloat *= 10.0; 
    digit = (int) tempfloat;
    Serial.print(digit,DEC);  
    // once written, subtract off that digit
    tempfloat = tempfloat - (float) digit; 
  }
}

void loop() {
  // Report Position & Velocity of Encoder to Serial Terminal
  Serial.print(QEIPositionGet(QEI0_BASE));
  Serial.print(',');
  Serial.println(QEIVelocityGet(QEI0_BASE));
}
