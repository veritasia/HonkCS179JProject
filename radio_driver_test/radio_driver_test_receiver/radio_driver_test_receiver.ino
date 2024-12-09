#include <SoftwareSerial.h>
#include "radio_utils.h"
#include "string.h"

SoftwareSerial lora(2, 3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lora.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (lora.available()) {
    String raw_data = lora.readString();
    char* data = ParseReceivePacket(raw_data);
    Serial.println(raw_data);
    Serial.println(String(data[2]));
    if (data[2] == 127) {
      Serial.println("LFG 127");
    }
    uint8_t pitch = raw_data[9];
    uint8_t roll = raw_data[10];
    uint8_t finger = raw_data[11];
    Serial.println(String(pitch) += " pitch");
    Serial.println(String(roll) += " roll");
    Serial.println(String(finger) += " finger");
    Serial.println("AAAAAA");
  }
  // Serial.println("loop");
  delay(10);
}
