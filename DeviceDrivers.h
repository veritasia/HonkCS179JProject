#ifndef _DeviceDrivers_H_
#define _DeviceDrivers_H_


#include "Arduino.h"

class Dd_Set_Motor
{
  public:

    //Dd_Set_Motor () = default;
    void Dd_Set_Motor_Init(void);

    void Dd_Set_Motor_Control(bool dir_A, uint8_t speed_A,
                              bool dir_B, uint8_t speed_B,
                              bool enableMotors
                              );

  public:
    #define Max_speed 255
    #define direction_forward true
    #define direction_reverse false
    #define direction_null 3

    #define Duration_enable true
    #define Duration_disable false
    #define control_enable true
    #define control_disable false
        

  private: 
    //TB6612 Driver pins
    #define PIN_Motor_PWMA 5
    #define PIN_Motor_PWMB 6
    #define PIN_Motor_BIN_1 8
    #define PIN_Motor_AIN_1 7
    #define PIN_Motor_STBY 3


};


class Dd_Set_Servo
{
  public:
    void Dd_Set_Servo_Init(unsigned int position);
    void Dd_Set_Servo_Control(unsigned int position);

  private:
    #define PIN_Servo_z 10
    #define PIN_Servo_y 11

};

#endif