/**
* @file waveform_generator.h
*
* @brief See the source file.
* 
* COPYRIGHT NOTICE: (c) 2024 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

#ifndef __WAVEFORM_GENERATOR_H__
#define __WAVEFORM_GENERATOR_H__

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------- INCLUDES ----------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"
#include "driver/dac.h"
#include "driver/gptimer.h"
#include "esp_log.h"
#include "esp_err.h"
//---------------------------------- MACROS -----------------------------------
#define TIMER_INTR_US      (8U)                               // Execution time of each ISR interval in micro-seconds
#define POINT_ARR_LEN      (200U)                             // Length of points array
#define VDD                (3300U)                            // VDD is 3.3V, 3300mV
#define CONST_PERIOD_2_PI  (6.2832)
#define AMP_DAC_MAX_VALUE  (255U)                             // Amplitude of DAC voltage. If it's more than 256 will causes dac_output_voltage() output 0.

#define MIN_FREQUENCY      (1000U)  //these values have to be tested
#define MAX_FREQUENCY      (10000U) 

#define BIT_START          (1 << 0)
#define BIT_STOP           (1 << 1)
#define BIT_UPDATE         (1 << 3)

#define DAC_CHANNEL_TO_USE  DAC_CHANNEL_1 // DAC_CHANNEL_2 is connected to BTN_4 and DAC_CHANNEL_2 to LED_RED
                                          // regardless of that there is still support for adding DAC_CHANNEL_2
//-------------------------------- DATA TYPES ---------------------------------

typedef enum {
    WAVEFORM_SINE,
    WAVEFORM_TRIANGLE,
    WAVEFORM_SAWTOOTH,
    WAVEFORM_SQUARE,

    WAVEFORM_COUNT
} waveform_t;

extern EventGroupHandle_t event_gruop_handle[DAC_CHANNEL_MAX];
//---------------------- PUBLIC FUNCTION PROTOTYPES --------------------------
/**
 * @brief Initializes waveform generators.
 * 
 * @return esp_err_t ESP_OK if everything is ok, ESP_FAIL else
 */
esp_err_t waveform_generator_init(dac_channel_t dac_channel);

/**
 * @brief Sets parameters on given DAC channel.
 * 
 * @param dac_channel Choose DAC channel to update parameters (currently, only DAC_CHANNEL_1 is supported but it is open for an easy upgrade).
 * @param waveform One of four predefined waveforms
 * @param frequency Frequency od a waveform [MIN_FREQUENCY, MAX_FREQUENCY]
 * @param amplitude_mv Amplitude of a signal (maximum is 3.3V)
 * @param duty_cycle_percenatge SUpported for square wave type
 * @return esp_err_t ESP_OK if everything is ok, ESP_FAIL else
 */
esp_err_t waveform_generator_set_all_parameters(dac_channel_t dac_channel,
                                                waveform_t    waveform,
                                                uint32_t      frequency,
                                                uint32_t      amplitude_mv,
                                                uint32_t      duty_cycle_percenatge);

/**
 * @brief Sets waveform generator waveform and updates the output if input is valid.
 * 
 * @param dac_channel Channel to update
 * @param waveform waveform that needs to be applied
 * @return esp_err_t ESP_OK is everything is ok, ESP_FAIL else
 */
esp_err_t waveform_generator_set_waveform(dac_channel_t dac_channel, waveform_t waveform);

/**
 * @brief Sets waveform generator frequency and updates the output if input is valid.
 * 
 * @param dac_channel Channel to update
 * @param frequency frequency that needs to be applied
 * @return esp_err_t ESP_OK is everything is ok, ESP_FAIL else
 */
esp_err_t waveform_generator_set_frequency(dac_channel_t dac_channel, uint32_t frequency);

/**
 * @brief Sets waveform generator amplitude and updates the output if input is valid.
 * 
 * @param dac_channel Channel to update
 * @param amplitude_mv amplitude that needs to be applied
 * @return esp_err_t ESP_OK is everything is ok, ESP_FAIL else
 */
esp_err_t waveform_generator_set_amplitude_mv(dac_channel_t dac_channel, uint32_t amplitude_mv);

/**
 * @brief Sets waveform generator duty cycle and updates the output if input is valid.
 * 
 * @param dac_channel Channel to update
 * @param duty_cycle_percenatge duty cacle that needs to be applied
 * @return esp_err_t ESP_OK is everything is ok, ESP_FAIL else
 */
esp_err_t waveform_generator_set_duty_cycle_percenatge(dac_channel_t dac_channel, uint32_t duty_cycle_percenatge);


#ifdef __cplusplus
}
#endif

#endif // __WAVEFORM_GENERATOR_H__
