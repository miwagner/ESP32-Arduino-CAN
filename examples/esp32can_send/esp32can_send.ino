#include <Arduino.h>
#include <ESP32CAN.h>

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32-Arduino-CAN Send Basic Demo");

  ESP32Can.CANInit(GPIO_NUM_5, GPIO_NUM_4, ESP32_SPEED_500KBPS);
}

void loop() {
  twai_message_t tx_frame;
  
  tx_frame.extd = 0;
  tx_frame.data_length_code = 8;
  tx_frame.identifier = 0x123;

  tx_frame.data[0] = 0xDE;
  tx_frame.data[1] = 0xAD;
  tx_frame.data[2] = 0xBE;
  tx_frame.data[3] = 0xEF;
  tx_frame.data[4] = 0xBA;
  tx_frame.data[5] = 0x5E;
  tx_frame.data[6] = 0xBA;
  tx_frame.data[7] = 0x11;

  ESP32Can.CANWriteFrame(&tx_frame);

  Serial.println("CAN Frame Sent");

  delay(1000);
}
