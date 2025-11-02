# Arduino Library for the ESP32 CAN Bus (ESP32-Arduino-CAN)

## V2.0
This is a rewrite of the original library created by [miwagner](https://github.com/miwagner/ESP32-Arduino-CAN). The old version of this library used registry commands to configure the TWAI[^1] peripheral and has not been updated in years. Newer versions of the Espressif IDF have built in functions to configure the TWAI peripheral that are more reliable. V2.0 of this library incorporates these new IDF functions in the original function calls of the ESP32-Arduino-CAN library.

## Features
* Easily configure the ESP32 TWAI[^1] peripheral
* Send and receive CAN Bus messages
* Various bus speeds
* Standard (11bit) and extended (29bit) frames
* ~~CAN Message Filter~~

## Usage
See the examples in the [/examples](examples) folder.

## Enable Debug Messages
To eanble debug messages on the serial terminal set `ESP32CAN_DEBUG` to `1` using build flags. 

In platformio adding the following to `platformio.ini` will enable debug messages:
```
build_flags = 
    -D ESP32CAN_DEBUG=1
```

## Espressif IDF Documentation
The Espressif IDF documentation at the link below should be reference first if you encounter any errors. Make sure you are using the most recent IDF version, V5.0 when writing this. 

https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/twai.html#examples

## TODO
- [x] Advanced example with RTOS tasks
- [x] Release to main branch, no push to miwagner version
- [ ] Implement CAN Message Filter

[^1]: TWAI is the name that Esperssif uses for the CAN peripheral. It stands for *two wire automotive interface*