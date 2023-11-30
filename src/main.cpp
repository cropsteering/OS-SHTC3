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
  SHTC3_Status_TypeDef shtc3_status = shtc3.begin();
  if(shtc3_status == SHTC3_Status_Nominal)
  {
    Wire.setClock(400000);
    shtc3.setMode(SHTC3_CMD_CSD_TF_NPM);
    shtc3.sleep();
    R_LOG("SHTC3", "Found");
  } else {
    R_LOG("SHTC3", "Not found");
  }
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
    R_LOG("SHTC3", "Reading");
    shtc3.wake();
    shtc3.update();
    shtc3.sleep();

    if(shtc3.passIDcrc) 
    {
        /** -2 Adjustment for MCU/Board heat */
        data = String(shtc3.toDegC()-2) + "+" + String(shtc3.toPercent()-2);
        R_LOG("SHTC3", data);
        mqtt_lib.mqtt_publish(MQTT_ID, data);
    } else {
        R_LOG("SHTC3", "Failed checksum");
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
