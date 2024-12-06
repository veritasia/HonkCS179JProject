#include "radio_utils.h"
#include <SoftwareSerial.h>

SoftwareSerial lora(2, 3);

void setup() {
  // put your setup code here, to run once:
  lora.begin(115200);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(MakeSendPacket(0, 1, 2, 3));
  lora.println(MakeSendPacket(0, 1, 2, 3));
  delay(1000);
}
