/**
* @file oscilloscope.h
*
* @brief See the source file.
* 
* COPYRIGHT NOTICE: (c) 2024 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

#ifndef __OSCILLOSCOPE_H__
#define __OSCILLOSCOPE_H__

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------- INCLUDES ----------------------------------
#include "ui.h"

//---------------------------------- MACROS -----------------------------------

//-------------------------------- DATA TYPES ---------------------------------
typedef enum{
    OSCILLOSCOPE_ZOOM_IN,
    OSCILLOSCOPE_ZOOM_OUT
} oscilloscope_zoom_t;
//---------------------- PUBLIC FUNCTION PROTOTYPES --------------------------
/**
 * @brief Initialize the oscilloscope module.
 *
 * This function sets up the oscilloscope by creating an ESP timer used for sampling
 * and waveform drawing at a defined sampling rate.
 */
void oscilloscope_init(void);

/**
 * @brief Start the oscilloscope's periodic sampling.
 *
 * This function starts the periodic sampling timer that collects data from the ADC
 * and triggers the waveform drawing function. Additionally, it starts a slow blink
 * LED pattern to indicate the oscilloscope is running.
 */
void oscilloscope_start(void);

/**
 * @brief Stop the oscilloscope's periodic sampling.
 *
 * This function stops the periodic sampling timer, preventing further ADC data collection.
 * It also changes the LED pattern to indicate that the oscilloscope has stopped.
 */
void oscilloscope_stop(void);

/**
 * @brief Take a screenshot of the current oscilloscope chart data.
 *
 * This function captures the waveform data from the current chart (`ui_OscilloscopeChart`)
 * and copies it into a new chart (`ui_OscilloscopeChart2`). It creates new series for the 
 * second chart and transfers the Y-axis data from the original series to the new one.
 *
 * @note This function assumes that the chart series (`ui_Chart_series1` and `ui_Chart_series2`)
 *       are already initialized.
 */
void oscilloscope_screenshot(void);

/**
 * @brief Display the captured oscilloscope screenshot.
 *
 * This function sets the Y-axis data for the chart `ui_OscilloscopeChart2` based on the
 * captured oscilloscope data stored in the screenshot series.
 */
void oscilloscope_display_screenshot(void);

void oscilloscopeCH1_zoom(oscilloscope_zoom_t zoom);

void oscilloscopeCH2_zoom(oscilloscope_zoom_t zoom);

#ifdef __cplusplus
}
#endif

#endif // __OSCILLOSCOPE_H__
