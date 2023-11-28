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
/** SHTC3 */
#include <SparkFun_SHTC3.h>
#include <Wire.h>

/** MQTT Lib */
MQTT mqtt_lib;
/** Flash Lib */
FLASH flash_lib;
/** Sleep time for logic */
uint64_t sleep_time;
/** Turn on/off debug output */
#define DEBUG 1
/** SHTC3 */
SHTC3 shtc3;

/** Forward declatration */
void load_flash();
void R_LOG(String chan, String data);
/** SHTC3 */
void check_shtc3();

/**
 * @brief Main firmware setup
 * 
 */
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
    /** Load settings from flash */
    load_flash();

    /** SHTC3 */
    Wire.begin();
    delay(500);
    if(shtc3.begin() == SHTC3_Status_Nominal)
    {
        R_LOG("SHTC3", "Found");
    } else {
        R_LOG("SHTC3", "Not found");
    }
    Wire.setClock(400000);
}

/**
 * @brief Main firmware loop
 * 
 */
void loop() 
{
    /** Loop our MQTT lib */
    mqtt_lib.mqtt_loop();
    /** Run logic every `sleep_time` */
    static uint32_t last_sch;
    if ((micros() - last_sch) >= sleep_time)
    {
        last_sch = micros();
        /** SHTC3 */
        check_shtc3();
    }
}

/**
 * @brief Check for SHTC3 readings and send via MQTT
 * 
 */
void check_shtc3()
{
    String data;
    shtc3.update();
    if(shtc3.lastStatus == SHTC3_Status_Nominal)
    {
        data = String(shtc3.toDegC()) + "+" + String(shtc3.toPercent());
        R_LOG("SHTC3", data);
        mqtt_lib.mqtt_publish(MQTT_ID, data);
    }
}

/**
 * @brief Read sensor settings from flash
 * 
 */
void load_flash()
{
    sleep_time = flash_lib.get_64u("stime", 15000000);
    R_LOG("FLASH", "Read: Sleep time " + String(sleep_time));
}

/**
 * @brief Debug serial output
 * 
 * @param chan Output channel
 * @param data String to output
 */
void R_LOG(String chan, String data)
{
  #if DEBUG
  String disp = "["+chan+"] " + data;
  Serial.println(disp);
  #endif
}
