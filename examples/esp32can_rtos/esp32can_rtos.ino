/* ESP32 Arduino CAN RTOS Example
 *     This example will send and receive messages on the CAN bus using RTOS tasks
 *
 *     When controlling a device over CAN bus that device may expect a CAN message to be received at a specific frequency.
 *     Using RTOS tasks simplifies sending dataframes and makes the timing more consistent.
 *
 *     An external transceiver is required and should be connected 
 *     to the CAN_tx and CAN_rx gpio pins specified by CANInit. Be sure
 *     to use a 3.3V compatable transceiver such as the SN65HVD23x
 *
 */

#include <Arduino.h>
#include <ESP32CAN.h>

/* RTOS priorities, higher number is more important */
#define CAN_TX_PRIORITY     3
#define CAN_RX_PRIORITY     1

#define CAN_TX_RATE_ms      500
#define CAN_RX_RATE_ms      1000

/* can frame to send */
twai_message_t tx_frame;

/* CAN RTOS callback functions */
void canSend(void *pvParameters);
void canReceive(void *pvParameters);

/* CAN RTOS task handles */
static TaskHandle_t canTxTask = NULL;
static TaskHandle_t canRxTask = NULL;

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32-Arduino-CAN RTOS Example");

  /* initialize and start, use pin 5 as CAN_tx and pin 4 as CAN_rx, CAN bus is set to 500kbps */
  ESP32Can.CANInit(GPIO_NUM_5, GPIO_NUM_4, ESP32CAN_SPEED_500KBPS);

  /* setup can send RTOS task */
  xTaskCreatePinnedToCore(canSend,         /* callback function */
                          "CAN TX",        /* name of task */
                          1024,            /* stack size (bytes in ESP32, words in FreeRTOS */
                          NULL,            /* parameter to pass to function */
                          CAN_TX_PRIORITY, /* task priority (0 to configMAX_PRIORITES - 1 */
                          &canTxTask,      /* task handle */
                          1);              /* CPU core, Arduino runs on 1 */

  /* setup can receive RTOS task */
	xTaskCreatePinnedToCore(canReceive,      /* callback function */
                          "CAN RX",        /* name of task */
                          2048,            /* stack size (bytes in ESP32, words in FreeRTOS */
                          NULL,            /* parameter to pass to function */
                          CAN_RX_PRIORITY, /* task priority (0 to configMAX_PRIORITES - 1 */
                          &canRxTask,      /* task handle */
                          1);              /* CPU core, Arduino runs on 1 */


  /* setup can send frame */
  tx_frame.extd = 0;             /* standard 11bit ID */
  tx_frame.identifier = 0x123;   /* CAN ID */
  tx_frame.data_length_code = 8; /* 8 bytes of data */
  for (int8_t i= 0; i<8; i++) {
    tx_frame.data[i] = 0xFF;     /* pad frame with 0xFF */
  }
}

void loop() {
  /* place time in seconds into first two bytes of dataframe */
  uint16_t time = millis()/1000;
  tx_frame.data[0] = highByte(time);
  tx_frame.data[1] = lowByte(time);

  delay(500);
}

/* CAN Send function called by RTOS can send task */
void canSend(void *pvParameters) {
	TickType_t xLastWakeTime; /* keep track of last time can message was sent */
	TickType_t xFrequency = CAN_TX_RATE_ms / portTICK_PERIOD_MS; /* set the transmit frequency */

  /* this task will run forever at frequency set above 
   * to stop this task from running call vTaskSuspend(canTxTask) in the main loop */
	for (;;) {
		ESP32Can.CANWriteFrame(&tx_frame);           /* send dataframe */

		vTaskDelayUntil(&xLastWakeTime, xFrequency); /* do something else until it is time to send again */
        /* the above delay function was used since it specifies an absolute wake time. 
         * Make sure the code in the forever for loop can run faster then desired send frequency or this task will take all of the CPU time available */
	}
}

void canReceive(void *pvParameters) {
	const TickType_t xDelay = CAN_RX_RATE_ms / portTICK_PERIOD_MS;
	twai_message_t rx_frame;

	for (;;) {
    if (ESP32CAN_OK == ESP32Can.CANReadFrame(&rx_frame)) {  /* only print when CAN message is received*/
      Serial.print(rx_frame.identifier, HEX);               /* print the CAN ID*/
      Serial.print(" ");
      Serial.print(rx_frame.data_length_code);              /* print number of bytes in data frame*/
      
      for (int i=0; i<rx_frame.data_length_code; i++) {     /* print the data frame*/
        Serial.print(rx_frame.data[i], HEX);
      }

      Serial.println();
    }
		vTaskDelay(xDelay); /* do something else until it is time to receive again. This is a simple delay task. */
	}
}
