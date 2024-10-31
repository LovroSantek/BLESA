/**
* @file i2c_wrapper.c
*
* @brief 
*
* COPYRIGHT NOTICE: (c) 2024 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

//--------------------------------- INCLUDES ----------------------------------
#include "i2c_wrapper.h"
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"

//---------------------------------- MACROS -----------------------------------

//-------------------------------- DATA TYPES ---------------------------------

//---------------------- PRIVATE FUNCTION PROTOTYPES --------------------------

//------------------------- STATIC DATA & CONSTANTS ---------------------------
static i2c_init_status_t i2c_initialization_status = I2C_STATUS_NOT_INITIALIZED;

//------------------------------- GLOBAL DATA ---------------------------------

//------------------------------ PUBLIC FUNCTIONS -----------------------------
i2c_init_status_t i2c_initialize(void)
{
    if(I2C_STATUS_NOT_INITIALIZED == i2c_initialization_status)
    {
        int i2c_master_port = I2C_MASTER_NUM;

        i2c_config_t conf = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = I2C_MASTER_SDA_IO,
            .scl_io_num = I2C_MASTER_SCL_IO,
            .sda_pullup_en = GPIO_PULLUP_DISABLE,
            .scl_pullup_en = GPIO_PULLUP_DISABLE,
            .master.clk_speed = I2C_MASTER_FREQ_HZ,
        };

        i2c_param_config(i2c_master_port, &conf);
        i2c_initialization_status = I2C_STATUS_INITALIZED;
        esp_err_t err = i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
        if(ESP_OK != err)
        {
            return I2C_STATUS_NOT_INITIALIZED;
        }
    }

    return I2C_STATUS_INITALIZED;
}

i2c_status_t i2c_wrapper_read(uint8_t sensor_adress, uint8_t reg_addr, uint8_t *data, size_t len)
{
    esp_err_t err = i2c_master_write_read_device(I2C_MASTER_NUM, sensor_adress, &reg_addr, 1, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    if(ESP_OK != err)
    {
        return I2C_FAIL;
    }

    return I2C_OK;
}

i2c_status_t i2c_wrapper_write(uint8_t sensor_adress, uint8_t reg_addr, uint8_t data)
{
    esp_err_t err;
    uint8_t write_buf[2] = {reg_addr, data};

    err = i2c_master_write_to_device(I2C_MASTER_NUM, sensor_adress, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    if(ESP_OK != err)
    {
        return I2C_FAIL;
    }

    return I2C_OK;
}
//---------------------------- PRIVATE FUNCTIONS ------------------------------

//---------------------------- INTERRUPT HANDLERS -----------------------------


