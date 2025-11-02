#include "ESP32CAN.h"

ESP32CAN_status_t ESP32CAN::CANInit(gpio_num_t tx_pin, gpio_num_t rx_pin, ESP32CAN_timing_t baud) {
    /* initialize configuration structures */
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(tx_pin, rx_pin, TWAI_MODE_NORMAL);
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    twai_timing_config_t t_config;

    switch (baud) {
        case ESP32CAN_SPEED_100KBPS:
            t_config = TWAI_TIMING_CONFIG_100KBITS();
            break;
        case ESP32CAN_SPEED_125KBPS:
            t_config = TWAI_TIMING_CONFIG_125KBITS();
            break;
        case ESP32CAN_SPEED_250KBPS:
            t_config = TWAI_TIMING_CONFIG_250KBITS();
            break;
        case ESP32CAN_SPEED_500KBPS:
            t_config = TWAI_TIMING_CONFIG_500KBITS();
            break;
        case ESP32CAN_SPEED_800KBPS:
            t_config = TWAI_TIMING_CONFIG_800KBITS();
            break;
        case ESP32CAN_SPEED_1MBPS:
            t_config = TWAI_TIMING_CONFIG_1MBITS();
            break;
        default:
            debugPrintln("TWAI: undefined buad rate");
            return ESP32CAN_NOK;
            break;
    }

    /* install TWAI driver */
    switch (twai_driver_install(&g_config, &t_config, &f_config)) {
        case ESP_OK:
            debugPrintln("TWAI INSTALL: ok");
            break;
        case ESP_ERR_INVALID_ARG:
            debugPrintln("TWAI INSTALL: ESP_ERR_INVALID_ARG");
            return ESP32CAN_NOK;
            break;
        case ESP_ERR_NO_MEM:
            debugPrintln("TWAI INSTALL: ESP_ERR_NO_MEM");
            return ESP32CAN_NOK;
            break;
        case ESP_ERR_INVALID_STATE:
            debugPrintln("TWAI INSTALL: ESP_ERR_INVALID_STATE");
            return ESP32CAN_NOK;
            break;
        default:
            debugPrintln("TWAI INSTALL: uknown error");
            return ESP32CAN_NOK;
            break;
    }

    /* start TWAI driver */
    switch (twai_start()) {
        case ESP_OK:
            debugPrintln("TWAI START: ok");
            break;
        case ESP_ERR_INVALID_STATE:
            debugPrintln("TWAI START: ESP_ERR_INVALID_STATE");
            return ESP32CAN_NOK;
            break;
        default:
            debugPrintln("TWAI START: uknown error");
            return ESP32CAN_NOK;
            break;
    }

    return ESP32CAN_OK;
}

ESP32CAN_status_t ESP32CAN::CANStop() {
    /* stop the TWAI driver */
    switch (twai_stop()) {
        case ESP_OK:
            debugPrintln("TWAI STOP: ok");
            break;
        case ESP_ERR_INVALID_STATE:
            debugPrintln("TWAI STOP: ESP_ERR_INVALID_STATE");
            return ESP32CAN_NOK;
            break;
        default:
            debugPrintln("TWAI STOP: unknow error");
            return ESP32CAN_NOK;
            break;
    }

    /* uninstall TWAI driver */
    switch (twai_driver_uninstall()) {
        case ESP_OK:
            debugPrintln("TWAI UNINSTALL: ok");
            break;
        case ESP_ERR_INVALID_STATE:
            debugPrintln("TWAI UNINSTALL: ESP_ERR_INVALID_STATE");
            return ESP32CAN_NOK;
            break;
        default:
            break;
    }

    return ESP32CAN_OK;
}

ESP32CAN_status_t ESP32CAN::CANWriteFrame(const twai_message_t* p_frame) {
    /* queue message for transmission */
    switch (twai_transmit(p_frame, pdMS_TO_TICKS(10))) {
        case ESP_OK:
            break;
        case ESP_ERR_INVALID_ARG:
            debugPrintln("TWAI TX: ESP_ERR_INVALID_ARG");
            return ESP32CAN_NOK;
            break;
        case ESP_ERR_TIMEOUT:
            debugPrintln("TWAI TX: ESP_ERR_TIMEOUT");
            return ESP32CAN_NOK;
            break;
        case ESP_FAIL:
            debugPrintln("TWAI TX: ESP_FAIL");
            return ESP32CAN_NOK;
            break;
        case ESP_ERR_INVALID_STATE:
            debugPrintln("TWAI TX: ESP_ERR_INVALID_STATE");
            return ESP32CAN_NOK;
            break;
        case ESP_ERR_NOT_SUPPORTED:
            debugPrintln("TWAI TX: ESP_ERR_NOT_SUPPORTED");
            return ESP32CAN_NOK;
            break;
        default:
            debugPrintln("TWAI TX: unknow error");
            return ESP32CAN_NOK;
            break;
    }

    return ESP32CAN_OK;
}

ESP32CAN_status_t ESP32CAN::CANReadFrame(twai_message_t* p_frame) {
    switch (twai_receive(p_frame, pdMS_TO_TICKS(10))) {
    case ESP_OK:
        break;
    case ESP_ERR_TIMEOUT:
        debugPrintln("TWAI RX: ESP_ERR_TIMEOUT");
        return ESP32CAN_NOK;
        break;
    case ESP_ERR_INVALID_ARG:
        debugPrintln("TWAI RX: ESP_ERR_INVALID_ARG");
        return ESP32CAN_NOK;
        break;
    case ESP_ERR_INVALID_STATE:
        debugPrintln("TWAI RX: ESP_ERR_INVALID_STATE");
        return ESP32CAN_NOK;
        break;
    default:
        debugPrintln("TWAI RX: unknow error");
        return ESP32CAN_NOK;
        break;
    }

    return ESP32CAN_OK;
}

// int ESP32CAN::CANConfigFilter(const CAN_filter_t* p_filter)
// {
//     return CAN_config_filter(p_filter);
// }

ESP32CAN ESP32Can;
