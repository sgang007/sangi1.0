//	* +V connection of the PING))) attached to +5V
//	* GND connection of the PING))) attached to ground
//	* SIG connection of the PING))) attached to digital pin 7

//#include <MatrixMath.h>
#define PX 1.0347*x+0.7794
#define MAX_RANGE 150
#define FORWARD_STEP 1 //use 1,2,3 or 5
#define TIME_PER 50
//map size must be >max_range/forward_step
//global variable initialization
#define PI 3.147832

float scan[MAX_RANGE/FORWARD_STEP]; //array storing obstacle probability for discrete disctance elements with each step
const int pingPin = 7;

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
//==============================================================================================================
void setup() {
  Serial.begin(9600);
}
float gaussian(int x, float mean, float variance)
{
  float result=(1./sqrt(2*PI*variance))*exp((-0.5*pow((x-mean),2))/variance);
  return result;
}

//calculate combination of 2 gaussians
float update(float mean1,float var1,float mean2,float var2)
{
    float mean=(mean1*var2+mean2*var1)/(var1+var2);
    float variance=(var1*var2)/(var1+var2);
        
}
float get_distance()
{
  long duration;
  float d,x;
  //sending pulse
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(10);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(25); //change this to change pulse duration
  digitalWrite(pingPin, LOW);
  delayMicroseconds(10);
  //read pulse width from echo
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH); 
  x = microsecondsToCentimeters(duration); //convert echo time to distance
  
  //calculated approximate distance in centimetres
  
  return PX;
  
}

//Matrix.Multiply((float*)A,(float*)B,N,N,N,(float*)C);
//Matrix.Print((float*)A,N,N,"A");
//Matrix.Add((float*) B, (float*) C, N, N, (float*) C);
//Matrix.Copy((float*)A,N,N,(float*)B);
//Matrix.Invert((float*)A,N);

void fill_scan(){
  float x1=get_distance();
  delay(TIME_PER);
  float x2=get_distance();
  delay(TIME_PER);
  float x3=get_distance();
  float mean=(x1+x2+x3)/3.0;
  float variance=(pow((x1-mean),2)+pow((x2-mean),2)+pow((x3-mean),2))/3.0;
  if (variance < FORWARD_STEP/2.0)
  variance=FORWARD_STEP/2.0;
  //without this constraint the gaussian will have a low covariance with no values in the discrete distance elements
  Serial.print("Mean:");
  Serial.print(mean);
  Serial.print("   Variance:");
  Serial.print(variance);
  Serial.println();
    
  for(int i=0; i<MAX_RANGE/FORWARD_STEP; i++)
  {
    scan[i]=gaussian(i*FORWARD_STEP,mean,variance);
    Serial.print("[");
    printFloat(scan[i],4);
    Serial.print("],");
  }
}
void loop()
{
  
  fill_scan();
  //write next part of the code here
  while(1);
  //remove this infinite while loop to keep running the code
  
  
}

long microsecondsToInches(long microseconds)
{
  
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
