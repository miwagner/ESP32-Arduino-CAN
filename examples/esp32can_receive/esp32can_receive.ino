#include <Arduino.h>
#include <ESP32CAN.h>

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32-Arduino-CAN Receive Basic Demo");

  ESP32Can.CANInit(GPIO_NUM_5, GPIO_NUM_4, ESP32CAN_SPEED_500KBPS);
}

void loop() {
  twai_message_t rx_frame;
  
  ESP32Can.CANReadFrame(&rx_frame);

  Serial.print(rx_frame.identifier, HEX);
  Serial.print(" ");
  Serial.print(rx_frame.data_length_code);
  
  for (int i=0; i<rx_frame.data_length_code; i++) {
    Serial.print(rx_frame.data[i], HEX);
  }

  Serial.println();
  
  delay(1000);
}
