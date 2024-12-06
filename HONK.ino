#include <avr/wdt.h>
#include "DeviceDrivers.h"

Dd_Set_Motor mots;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  wdt_enable(WDTO_2S);
  //mots.Dd_Set_Motor_Init();
  

}

void loop() {
  // put your main code here, to run repeatedly:
  //mots.Dd_Set_Motor_Init();
  
  mots.Dd_Set_Motor_Control(direction_forward, Max_speed, direction_reverse, Max_speed, true);
  
  
}