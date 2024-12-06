#include <SoftwareSerial.h>
#include "radio_utils.h"

SoftwareSerial lora(2, 3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lora.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (lora.available()) {
    char* data = ParseReceivePacket(lora.readString());
    Serial.print(data);
    Serial.print("\n");
  }
  // Serial.println("loop");
  delay(10);
}
