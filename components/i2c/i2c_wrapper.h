/**
* @file i2c_wrapper.h
*
* @brief See the source file.
* 
* COPYRIGHT NOTICE: (c) 2024 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

#ifndef __I2C_WRAPPER_H__
#define __I2C_WRAPPER_H__

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------- INCLUDES ----------------------------------
#include "esp_err.h"

//---------------------------------- MACROS -----------------------------------
#define I2C_MASTER_SCL_IO           (21)      /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           (22)      /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              (0)                          /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          (400000)                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   (0)                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   (0)                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       (1000)

//-------------------------------- DATA TYPES ---------------------------------
typedef enum {
    I2C_STATUS_NOT_INITIALIZED,
    I2C_STATUS_INITALIZED,
} i2c_init_status_t;

typedef enum {
    I2C_OK,
    I2C_FAIL,
} i2c_status_t;

//---------------------- PUBLIC FUNCTION PROTOTYPES --------------------------
/**
 * @brief Initializes the I2C interface if it has not already been initialized.
 * 
 * This function configures the I2C master mode, sets up the SDA and SCL pins, 
 * and installs the I2C driver. It checks the current initialization status 
 * before attempting to configure the I2C interface to prevent reinitialization.
 * 
 * @return i2c_init_status_t Returns `I2C_STATUS_INITALIZED` if the I2C interface 
 *                           is successfully initialized, or `I2C_STATUS_NOT_INITIALIZED` 
 *                           if initialization fails or if the interface was already initialized.
 */
i2c_init_status_t i2c_initialize(void);


/**
 * @brief Reads data from a specified register of an I2C peripheral.
 * 
 * This function sends a read command to the I2C device specified by the sensor address,
 * retrieves data from the specified register, and stores it in the provided data buffer.
 * 
 * @param sensor_address The I2C address of the sensor from which data is to be read.
 * @param reg_addr The register address from which to read the data. Some sensors may require this
 *                 to specify which register to read, while others may use a dummy address.
 * @param data Pointer to the buffer where the read data will be stored.
 * @param len The number of bytes to read from the sensor. This should be the size of the buffer
 *            pointed to by `data`.
 * @return i2c_status_t Status of the read operation. Returns `I2C_OK` if the read operation 
 *                      was successful, or `I2C_FAIL` if an error occurred during the operation.
 */
i2c_status_t i2c_wrapper_read(uint8_t sensor_address, uint8_t reg_addr, uint8_t *data, size_t len);

/**
 * @brief Writes data to a specified register of an I2C peripheral.
 * 
 * This function sends a write command to the I2C device specified by the sensor address,
 * along with the register address and the data to be written. It is useful for configuring 
 * the sensor or initiating a measurement.
 * 
 * @param sensor_address The I2C address of the sensor to which data is to be written.
 * @param reg_addr The register address to which the data should be written. 
 * @param data The data to write to the specified register. 
 * @return i2c_status_t Status of the write operation. Returns `I2C_OK` if the write operation 
 *                      was successful, or `I2C_FAIL` if an error occurred during the operation.
 */
i2c_status_t i2c_wrapper_write(uint8_t sensor_address, uint8_t reg_addr, uint8_t data);


#ifdef __cplusplus
}
#endif

#endif // __I2C_WRAPPER_H__
