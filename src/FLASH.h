/**
 * @file FLASH.h
 * @author Jamie Howse (r4wknet@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __FLASH_H__
#define __FLASH_H__

#include <map>
#include <vector>

/**
 * @brief Flash Lib
 * 
 */
class FLASH
{
    public:
    void flash_setup();
    void flash_32(const char* key, int32_t value, bool restart);
    void flash_32u(const char* key, uint32_t value, bool restart);
    void flash_64u(const char* key, uint64_t value, bool restart);
    void flash_bool(const char* key, bool value, bool restart);
    void flash_bytes(const char* key, uint8_t value[8], bool restart);
    void delete_key(String key);

    const char* MQTT_ID;
};

/** Overloads for config */
extern bool CSV;

#endif
