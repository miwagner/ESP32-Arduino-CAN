/* ESP32 Arduino CAN Receive Basic Demo
 *     This example will receive messages on the CAN bus
 *
 *     An external transceiver is required and should be connected 
 *     to the CAN_tx and CAN_rx gpio pins specified by CANInit. Be sure
 *     to use a 3.3V compatable transceiver such as the SN65HVD23x
 *
 */

#include <Arduino.h>
#include <ESP32CAN.h>

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32-Arduino-CAN Receive Basic Demo");

  /* initialize and start, use pin 5 as CAN_tx and pin 4 as CAN_rx, CAN bus is set to 500kbps */
  ESP32Can.CANInit(GPIO_NUM_5, GPIO_NUM_4, ESP32CAN_SPEED_500KBPS);
}

void loop() {
  twai_message_t rx_frame;
  
  if (ESP32CAN_OK == ESP32Can.CANReadFrame(&rx_frame)) {  /* only print when CAN message is received*/
    Serial.print(rx_frame.identifier, HEX);               /* print the CAN ID*/
    Serial.print(" ");
    Serial.print(rx_frame.data_length_code);              /* print number of bytes in data frame*/
    
    for (int i=0; i<rx_frame.data_length_code; i++) {     /* print the data frame*/
      Serial.print(rx_frame.data[i], HEX);
    }

    Serial.println();
  }
  
  delay(1000);
}
