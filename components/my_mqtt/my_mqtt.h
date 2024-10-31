/**
* @file my_mqtt.h
*
* @brief Header file for the MQTT client implementation, including initialization
*        and data publishing functions for temperature and humidity.
* 
* COPYRIGHT NOTICE: (c) 2024 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

#ifndef __MY_MQTT_H__
#define __MY_MQTT_H__

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------- INCLUDES ----------------------------------

#include "mqtt_client.h"

//---------------------------------- MACROS -----------------------------------
#define MQTT_BROKER_URI "mqtt://80.208.225.96:1883"
#define MQTT_TOPIC "/blesa/final_task"

//-------------------------------- DATA TYPES ---------------------------------

//---------------------- PUBLIC FUNCTION PROTOTYPES --------------------------
/**
 * @brief Initializes the MQTT client and connects to the broker.
 */
void mqtt_init(void);

/**
 * @brief Publishes temperature and humidity data to the MQTT broker.
 * 
 * @param temperature The temperature value to send (uint32_t).
 * @param humidity The humidity value to send (uint32_t).
 */
void send_data(uint32_t temperature, uint32_t humidity);

#ifdef __cplusplus
}
#endif

#endif // __MY_MQTT_H__
