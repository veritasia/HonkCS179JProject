#include "DeviceDrivers.h"

#include <avr/wdt.h>



static void
delay_xxx(uint16_t _ms)
{
  wdt_reset();
  for (unsigned long i = 0; i < _ms; i++)
  {
    delay(1);
  }
}


//Motor drivers


void Dd_Set_Motor::Dd_Set_Motor_Init(void){
  //A = left motor
  //B = right motor
  //puts motors in ready/standby then takes out for test
  digitalWrite(PIN_Motor_STBY, HIGH);
  digitalWrite(PIN_Motor_AIN_1, HIGH);
  analogWrite(PIN_Motor_PWMA, 100);
  digitalWrite(PIN_Motor_BIN_1, HIGH);
  analogWrite(PIN_Motor_PWMB, 100);
  delay_xxx(5000);

  digitalWrite(PIN_Motor_STBY, LOW);
  delay_xxx(5000);
  digitalWrite(PIN_Motor_STBY, HIGH);
  digitalWrite(PIN_Motor_AIN_1, LOW);
  analogWrite(PIN_Motor_PWMA, 100);
  digitalWrite(PIN_Motor_BIN_1, LOW);
  analogWrite(PIN_Motor_PWMB, 100);

  delay_xxx(1000);

}

void Dd_Set_Motor::Dd_Set_Motor_Control(bool dir_A, uint8_t speed_A,
                          bool dir_B, uint8_t speed_B,
                          bool enableMotors)
{
  if(speed_A > Max_speed){
    speed_A = Max_speed;
  }



  if(enableMotors == true){
    digitalWrite(PIN_Motor_STBY, HIGH);
    //Switch cases for driver side(A) direction
    {
      switch(dir_A)
      {
        case direction_forward:
        digitalWrite(PIN_Motor_AIN_1, HIGH);
        analogWrite(PIN_Motor_PWMA, speed_A);
        break;

        case direction_reverse:
        digitalWrite(PIN_Motor_AIN_1, LOW);
        analogWrite(PIN_Motor_PWMA, speed_A);
        break;

        case direction_null:
        analogWrite(PIN_Motor_PWMA, 0);
        digitalWrite(PIN_Motor_STBY, LOW);
        break;

        default:
        analogWrite(PIN_Motor_PWMA, 0);
        digitalWrite(PIN_Motor_STBY, LOW);
        break;
      }
    }
    //Switch cases for pass side (B)
    {
      switch(dir_B)
      {
        case direction_forward:
        digitalWrite(PIN_Motor_BIN_1, HIGH);
        analogWrite(PIN_Motor_PWMB, speed_B);
        break;

        case direction_reverse:
        digitalWrite(PIN_Motor_BIN_1, LOW);
        analogWrite(PIN_Motor_PWMB, speed_B);
        break;

        case direction_null:
        analogWrite(PIN_Motor_PWMB, 0);
        digitalWrite(PIN_Motor_STBY, LOW);
        break;

        default:
        analogWrite(PIN_Motor_PWMB, 0);
        digitalWrite(PIN_Motor_STBY, LOW);
        break;
      }

    }
  }
  else{
    digitalWrite(PIN_Motor_STBY, LOW);
    return;
  }
}









