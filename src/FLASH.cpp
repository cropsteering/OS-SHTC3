/**
 * @file FLASH.cpp
 * @author Jamie Howse (r4wknet@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <Arduino.h>
#include <FLASH.h>
#include <Preferences.h>

/** Preferences instance */
Preferences flash_storage;
/** Turn on/off MQTT debug output*/
#define FLASH_DEBUG 1

/** Forward declatration */
void FLASH_LOG(String chan, String data);

void FLASH::flash_setup()
{
  /** Initialize flash storage */
  FLASH_LOG("FLASH", "Starting flash storage");
  flash_storage.begin("CROP", false);
}

/**
 * @brief Save key:value data to flash
 * 
 * @param key char
 * @param value uint32_t
 * @param flag unused
 */
void flash_32(const char* key, int32_t value, bool flag)
{
  flash_storage.putInt(key, value);
  FLASH_LOG("FLASH", "Write: " + String(key) + "/" + String(value));
}

/**
 * @brief Save key:value data to flash
 * 
 * @param key char
 * @param value uint32_t
 * @param flag unused
 */
void flash_32u(const char* key, uint32_t value, bool flag)
{
  flash_storage.putUInt(key, value);
  FLASH_LOG("FLASH", "Write: " + String(key) + "/" + String(value));
}

/**
 * @brief Save key:value data to flash
 * 
 * @param key char
 * @param value uint64_t
 * @param flag unused
 */
void flash_64u(const char* key, uint64_t value, bool flag)
{
  flash_storage.putULong64(key, value);
  FLASH_LOG("FLASH", "Write: " + String(key) + "/" + String(value));
}

/**
 * @brief Save key:value data to flash
 * 
 * @param key char
 * @param value bool
 * @param flag unused
 */
void flash_bool(const char* key, bool value, bool flag)
{
  flash_storage.putBool(key, value);
  FLASH_LOG("FLASH", "Write: " + String(key) + "/" + String(value));
}

/**
 * @brief Save key:value data to flash
 * 
 * @param key char
 * @param value bool
 * @param flag unused
 */
void flash_bytes(const char* key, uint8_t value[8], bool flag)
{
  flash_storage.putBytes(key, value, 8);
  FLASH_LOG("FLASH", "Write: " + String(key));
}

/**
 * @brief Delete key from flash
 * 
 * @param key to delete
 */
void delete_key(String key)
{
  flash_storage.remove(key.c_str());
  FLASH_LOG("FLASH", "Delete: " + String(key));
}

/**
 * @brief 
 * 
 * @param chan Output channel
 * @param data String to output
 */
void FLASH_LOG(String chan, String data)
{
  #if FLASH_DEBUG
  String disp = "["+chan+"] " + data;
  Serial.println(disp);
  #endif
}
