#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#define THRES 200   //descrease for better button retention
#define SAMPLES 20  //i think ^ || but quicker
#define LED1 3      //D3

typedef struct touchButton{
  int button;
  int capVal;
  int valueType;
  char state = 0;
  char hornState = 0;    
};

touchButton touchButton1;  //can add more (ex: mainButton2 if we desire more buttons)

Adafruit_MPU6050 mpu;
bool turnLeft = false;
bool turnRight = false;
long voltVal;
const int MPU_addr1 = 0x68;
float X, Y, Z, roll, pitch;
float value1;

enum ROLLAXIS {LEFT, HOLD, RIGHT} dState = HOLD;
void pivotMotion(void)   { 
  switch (dState)    {
      case HOLD:
          if (roll > 50.0 && (pitch > -50.0 && pitch < 50.0))  {dState = RIGHT;}    //Y > -0.10 && Y < 0.10
          else if (roll < -50.0 && (pitch > -50.0 && pitch < 50.0))  {dState = LEFT;}
          else {dState = HOLD;}
      break;

      case RIGHT:
          turnRight = true;
          if (roll > 50.0)   {dState = RIGHT;}
          else {dState = HOLD;}
      break;

      case LEFT:
          turnLeft = true;
          if (roll < -50.0)   {dState = LEFT;}
          else {dState = HOLD;}
      break;

    default:
      HOLD;
    break;
    }
//****************************send values via LoRa here****************************
    switch (dState)    {
      case HOLD:
      break;

      case RIGHT:
          Serial.print("RIGHT");
      break;
      
      case LEFT:
          Serial.print("LEFT");
      break;

      default:
      break;
    }
}

enum PITCHAXIS {FORWARD, WAIT, BACK} aState = WAIT;
void movementMotion(void)   {  
  switch (aState)    {
      case WAIT:
          if (pitch > 50.0)  {aState = FORWARD;}   
          else if (pitch < -50.0)  {aState = BACK;}
          else {aState = WAIT;}
      break;

      case FORWARD:
          if (pitch > 50.0)   {aState = FORWARD;}
          else {aState = WAIT;}
      break;

      case BACK:
          if (pitch < -50.0) {aState = BACK;}
          else {aState = WAIT;}
      break;

    default:
      WAIT;
    break;
    }
//****************************send values via LoRa here****************************
    switch (aState)    {
      case WAIT:
      break;

      case FORWARD:
          Serial.print("FORWARD");
      break;
      
      case BACK:
          Serial.print("BACK");
      break;

      default:
      break;
    }
}

void touchButtonInit(touchButton *touchTap, int button){
  touchTap->button=button;
  touchTap->capVal = (capacitor2(button) - capacitor1(button));
  DIDR0 |= 1;
  DIDR0 |= 1 << (button-A0);
}

int capacitor1(int senseButton){  //VCC becomes input so button -> ground by pull-up R (low)
  ADMUX = 0b01000000;
  pinMode(senseButton, OUTPUT);
  digitalWrite(senseButton, 0);
  pinMode(senseButton, INPUT);
  ADMUX = 0b01000000 | senseButton-A0;
  ADCSRA |= 1<<ADSC;
  while((ADCSRA & (1<<ADSC)) != 0);
  
  return ADC;
}

int capacitor2(int senseButton){  //ground becomes input so button -> VCC charge by pull-up R (high)
  ADMUX = 0b01001111;
  pinMode(senseButton, INPUT_PULLUP);
  pinMode(senseButton, INPUT);
  ADMUX = 0b01000000 | senseButton-A0;
  ADCSRA |= 1<<ADSC;
  while((ADCSRA & (1<<ADSC)) != 0); //wait
  
  return ADC;
}

void touchButtonCheck(touchButton *touchTap){
  static float cap1, cap2;
  cap1, cap2=0;
  for(int i = 0; i < SAMPLES; i++){
    cap1 += capacitor1(touchTap->button);
    cap2 += capacitor2(touchTap->button);
  }

  cap1 /= SAMPLES;
  cap2 /= SAMPLES;
  touchTap->valueType = (cap2 - cap1);
  if(touchTap->valueType > (touchTap->capVal + THRES))  
    touchTap->state=1;  //change in sensor reading = touch
  else{
    touchTap->state=0;
    touchTap->capVal=((float)touchTap->capVal*0.9)+((float)touchTap->valueType*0.1);
  }
  
  if(touchTap->state == 1)  {touchTap->hornState = 1;}    //hornState might be easier to send to HONK as 0 or 1
  else {touchTap->hornState = 0;}
}

void setup() {
  touchButtonInit(&touchButton1, A1);
  Wire.begin();                                      
  Wire.beginTransmission(MPU_addr1);                 //send the slave addr(68)
  Wire.write(0x6B);                                  
  Wire.write(0);
  Wire.endTransmission(true);                        
  Serial.begin(9600);
  pinMode(A0, INPUT_PULLUP);  //this is our reference pin for voltage/capitance
  analogRead(A0);

//for testing button
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, 0);
  
}

void loop() {
  Wire.beginTransmission(MPU_addr1);
  Wire.write(0x3B);  //send starting reg address 4 accel 8msb
  Wire.endTransmission(false); 
  Wire.requestFrom(MPU_addr1, 6); //retrieve 6byt accel vals
  int w = Wire.read();
  X = (w << 8) | Wire.read();
  w = Wire.read();
  Y = (w << 8) | Wire.read();
  w = Wire.read();
  Z = (w << 8) | Wire.read();
  roll = atan2(Y, Z)*180.0/PI;
  pitch = atan2(-X, sqrt(Y*Y+Z*Z))*180.0/PI; //works around roll values

//  Serial.print("roll = ");
//  Serial.print(roll,1);
//  Serial.print(", pitch = ");
//  Serial.println(pitch,1);
//  delay(100);
  
  touchButtonCheck(&touchButton1);   //might need an inital set for horn = 0
  //testing
//  Serial.print(touchButton1.valueType);
  delay(10);
  digitalWrite(LED1, touchButton1.hornState);

pivotMotion();      //calls pivot SM - prints LEFT and RIGHT
Serial.println("");
movementMotion();   //calls pitch SM - print FORWARD and BACK
Serial.println("");
}
