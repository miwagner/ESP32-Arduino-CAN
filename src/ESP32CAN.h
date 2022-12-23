#ifndef INC_ESP32CAN_H
#define INC_ESP32CAN_H

#include <Arduino.h>
#include "driver/gpio.h"
#include "driver/twai.h"

/* Defines ------------------------------------------------------------------- */
#define ESP32CAN_DEBUG     /* serial print debug info */

/* Macros -------------------------------------------------------------------- */
#ifdef ESP32CAN_DEBUG
#define debugPrint(x) 	    Serial.print(x)
#define debugPrintln(x)	    Serial.println(x)
#else
#define debugPrint(x)
#define debugPrintln(x)
#endif

/* typedef ------------------------------------------------------------------- */
typedef enum {
    ESP32CAN_NOK = 0,  /* not ok, something is wrong */
    ESP32CAN_OK  = 1   /* ok, all seems well */
} ESP32CAN_status_t;

typedef enum {
    ESP32CAN_SPEED_100KBPS = 100,
    ESP32CAN_SPEED_125KBPS = 125,
    ESP32CAN_SPEED_250KBPS = 250,
    ESP32CAN_SPEED_500KBPS = 500,
    ESP32CAN_SPEED_800KBPS = 800,
    ESP32CAN_SPEED_1MBPS   = 1000,
} ESP32CAN_timing_t;

/* Globals ------------------------------------------------------------------- */

/* Function Prototypes ------------------------------------------------------- */

/* Class --------------------------------------------------------------------- */
class ESP32CAN {
    public:
        ESP32CAN_status_t CANInit(gpio_num_t tx_pin, gpio_num_t rx_pin, ESP32CAN_timing_t baud);
        ESP32CAN_status_t CANStop();
        ESP32CAN_status_t CANWriteFrame(const twai_message_t* p_frame);
        ESP32CAN_status_t CANReadFrame(twai_message_t* p_frame);

        // int CANConfigFilter(const CAN_filter_t* p_filter);

    private:

};

/* External Globals ---------------------------------------------------------- */
extern ESP32CAN ESP32Can;

#endif
