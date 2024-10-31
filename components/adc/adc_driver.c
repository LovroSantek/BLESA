/**
* @file adc_driver.c
*
* @brief 
*
* COPYRIGHT NOTICE: (c) 2024 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

//--------------------------------- INCLUDES ----------------------------------
#include "adc_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_log.h"

//---------------------------------- MACROS -----------------------------------
#define ADC_CALIBRATION_OFFSET (130U)
//-------------------------------- DATA TYPES ---------------------------------

//---------------------- PRIVATE FUNCTION PROTOTYPES --------------------------
/**
 * @brief Initialize ADC calibration for the specified ADC unit and attenuation level.
 *
 * This function sets up the ADC calibration for improved accuracy, based on the specified ADC unit
 * and attenuation level. The calibration handle is returned via the `out_handle` parameter.
 *
 * @param[in] unit The ADC unit to calibrate (e.g., ADC_UNIT_1, ADC_UNIT_2).
 * @param[in] atten The attenuation level to use (e.g., ADC_ATTEN_DB_0, ADC_ATTEN_DB_6).
 * @param[out] out_handle Pointer to the handle that will hold the calibration context.
 *
 * @return
 *    - `true`: Calibration initialization successful.
 *    - `false`: Calibration initialization failed.
 */
static bool _adc_calibration_init(adc_unit_t unit, adc_atten_t atten, adc_cali_handle_t *out_handle);

/**
 * @brief Deinitialize ADC calibration.
 *
 * This function deinitializes the ADC calibration, freeing any resources used by the calibration context.
 *
 * @param[in] handle The handle to the ADC calibration context that was previously initialized.
 *
 * @return
 *    - `ADC_OK`: Deinitialization successful.
 *    - `ADC_ERR_CALIBRATION_DEINIT_FAILED`: Deinitialization failed.
 */
static adc_err_t _adc_calibration_deinit(adc_cali_handle_t handle);

//------------------------- STATIC DATA & CONSTANTS ---------------------------

//------------------------------- GLOBAL DATA ---------------------------------
static adc_oneshot_unit_handle_t adc1_handle = NULL;
static adc_cali_handle_t adc1_cali_handle    = NULL;

static bool adc_calibration_enabled = false;

//------------------------------ PUBLIC FUNCTIONS -----------------------------
adc_err_t adc_initialize(adc_unit_t adc_unit, bool adc_want_calibration)
{
    if(NULL == adc1_handle)
    {
        adc_oneshot_unit_init_cfg_t init_config1 = {
            .unit_id = ADC_TO_USE,
        };

        if(ESP_OK != adc_oneshot_new_unit(&init_config1, &adc1_handle))
        {
            return ADC_INITIALIZATION_FAIL;
        }
    }

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_11,
    };

    /* This is the place for adding channels to use. */
    if(ESP_OK != adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_3, &config)) // ACC_IRQ1
    {
        return ADC_INITIALIZATION_FAIL;
    }
    if(ESP_OK != adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_6, &config)) // JOY_X
    {
        return ADC_INITIALIZATION_FAIL;
    }
    if(ESP_OK != adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_7, &config)) // JOY_Y
    {
        return ADC_INITIALIZATION_FAIL;
    }

    if(true == adc_want_calibration)
    {
        adc_calibration_enabled = _adc_calibration_init(ADC_TO_USE, ADC_ATTEN_DB_11, &adc1_cali_handle);;
    }

    return ADC_INITIALIZATION_SUCCESS;
}

uint32_t adc_oneshot_get_voltage(adc_channel_t channel)
{
    int adc_raw_data = 0;
    int voltage = 0;
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, channel, &adc_raw_data));
    if(adc_calibration_enabled)
    {
        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc1_cali_handle, adc_raw_data, &voltage));
        voltage = voltage - ADC_CALIBRATION_OFFSET;
    }

    return (uint32_t)voltage;
}

adc_err_t adc_deinitialize(adc_unit_t adc_unit)
{
    if(NULL != adc1_handle)
    {
        adc_oneshot_del_unit(adc1_handle);
        if(adc_calibration_enabled)
        {
            _adc_calibration_deinit(adc1_cali_handle);
        }
    }
    return ADC_OK;
}

//---------------------------- PRIVATE FUNCTIONS ------------------------------
static bool _adc_calibration_init(adc_unit_t unit, adc_atten_t atten, adc_cali_handle_t *out_handle)
{
    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;
    bool calibrate = false;

    if (false == calibrate)
    {
        adc_cali_line_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
        if (ESP_OK == ret)
        {
            calibrate = true;
        }
    }
    *out_handle = handle;

    return calibrate;
}

static adc_err_t _adc_calibration_deinit(adc_cali_handle_t handle)
{
    if(ESP_OK != adc_cali_delete_scheme_line_fitting(handle))
    {
        return ADC_FAIL;
    }
    return ADC_OK;
}

//---------------------------- INTERRUPT HANDLERS -----------------------------


