#include <avr/wdt.h>
#include "DeviceDrivers.h"
#include "radio_utils.h"
#include <SoftwareSerial.h>

SoftwareSerial lora(62, 34);
Dd_Set_Motor mots;
bool fwd_flag = false;
bool back_flag = false;
bool right_flag = false;
bool left_flag = false;
int time_since_packet = 500;

void setup() {
  // put your setup code here, to run once:
  lora.begin(9600);
  Serial.begin(9600);
  wdt_enable(WDTO_2S);
  pinMode(40, OUTPUT);
  //mots.Dd_Set_Motor_Init();
  digitalWrite(40, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  //mots.Dd_Set_Motor_Init();
  // if (time_since_packet > 5000) {
  //   mots.Dd_Set_Motor_Control(direction_reverse, 0, direction_reverse, 0, false);
  // }
        // mots.Dd_Set_Motor_Control(direction_forward, Max_speed / 2, direction_forward, Max_speed / 2, true);
        
  if (lora.available()) {
    time_since_packet = 0;
    String data = lora.readString();
    Serial.println(data);
    uint8_t pitch = uint8_t(data[9]);
    uint8_t roll = uint8_t(data[10]);
    uint8_t fingerstate = uint8_t(data[11]);
    Serial.println("pitch " + String(pitch));
    Serial.println("roll " + String(roll));
    Serial.println("finger " + String(fingerstate));    
    

    if(fingerstate == 2){
      digitalWrite(40, HIGH);
      delay(500);
      digitalWrite(40, LOW);
    }
    else {
      digitalWrite(40, LOW);
    }
    
    // set flags
    if (pitch > 200) {
      fwd_flag = true;
      back_flag = false;
    }
    else if (pitch < 100) {
      back_flag = true;
      fwd_flag = false;
    }
    else {
      fwd_flag = false;
      back_flag = false;
    }

    if (roll > 200) {
      right_flag = true;
      left_flag = false;
    }
    else if (roll < 100) {
      left_flag = true;
      right_flag = false;
    }
    else {
      left_flag = false;
      right_flag = false;
    }    

  //   if (right_flag && !(fwd_flag || back_flag)) {
  //     // go right
  //     mots.Dd_Set_Motor_Control(direction_forward, Max_speed / 2, direction_reverse, Max_speed / 2, true);
  //   }
  //   else if (left_flag && !(fwd_flag || back_flag)) {
  //     // go left
  //     mots.Dd_Set_Motor_Control(direction_reverse, Max_speed / 2, direction_forward, Max_speed / 2, true);
  //   }

  //   if (!(fwd_flag && back_flag && right_flag && left_flag)) {
  //     // stop
  //     mots.Dd_Set_Motor_Control(direction_reverse, 0, direction_reverse, 0, false);
  //   }
  // }    
  }
  if(fwd_flag){
      if(left_flag){
        mots.Dd_Set_Motor_Control(direction_forward, Max_speed / 4, direction_forward, Max_speed / 2, true);
      }
      else if(right_flag){
        mots.Dd_Set_Motor_Control(direction_forward, Max_speed / 2, direction_forward, Max_speed / 4, true);
      }
      else {
        mots.Dd_Set_Motor_Control(direction_forward, Max_speed / 2, direction_forward, Max_speed / 2, true);
      }
    }
    else if(back_flag){
      if(left_flag){
        mots.Dd_Set_Motor_Control(direction_reverse, Max_speed / 4, direction_reverse, Max_speed / 2, true);
      }
      else if(right_flag){
        mots.Dd_Set_Motor_Control(direction_reverse, Max_speed / 2, direction_reverse, Max_speed / 4, true);

      }
      else {
        mots.Dd_Set_Motor_Control(direction_reverse, Max_speed / 2, direction_reverse, Max_speed / 2, true);
      }
    }
    else {
      if(left_flag){
        mots.Dd_Set_Motor_Control(direction_reverse, Max_speed / 2, direction_forward, Max_speed / 2, true);
      }
      else if(right_flag){
        mots.Dd_Set_Motor_Control(direction_forward, Max_speed / 2, direction_reverse, Max_speed / 2, true);
      }
      else {
        mots.Dd_Set_Motor_Control(direction_reverse, 0, direction_reverse, 0, false);
      }
    }
  delay(20);
  time_since_packet += 10;
}