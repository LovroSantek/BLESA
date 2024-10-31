/**
* @file waveform_generator.c
*
* @brief 
*
* COPYRIGHT NOTICE: (c) 2024 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

//--------------------------------- INCLUDES ----------------------------------
#include "waveform_generator.h"
#include "led.h"

//---------------------------------- MACROS -----------------------------------
#define RESOLUTION_1_MHZ   (1000000)

#define DEFAULT_FREQUENCY  (5000U)
#define DEFAULT_AMPLITUDE  (3000U)
#define DEFAULT_DUTY_CYCLE (50U)

#define TICKS_TO_WAIT      (10U)

//-------------------------------- DATA TYPES ---------------------------------
typedef struct {
    waveform_t    waveform;
    uint32_t      frequency;
    uint32_t      amplitude_mv;
    uint32_t      duty_cycle_percentage;
    dac_channel_t dac_channel;
    TaskHandle_t  task_handle;
} waveform_generator_t;

EventGroupHandle_t event_gruop_handle[DAC_CHANNEL_MAX] = {NULL, NULL};

typedef enum {
    WAVEFORM_GENERATOR_STATE_STARTED,
    WAVEFORM_GENERATOR_STATE_STOPPED,
} waveform_generator_state_t;
//---------------------- PRIVATE FUNCTION PROTOTYPES --------------------------
/**
 * @brief Prepare values to be writen on DAC pin in the next period (calculates all data points for one signal period).
 * 
 * @param dac_channel One of two DAC channels (currently hardcoded to DAC_CHANNEL_TO_USE).
 */
static void _prepare_data(dac_channel_t dac_channel);

static void _waveform_generator_task_ch1(void *pvParameters);

/**
 * @brief Validates all four input parameters.
 * 
 * @param waveform One of four predefined waveforms
 * @param frequency Frequency od a waveform [MIN_FREQUENCY, MAX_FREQUENCY]
 * @param amplitude_mv Amplitude of a signal (maximum is 3.3V)
 * @param duty_cycle_percenatge SUpported for square wave type
 * @return esp_err_t ESP_OK if everything is ok, ESP_FAIL else
 */
esp_err_t _validate_inputs(waveform_t waveform, uint32_t frequency, uint32_t amplitude_mv, uint32_t duty_cycle_percenatge);

/**
 * @brief Initializes timer
 * 
 * @return esp_err_t ESP_OK if everything is ok, ESP_FAIL else
 */
esp_err_t _init_timer();

/**
 * @brief Generatees waveform with provided parameters.
 * 
 * @param dac_channel One of two DAC channels (currently hardcoded to DAC_CHANNEL_TO_USE).
 */
esp_err_t _genarate_waveform(dac_channel_t dac_channel);

/**
 * @brief Timer callback.
 * 
 * @param timer Timer handle
 * @return false if OK
 */
static bool IRAM_ATTR _on_timer_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data);
//------------------------- STATIC DATA & CONSTANTS ---------------------------
static uint32_t g_index = 0;
static gptimer_handle_t gptimer = NULL;

static uint32_t raw_val[POINT_ARR_LEN];   // Used to store raw values

static uint32_t output_point_number[DAC_CHANNEL_MAX];

static waveform_generator_t waveform_generator[DAC_CHANNEL_MAX] = { // set to inital (default) values
    {
        .waveform = WAVEFORM_SINE,
        .frequency = DEFAULT_FREQUENCY,
        .amplitude_mv = DEFAULT_AMPLITUDE,
        .duty_cycle_percentage = DEFAULT_DUTY_CYCLE,
        .dac_channel = DAC_CHANNEL_1,
        .task_handle = NULL,
    },
    {
        .waveform = WAVEFORM_SINE,
        .frequency = DEFAULT_FREQUENCY,
        .amplitude_mv = DEFAULT_AMPLITUDE,
        .duty_cycle_percentage = DEFAULT_DUTY_CYCLE,
        .dac_channel = DAC_CHANNEL_2,
        .task_handle = NULL,
    }
};  // channel 1 is not in use, but it can be added in the future

//------------------------------- GLOBAL DATA ---------------------------------

//------------------------------ PUBLIC FUNCTIONS -----------------------------
esp_err_t waveform_generator_init(dac_channel_t dac_channel)
{
    event_gruop_handle[DAC_CHANNEL_TO_USE] = xEventGroupCreate();
    if(NULL == event_gruop_handle[DAC_CHANNEL_TO_USE])
    {
        ESP_LOGE("WAVEFORM GENERATOR INIT: ", "Failed to create an event group!");
        return ESP_FAIL;
    }

    if(pdPASS !=  xTaskCreatePinnedToCore(_waveform_generator_task_ch1, "CHANNEL_1 WAVEFORM GENERATOR", 1024, 0, 5, 
                                            &(waveform_generator[DAC_CHANNEL_TO_USE].task_handle), 0))
    {
        ESP_LOGE("WAVEFORM GENERATOR INIT: ", "Failed to create freeRTOS task!");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t waveform_generator_set_all_parameters(dac_channel_t dac_channel, waveform_t waveform, uint32_t frequency,
                                            uint32_t amplitude_mv, uint32_t duty_cycle_percenatge)
{
    esp_err_t err = _validate_inputs(waveform, frequency, amplitude_mv, duty_cycle_percenatge);
    if (ESP_OK != err)
    {
        return err;
    }

    waveform_generator[DAC_CHANNEL_TO_USE].waveform = waveform;
    waveform_generator[DAC_CHANNEL_TO_USE].frequency = frequency;
    waveform_generator[DAC_CHANNEL_TO_USE].amplitude_mv = amplitude_mv;
    waveform_generator[DAC_CHANNEL_TO_USE].duty_cycle_percentage = duty_cycle_percenatge;

    xEventGroupSetBits(event_gruop_handle[DAC_CHANNEL_TO_USE], BIT_UPDATE);

    return ESP_OK;
}

esp_err_t waveform_generator_set_waveform(dac_channel_t dac_channel, waveform_t waveform)
{
    if (WAVEFORM_COUNT <= waveform)
    {
        ESP_LOGE("WAVEFORM GEN: ", "Invalid waveform type!");
        return ESP_FAIL;
    }
    waveform_generator[DAC_CHANNEL_TO_USE].waveform = waveform;

    xEventGroupSetBits(event_gruop_handle[DAC_CHANNEL_TO_USE], BIT_UPDATE);

    return ESP_OK;
}

esp_err_t waveform_generator_set_frequency(dac_channel_t dac_channel, uint32_t frequency)
{
    if((MIN_FREQUENCY > frequency) || (MAX_FREQUENCY < frequency))
    {
        ESP_LOGE("WAVEFORM GEN: ", "Invalid frequency!");
        return ESP_FAIL;
    }
    waveform_generator[DAC_CHANNEL_TO_USE].frequency = frequency;

    xEventGroupSetBits(event_gruop_handle[DAC_CHANNEL_TO_USE], BIT_UPDATE);

    return ESP_OK;
}

esp_err_t waveform_generator_set_amplitude_mv(dac_channel_t dac_channel, uint32_t amplitude_mv)
{
    if(VDD < amplitude_mv)
    {
        ESP_LOGE("WAVEFORM GEN: ", "Invalid amplitude!");
        return ESP_FAIL;
    }
    waveform_generator[DAC_CHANNEL_TO_USE].amplitude_mv = amplitude_mv;

    xEventGroupSetBits(event_gruop_handle[DAC_CHANNEL_TO_USE], BIT_UPDATE);

    return ESP_OK;
}

esp_err_t waveform_generator_set_duty_cycle_percenatge(dac_channel_t dac_channel, uint32_t duty_cycle_percenatge)
{
    if(100 < duty_cycle_percenatge)
    {
        ESP_LOGE("WAVEFORM GEN: ", "Invalid duty cycle percentage!");
        return ESP_FAIL;
    }
    waveform_generator[DAC_CHANNEL_TO_USE].duty_cycle_percentage = duty_cycle_percenatge;

    xEventGroupSetBits(event_gruop_handle[DAC_CHANNEL_TO_USE], BIT_UPDATE);

    return ESP_OK;
}
//---------------------------- PRIVATE FUNCTIONS ------------------------------
static void _prepare_data(dac_channel_t dac_channel)
{
    uint32_t amplitude_dac = (uint32_t)(((float)waveform_generator[DAC_CHANNEL_TO_USE].amplitude_mv / VDD) * AMP_DAC_MAX_VALUE);
    uint32_t square_duration = (uint32_t)(output_point_number[DAC_CHANNEL_TO_USE] * ((float)waveform_generator[DAC_CHANNEL_TO_USE].duty_cycle_percentage / 100));
    g_index = 0;

    for (int i = 0; i < output_point_number[DAC_CHANNEL_TO_USE]; i ++) {
        switch (waveform_generator[DAC_CHANNEL_TO_USE].waveform)
        {
            case WAVEFORM_SINE:
                raw_val[i] = (int)((sin( i * CONST_PERIOD_2_PI / output_point_number[DAC_CHANNEL_TO_USE] ) + 1) * (double)(amplitude_dac) / 2 + 0.5);
                break;
            case WAVEFORM_TRIANGLE:
                raw_val[i] = (i > (output_point_number[DAC_CHANNEL_TO_USE]  / 2)) ? (2 * amplitude_dac * (output_point_number[DAC_CHANNEL_TO_USE]  - i) / output_point_number[DAC_CHANNEL_TO_USE] ) : (2 * amplitude_dac * i / output_point_number[DAC_CHANNEL_TO_USE] );
                break;
            case WAVEFORM_SAWTOOTH:
                raw_val[i] = (i == output_point_number[DAC_CHANNEL_TO_USE] ) ? 0 : (i * amplitude_dac / output_point_number[DAC_CHANNEL_TO_USE] );
                break;
            case WAVEFORM_SQUARE:
                raw_val[i] = (i < square_duration) ? amplitude_dac : 0;
                break;
            default: break;       
        }
    }
}

static void _waveform_generator_task_ch1(void *pvParameters)
{
    waveform_generator_state_t state = WAVEFORM_GENERATOR_STATE_STOPPED;
    EventBits_t uxBits;

    esp_err_t err = _init_timer();
    if (ESP_OK != err)
    {
        ESP_LOGE("WAVEFORM GENERATOR: ", "Failed to initialize timer!");
    }
    vTaskDelay(pdMS_TO_TICKS(100));
    for(;;)
    {
        if(NULL != event_gruop_handle[DAC_CHANNEL_TO_USE])
        {
            uxBits = xEventGroupWaitBits(event_gruop_handle[DAC_CHANNEL_TO_USE], BIT_START | BIT_STOP | BIT_UPDATE, pdTRUE, pdFALSE, portMAX_DELAY);
            switch(state)
            {
                case WAVEFORM_GENERATOR_STATE_STOPPED:
                {
                    if(0 != (uxBits & BIT_START))
                    {
                        state = WAVEFORM_GENERATOR_STATE_STARTED;
                        _genarate_waveform(DAC_CHANNEL_TO_USE);
                        led_pattern_run(LED_GREEN, LED_PATTERN_FASTBLINK, 0);
                        gptimer_start(gptimer);
                    }
                    break;
                }
                case WAVEFORM_GENERATOR_STATE_STARTED:
                {
                    if(0 != (uxBits & BIT_STOP))
                    {
                        state = WAVEFORM_GENERATOR_STATE_STOPPED;
                        led_pattern_run(LED_GREEN, LED_PATTERN_KEEP_ON, 0);
                        gptimer_stop(gptimer);
                    }
                    else if(0 != (uxBits & BIT_UPDATE))
                    {
                        _genarate_waveform(DAC_CHANNEL_TO_USE);
                    }
                    break;
                }
            }
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

esp_err_t _validate_inputs(waveform_t waveform, uint32_t frequency, uint32_t amplitude_mv, uint32_t duty_cycle_percenatge)
{
    if (WAVEFORM_COUNT <= waveform)
    {
        ESP_LOGE("WAVEFORM GEN: ", "Invalid waveform type!");
        return ESP_FAIL;
    }

    if((MIN_FREQUENCY > frequency) || (MAX_FREQUENCY < frequency))
    {
        ESP_LOGE("WAVEFORM GEN: ", "Invalid frequency!");
        return ESP_FAIL;
    }

    if(VDD < amplitude_mv)
    {
        ESP_LOGE("WAVEFORM GEN: ", "Invalid amplitude!");
        return ESP_FAIL;
    }

    if(100 < duty_cycle_percenatge)
    {
        ESP_LOGE("WAVEFORM GEN: ", "Invalid duty cycle percentage!");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t _init_timer()
{
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = RESOLUTION_1_MHZ,
    };
    esp_err_t err = gptimer_new_timer(&timer_config, &gptimer);
    if(ESP_OK != err)
    {
        return err;
    }
    err = dac_output_enable(DAC_CHANNEL_TO_USE);
    if(ESP_OK != err)
    {
        return err;
    }

    gptimer_alarm_config_t alarm_config = {
        .reload_count = 0,
        .alarm_count = TIMER_INTR_US,
        .flags.auto_reload_on_alarm = true,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = _on_timer_alarm_cb,
    };

    err = gptimer_register_event_callbacks(gptimer, &cbs, raw_val);
    if(ESP_OK != err)
    {
        return err;
    }

    err = gptimer_set_alarm_action(gptimer, &alarm_config);
    if(ESP_OK != err)
    {
        return err;dac_output_voltage(DAC_CHANNEL_TO_USE, raw_val[g_index]);
    }

    err = gptimer_enable(gptimer);
    if(ESP_OK != err)
    {
        return err;
    }

    return ESP_OK;
}

esp_err_t _genarate_waveform(dac_channel_t dac_channel)
{
    output_point_number[DAC_CHANNEL_TO_USE] =  (int)(RESOLUTION_1_MHZ / (TIMER_INTR_US * waveform_generator[DAC_CHANNEL_TO_USE].frequency) + 0.5);
    if(output_point_number[DAC_CHANNEL_TO_USE] > POINT_ARR_LEN)
    {
        ESP_LOGE("WAVEFORM GENERATOR: ", "The frequency is too low!");
        return ESP_FAIL;
    }

    _prepare_data(DAC_CHANNEL_TO_USE);

    return ESP_OK;
}

//---------------------------- INTERRUPT HANDLERS -----------------------------
static bool IRAM_ATTR _on_timer_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)
{
    if (g_index >= output_point_number[DAC_CHANNEL_TO_USE]) {
        g_index = 0;
    }
    dac_output_voltage(DAC_CHANNEL_TO_USE, raw_val[g_index]);
    g_index++;

    return false;
}
