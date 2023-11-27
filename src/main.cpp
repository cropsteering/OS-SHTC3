/**
 * @file main.cpp
 * @author Jamie Howse (r4wknet@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <Arduino.h>
#include <MQTT.h>
#include <FLASH.h>

/** MQTT Lib */
MQTT mqtt_lib;
/** Flash Lib */
FLASH flash_lib;
/** Check frequency for schdule */
uint64_t sch_time = 200000;

void setup() 
{
    /** Setup WisBlock pins/io slots and serial */
    pinMode(WB_IO2, OUTPUT);
    digitalWrite(WB_IO2, HIGH); 
    delay(500);

    time_t timeout = millis();
    Serial.begin(115200);
    while (!Serial)
    {
        if ((millis() - timeout) < 5000)
        {
            delay(100);
        } else {
            break;
        }
    }

    /** Join WiFi and connect to MQTT */
    mqtt_lib.mqtt_setup();
    /** Setup Flash storage */
    flash_lib.flash_setup();
}

void loop() 
{
    /** Loop our MQTT lib */
    mqtt_lib.mqtt_loop();
    static uint32_t last_sch;
    if ((micros() - last_sch) >= sch_time)
    {
        last_sch = micros();
    }
}
