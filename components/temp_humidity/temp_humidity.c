/**
* @file temp_humidity.c
*
* @brief 
*
* COPYRIGHT NOTICE: (c) 2024 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

//--------------------------------- INCLUDES ----------------------------------
#include "temp_humidity.h"
#include "i2c_wrapper.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

//---------------------------------- MACROS -----------------------------------
#define TEMP_HUMIDITY_ADDRESS       (0x44)
#define SENSOR_CMD_HIGH_REP_STRETCH (0x2C06) // High repeatability, clock stretching enabled
//-------------------------------- DATA TYPES ---------------------------------

//---------------------- PRIVATE FUNCTION PROTOTYPES --------------------------
/**
 * @brief Reads raw sensor data for temperature and humidity.
 * 
 * This function sends a command to the temperature and humidity sensor to initiate 
 * a measurement and then reads the resulting raw data from the sensor. The function 
 * handles the communication via I2C, including sending the command and reading the 
 * expected number of bytes. The raw data is stored in the provided buffer.
 * 
 * @param data Pointer to a buffer where the raw sensor data will be stored. This buffer
 *             should be large enough to hold the expected data (e.g., 6 bytes).
 * @param len The number of bytes to read from the sensor. This should correspond to the
 *            size of the buffer pointed to by `data`.
 * @return temp_humidity_status_t Returns `TEMP_HUMIDITY_STATUS_OK` if the read operation 
 *                                 is successful, or `TEMP_HUMIDITY_STATUS_NOT_OK` if there 
 *                                 is an error in communication with the sensor.
 */
static temp_humidity_status_t _read_sensor_raw(uint8_t *data, size_t len);

/**
 * @brief Converts raw temperature value from the sensor to Celsius.
 * 
 * This function takes a raw temperature value as input and converts it
 * to a temperature in degrees Celsius using a specific conversion formula
 * defined in the sensor's datasheet. The conversion is based on the
 * sensor's output range, which is typically from -45°C to +130°C.
 * 
 * @param raw_value The raw temperature value obtained from the sensor,
 *                  represented as a 16-bit unsigned integer.
 * @return uint32_t The temperature in centi degrees Celsius.
 */
static uint32_t _convert_to_centi_celsius(uint16_t raw_value);

/**
 * @brief Converts raw humidity value from the sensor to percentage.
 * 
 * This function takes a raw humidity value as input and converts it
 * to a percentage using a specific conversion formula defined in the
 * sensor's datasheet. The conversion assumes that the raw value is 
 * represented as a 16-bit unsigned integer, with the output range 
 * from 0% to 100%.
 * 
 * @param raw_value The raw humidity value obtained from the sensor,
 *                  represented as a 16-bit unsigned integer.
 * @return uint32_t The humidity as a percentage, ranging from 0% to 100%.
 */
static uint32_t _convert_to_percentage(uint16_t raw_value);

//------------------------- STATIC DATA & CONSTANTS ---------------------------

//------------------------------- GLOBAL DATA ---------------------------------

//------------------------------ PUBLIC FUNCTIONS -----------------------------
temp_humidity_status_t temp_humidity_initialize()
{
    if(I2C_STATUS_NOT_INITIALIZED == i2c_initialize())
    {
        return TEMP_HUMIDITY_STATUS_NOT_OK;
    }

    return TEMP_HUMIDITY_STATUS_OK;
}

uint32_t temp_humidity_get_temperature()
{
    uint8_t data[6];
    temp_humidity_status_t err = _read_sensor_raw(data, sizeof(data));
    
    if(TEMP_HUMIDITY_STATUS_OK != err)
    {
        return INVALID_TEMPERATURE_RESULT; // Unreasonable value if something went wrong
    }

    uint16_t temp_raw = (data[0] << 8) | data[1];
    uint32_t temperature = _convert_to_centi_celsius(temp_raw);

    return temperature;
}

uint32_t temp_humidity_get_humidity()
{
    uint8_t data[6]; 
    temp_humidity_status_t err = _read_sensor_raw(data, sizeof(data));
    
    if(TEMP_HUMIDITY_STATUS_OK != err)
    {
        return INVALID_HUMIDITY_RESULT; // Unreasonable value if something went wrong
    }

    uint16_t hum_raw = (data[3] << 8) | data[4];
    uint32_t humidity = _convert_to_percentage(hum_raw);

    return humidity;
}

//---------------------------- PRIVATE FUNCTIONS ------------------------------
static temp_humidity_status_t _read_sensor_raw(uint8_t *data, size_t len)
{
    uint8_t cmd_msb = (SENSOR_CMD_HIGH_REP_STRETCH >> 8);
    uint8_t cmd_lsb = (SENSOR_CMD_HIGH_REP_STRETCH & 0xFF);
    
    // Send command to the sensor
    i2c_status_t err = i2c_wrapper_write(TEMP_HUMIDITY_ADDRESS, cmd_msb, cmd_lsb);
    if(I2C_OK != err)
    {
        ESP_LOGE("SENSOR", "Failed to send measurement command, error: %d", err);
        return TEMP_HUMIDITY_STATUS_NOT_OK;
    }

    vTaskDelay(pdMS_TO_TICKS(100));  // Time for sensor to take a measurement

    // Read 6 bytes: temperature (2 bytes), CRC (1 byte), humidity (2 bytes), CRC (1 byte)
    err = i2c_wrapper_read(TEMP_HUMIDITY_ADDRESS, 0x00, data, len);
    if(I2C_OK != err)
    {
        ESP_LOGE("SENSOR", "Failed to read bytes, error: %d", err);
        return TEMP_HUMIDITY_STATUS_NOT_OK;
    }
    
    return TEMP_HUMIDITY_STATUS_OK;
}

static uint32_t _convert_to_centi_celsius(uint16_t raw_value)
{
    return (uint32_t)(100.0f * (-46.0f + 175.0f * ((float)raw_value / 65535.0f)));
}

static uint32_t _convert_to_percentage(uint16_t raw_value)
{
    return (uint32_t)(100.0f * ((float)raw_value / 65535.0f));
}

//---------------------------- INTERRUPT HANDLERS -----------------------------
