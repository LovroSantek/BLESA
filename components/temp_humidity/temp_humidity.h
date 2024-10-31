/**
* @file temp_humidity.h
*
* @brief See the source file.
* 
* COPYRIGHT NOTICE: (c) 2024 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

#ifndef __TEMP_HUMIDITY_H__
#define __TEMP_HUMIDITY_H__

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------- INCLUDES ----------------------------------
#include <stdio.h>
//---------------------------------- MACROS -----------------------------------
#define INVALID_TEMPERATURE_RESULT (99999999)
#define INVALID_HUMIDITY_RESULT    (999)

//-------------------------------- DATA TYPES ---------------------------------
typedef enum {
    TEMP_HUMIDITY_STATUS_OK,
    TEMP_HUMIDITY_STATUS_NOT_OK,
} temp_humidity_status_t;

//---------------------- PUBLIC FUNCTION PROTOTYPES --------------------------
/**
 * @brief Initializes the temperature and humidity sensor.
 * 
 * This function checks if the I2C interface is initialized. If it is not, it attempts
 * to initialize the I2C interface for communication with the temperature and humidity
 * sensor. It returns a status indicating the success or failure of the initialization.
 * 
 * @return temp_humidity_status_t Returns `TEMP_HUMIDITY_STATUS_OK` if the sensor is successfully 
 *                                initialized, or `TEMP_HUMIDITY_STATUS_NOT_OK` if initialization fails.
 */
temp_humidity_status_t temp_humidity_initialize(void);

/**
 * @brief Retrieves the current temperature from the sensor.
 * 
 * This function reads the temperature value from the temperature and humidity sensor.
 * It returns the temperature in degrees Celsius. If there is an error in reading the 
 * sensor data, an unreasonable value (e.g., 99999999) is returned to indicate failure.
 * 
 * @return uint32_t The current temperature in centi degrees Celsius (it has precision of 2 decimal points).
 *                   If the reading fails, returns 99999999 to indicate an error.
 *                   To get real teperature in °C, the return value needs to be devided by 100.
 */
uint32_t temp_humidity_get_temperature(void);

/**
 * @brief Retrieves the current humidity from the sensor.
 * 
 * This function reads the humidity value from the temperature and humidity sensor.
 * It returns the humidity as a percentage. If there is an error in reading the 
 * sensor data, an unreasonable value (e.g., -1.0) is returned to indicate failure.
 * 
 * @return uint32_t The current humidity percentage. If the reading fails, returns 999
 *               to indicate an error.
 */
uint32_t temp_humidity_get_humidity(void);


#ifdef __cplusplus
}
#endif

#endif // __TEMP_HUMIDITY_H__
