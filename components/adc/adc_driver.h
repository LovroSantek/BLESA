/**
* @file adc_driver.h
*
* @brief See the source file.
* 
* COPYRIGHT NOTICE: (c) 2024 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

#ifndef __ADC_DRIVER_H__
#define __ADC_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------- INCLUDES ----------------------------------
#include <stdbool.h>
#include "esp_adc/adc_oneshot.h"
//---------------------------------- MACROS -----------------------------------
#define ADC_TO_USE ADC_UNIT_1

#define ADC_CALIBRATION_ENABLE  true
#define ADC_CALIBRATION_DISABLE false
//-------------------------------- DATA TYPES ---------------------------------
typedef enum
{
    ADC_INITIALIZATION_SUCCESS,
    ADC_INITIALIZATION_FAIL,
    ADC_OK,
    ADC_FAIL,
} adc_err_t;
//---------------------- PUBLIC FUNCTION PROTOTYPES --------------------------
/**
 * @brief Initialize the ADC unit.
 *
 * This function initializes the specified ADC unit and, if requested, calibrates the ADC for better accuracy.
 *
 * @param[in] adc_unit The ADC unit to initialize (e.g., ADC_UNIT_1, ADC_UNIT_2).
 * @param[in] adc_want_calibration Set to `true` if ADC calibration is desired, otherwise `false`.
 *
 * @return
 *    - `ADC_INITIALIZATION_SUCCESS`: Initialization successful.
 *    - `ADC_INITIALIZATION_FAIL`: ADC initialization failed.
 */
adc_err_t adc_initialize(adc_unit_t adc_unit, bool adc_want_calibration);

/**
 * @brief Get the voltage from a specified ADC channel using one-shot mode.
 *
 * This function reads the voltage value from the specified ADC channel in one-shot mode.
 *
 * @param[in] channel The ADC channel to sample (e.g., ADC_CHANNEL_0, ADC_CHANNEL_1).
 *
 * @return
 *    - The voltage reading in millivolts from the specified channel.
 */
uint32_t adc_oneshot_get_voltage(adc_channel_t channel);

/**
 * @brief Deinitialize the ADC unit.
 *
 * This function deinitializes the specified ADC unit, freeing any resources used by the ADC.
 *
 * @param[in] adc_unit The ADC unit to deinitialize (e.g., ADC_UNIT_1, ADC_UNIT_2).
 *
 * @return
 *    - `ADC_OK`: Deinitialization successful.
 *    - `ADC_FAIL`: ADC deinitialization failed.
 */
adc_err_t adc_deinitialize(adc_unit_t adc_unit);


#ifdef __cplusplus
}
#endif

#endif // __ADC_DRIVER_H__
