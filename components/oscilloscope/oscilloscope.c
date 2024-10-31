/**
* @file oscilloscope.c
*
* @brief 
*
* COPYRIGHT NOTICE: (c) 2024 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

//--------------------------------- INCLUDES ----------------------------------
#include "oscilloscope.h"
#include "esp_timer.h"
#include "adc_driver.h"
#include "led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdbool.h>

//---------------------------------- MACROS -----------------------------------
#define POINTS_PER_FRAME (200U)
#define DEFAULT_SAMPLING_RATE_CH1 (1000U)  // 1000 us = 1 ms
#define DEFAULT_SAMPLING_RATE_CH2 (100U)   // 100 us = 0.1 ms
#define OSCILLOSCOPE_SCREEN_REFRESH_RATE (1500000U)  // = 0.67 Hz
#define OSCILLOSCOPE_ZOOM_INCREMENT (100U)
#define OSCILLOSCOPE_MINIMUM_SAMPLING_RATE (20U)
#define OSCILLOSCOPE_RATE_THRESHOLD (110U)

#define OSCILLOSCOPE_MAX_VOLTAGE (3300U)
#define OSCILLOSCOPE_MIN_VOLTAGE (0U)
//-------------------------------- DATA TYPES ---------------------------------
typedef struct oscilloscope
{
    lv_coord_t measurement_data[POINTS_PER_FRAME];
    lv_chart_series_t *ui_Chart_series;
    uint32_t index;
    esp_timer_handle_t sample_timer;
    uint32_t sampling_rate;
    uint32_t min_voltage;
    uint32_t max_voltage;
} oscilloscope_channel_t;

//---------------------- PRIVATE FUNCTION PROTOTYPES --------------------------
/**
 * @brief Timer callback for sampling channel 1 ADC data.
 *
 * This function is called periodically to sample voltage from ADC channel 1.
 * It stores the sampled voltage into the measurement data array, updates the
 * maximum and minimum voltage values, and stops the timer when the buffer is full.
 *
 * @param[in] arg Unused argument passed by the timer.
 */
void _oscilloscope_sample_timer_callback_ch1(void* arg);

/**
 * @brief Timer callback for sampling channel 2 ADC data.
 *
 * This function is called periodically to sample voltage from ADC channel 2.
 * It stores the sampled voltage into the measurement data array, updates the
 * maximum and minimum voltage values, and stops the timer when the buffer is full.
 *
 * @param[in] arg Unused argument passed by the timer.
 */
void _oscilloscope_sample_timer_callback_ch2(void* arg);

/**
 * @brief Timer callback to notify the oscilloscope screen task.
 *
 * This function is used to stop the screen refresh timer and notify the oscilloscope
 * drawing task to update the waveform on the screen.
 *
 * @param[in] arg Unused argument passed by the timer.
 */
void _oscilloscope_timer_for_screen(void* arg);

/**
 * @brief Draws the waveform for both channels on the oscilloscope.
 *
 * This function updates the LVGL chart by setting the external Y array
 * for both channel 1 and channel 2 measurement data. It also updates
 * the displayed peak-to-peak voltage for both channels.
 */
void _draw_waveform(void);

/**
 * @brief Task for drawing the waveform periodically.
 *
 * This task waits for a notification, then draws the waveform, resets
 * the min/max voltage values, and restarts the sampling timers for both
 * channels. It also periodically starts the screen refresh timer.
 *
 * @param[in] pvParameters Unused parameter for task creation.
 */
void _draw_waveform_task(void * pvParameters);

/**
 * @brief Resets the maximum and minimum voltage values for both channels.
 *
 * This function resets the maximum and minimum voltage values to their
 * initial states for both channel 1 and channel 2.
 */
void _reset_max_and_min_voltage(void);

//------------------------- STATIC DATA & CONSTANTS ---------------------------
static bool started = false;
static oscilloscope_channel_t channel1 = {.measurement_data = {}, .index = 0, .sampling_rate = DEFAULT_SAMPLING_RATE_CH1,
                                          .min_voltage = OSCILLOSCOPE_MAX_VOLTAGE, .max_voltage = OSCILLOSCOPE_MIN_VOLTAGE};
static oscilloscope_channel_t channel2 = {.measurement_data = {}, .index = 0, .sampling_rate = DEFAULT_SAMPLING_RATE_CH2,
                                          .min_voltage = OSCILLOSCOPE_MAX_VOLTAGE, .max_voltage = OSCILLOSCOPE_MIN_VOLTAGE};

static esp_timer_handle_t oscilloscope_screen_timer;

TaskHandle_t oscilloscope_draw_task_handle = NULL;

//------------------------------- GLOBAL DATA ---------------------------------
lv_chart_series_t *screenshot_series1;
lv_chart_series_t *screenshot_series2;
//------------------------------ PUBLIC FUNCTIONS -----------------------------
void oscilloscope_init()
{
    /* Create ESP timer for measurements of channel 1. */
    esp_timer_create_args_t timer_args1_ch1 = {
    .callback = &_oscilloscope_sample_timer_callback_ch1,
    .arg = NULL,
    .name = "Oscilloscope sample timer ch1"
    };

    /* Create ESP timer for measurements of channel 2. */
    esp_timer_create_args_t timer_args1_ch2 = {
    .callback = &_oscilloscope_sample_timer_callback_ch2,
    .arg = NULL,
    .name = "Oscilloscope sample timer ch2"
    };

    /* Create ESP timer for screen. */
    esp_timer_create_args_t timer_args_screen = {
    .callback = &_oscilloscope_timer_for_screen,
    .arg = NULL,
    .name = "Oscilloscope sample timer"
    };

    if(pdTRUE != xTaskCreate(_draw_waveform_task, "Draw waveform", 2 * 1024, (void*)0, 5, &oscilloscope_draw_task_handle))
    {
        return;
    }

    esp_timer_create(&timer_args1_ch1, &channel1.sample_timer);
    esp_timer_create(&timer_args1_ch2, &channel2.sample_timer);
    esp_timer_create(&timer_args_screen, &oscilloscope_screen_timer);
}

void oscilloscope_start(void)
{
    if(!started)
    {
        channel1.ui_Chart_series = lv_chart_add_series(ui_OscilloscopeChart, lv_color_hex(0x20F080), LV_CHART_AXIS_PRIMARY_Y);
        channel2.ui_Chart_series = lv_chart_add_series(ui_OscilloscopeChart, lv_color_hex(0xFFF800), LV_CHART_AXIS_PRIMARY_Y);

        screenshot_series1 = lv_chart_add_series(ui_OscilloscopeChart2, lv_color_hex(0x20F080), LV_CHART_AXIS_PRIMARY_Y);
        screenshot_series2 = lv_chart_add_series(ui_OscilloscopeChart2, lv_color_hex(0xFFF800), LV_CHART_AXIS_PRIMARY_Y);
        
        started = true;
    }
        /* Start all timers. */
        esp_timer_start_periodic(channel1.sample_timer, channel1.sampling_rate);
        esp_timer_start_periodic(channel1.sample_timer, channel2.sampling_rate);
        esp_timer_start_periodic(oscilloscope_screen_timer, OSCILLOSCOPE_SCREEN_REFRESH_RATE);
        led_pattern_run(LED_GREEN, LED_PATTERN_SLOWBLINK, 0);

        lv_label_set_text_fmt(ui_CH1msdiv_label, "%d ms/div", (int)(channel1.sampling_rate / 20.0f));
        lv_label_set_text_fmt(ui_CH2msdiv_label, "%d ms/div", (int)(channel2.sampling_rate / 20.0f));
}

void oscilloscope_stop(void)
{
    /* Stop all timers. */
    esp_timer_stop(channel1.sample_timer);
    esp_timer_stop(channel2.sample_timer);
    esp_timer_stop(oscilloscope_screen_timer);
    led_pattern_run(LED_GREEN, LED_PATTERN_KEEP_ON, 0);
}

void oscilloscope_screenshot(void)
{
    // Ensure that both chart series are initialized
    if (channel1.ui_Chart_series == NULL || channel2.ui_Chart_series == NULL) {
        return; // Exit if the series are not initialized
    }

    // Copy the y-values from the original chart series to the new series
    for (uint16_t i = 0; i < POINTS_PER_FRAME; i++) {
        screenshot_series1->y_points[i] = channel1.ui_Chart_series->y_points[i];
        screenshot_series2->y_points[i] = channel2.ui_Chart_series->y_points[i];
    }
}

void oscilloscope_display_screenshot(void)
{
    lv_chart_set_ext_y_array(ui_OscilloscopeChart2, screenshot_series1, channel1.measurement_data);
    lv_chart_set_ext_y_array(ui_OscilloscopeChart2, screenshot_series2, channel2.measurement_data);
}

void oscilloscopeCH1_zoom(oscilloscope_zoom_t zoom)
{
    switch(zoom)
    {
    case OSCILLOSCOPE_ZOOM_IN:
    if(channel1.sampling_rate > OSCILLOSCOPE_MINIMUM_SAMPLING_RATE)
    {
        if(channel1.sampling_rate <= OSCILLOSCOPE_RATE_THRESHOLD)
        {
            channel1.sampling_rate -= OSCILLOSCOPE_ZOOM_INCREMENT / 10;
            break;
        }
        channel1.sampling_rate -= OSCILLOSCOPE_ZOOM_INCREMENT;
    }
    break;
    case OSCILLOSCOPE_ZOOM_OUT:
        if(channel1.sampling_rate < OSCILLOSCOPE_RATE_THRESHOLD)
        {
            channel1.sampling_rate += OSCILLOSCOPE_ZOOM_INCREMENT / 10;
            break;
        }
        channel1.sampling_rate += OSCILLOSCOPE_ZOOM_INCREMENT;
    break;
    default: break;
    }

    lv_label_set_text_fmt(ui_CH1msdiv_label, "%d ms/div", (int)(channel1.sampling_rate / 20.0f));
}

void oscilloscopeCH2_zoom(oscilloscope_zoom_t zoom)
{
    switch(zoom)
    {
    case OSCILLOSCOPE_ZOOM_IN:
    if(channel2.sampling_rate > OSCILLOSCOPE_MINIMUM_SAMPLING_RATE)
    {
        if(channel2.sampling_rate <= OSCILLOSCOPE_RATE_THRESHOLD)
        {
            channel2.sampling_rate -= OSCILLOSCOPE_ZOOM_INCREMENT / 10;
            break;
        }
        channel2.sampling_rate -= OSCILLOSCOPE_ZOOM_INCREMENT;
    }
    break;
    case OSCILLOSCOPE_ZOOM_OUT:
        if(channel2.sampling_rate < OSCILLOSCOPE_RATE_THRESHOLD)
        {
            channel2.sampling_rate += OSCILLOSCOPE_ZOOM_INCREMENT / 10;
            break;
        }
        channel2.sampling_rate += OSCILLOSCOPE_ZOOM_INCREMENT;
    break;
    default: break;
    }
    lv_label_set_text_fmt(ui_CH2msdiv_label, "%d ms/div", (int)(channel2.sampling_rate / 20.0f));
}

//---------------------------- PRIVATE FUNCTIONS ------------------------------
void _oscilloscope_sample_timer_callback_ch1(void* arg)
{
    uint32_t voltage = adc_oneshot_get_voltage(ADC_CHANNEL_3);
    channel1.measurement_data[channel1.index++] = voltage;

    if(voltage > channel1.max_voltage)
    {
        channel1.max_voltage = voltage;
    }

    if(voltage < channel1.min_voltage)
    {
        channel1.min_voltage = voltage;
    }

    if(POINTS_PER_FRAME == channel1.index)
    {
        channel1.index = 0;
        esp_timer_stop(channel1.sample_timer);
    }
}

void _oscilloscope_sample_timer_callback_ch2(void* arg)
{
    uint32_t voltage = adc_oneshot_get_voltage(ADC_CHANNEL_6);
    channel2.measurement_data[channel2.index++] = voltage;

    if(voltage > channel2.max_voltage)
    {
        channel2.max_voltage = voltage;
    }

    if(voltage < channel2.min_voltage)
    {
        channel2.min_voltage = voltage;
    }

    if(POINTS_PER_FRAME == channel2.index)
    {
        channel2.index = 0;
        esp_timer_stop(channel2.sample_timer);
    }
}

void _oscilloscope_timer_for_screen(void* arg)
{
    esp_timer_stop(oscilloscope_screen_timer);
    vTaskNotifyGiveFromISR(oscilloscope_draw_task_handle, NULL);
}

void _draw_waveform(void)
{
    /* Draw channel 1. */
    lv_chart_set_ext_y_array(ui_OscilloscopeChart, channel1.ui_Chart_series, channel1.measurement_data);

    /* Draw channel 2. */
    lv_chart_set_ext_y_array(ui_OscilloscopeChart, channel2.ui_Chart_series, channel2.measurement_data);

    /* Refresh Vpp values. */
    lv_label_set_text_fmt(ui_Ch1Vpp, "CH1: %d mVpp", (int)(channel1.max_voltage - channel1.min_voltage));
    lv_label_set_text_fmt(ui_Ch2Vpp, "CH2: %d mVpp", (int)(channel2.max_voltage - channel2.min_voltage));

    lv_chart_refresh(ui_OscilloscopeChart);
}

void _draw_waveform_task(void * pvParameters)
{
    for(;;)
    {
        xTaskNotifyWait(0x00, ULONG_MAX, NULL, portMAX_DELAY);
        _draw_waveform();

        _reset_max_and_min_voltage();

        esp_timer_start_periodic(channel1.sample_timer, channel1.sampling_rate);
        esp_timer_start_periodic(channel2.sample_timer, channel2.sampling_rate);
        esp_timer_start_periodic(oscilloscope_screen_timer, OSCILLOSCOPE_SCREEN_REFRESH_RATE);
    }
}

void _reset_max_and_min_voltage(void)
{
    channel1.max_voltage = OSCILLOSCOPE_MIN_VOLTAGE;
    channel1.min_voltage = OSCILLOSCOPE_MAX_VOLTAGE;

    channel2.max_voltage = OSCILLOSCOPE_MIN_VOLTAGE;
    channel2.min_voltage = OSCILLOSCOPE_MAX_VOLTAGE;
}

//---------------------------- INTERRUPT HANDLERS -----------------------------


