/**
* @file user_interface.c
*
* @brief 
*
* COPYRIGHT NOTICE: (c) 2024 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

//--------------------------------- INCLUDES ----------------------------------
#include <stdbool.h>
#include "user_interface.h"
#include "waveform_generator.h"
#include "temp_humidity.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "blesa_wifi.h"
#include "ui_app.h"
#include "ui.h"
#include "adc_driver.h"
#include "oscilloscope.h"
#include "my_mqtt.h"

#include "esp_log.h"

//---------------------------------- MACROS -----------------------------------
#define EVENT_MANAGER_QUEUE_LENGTH (20U)

#define RUN_INDEFINETLY            (0U)

#define KEY_UP    (1U)
#define KEY_RIGHT (2U)
#define KEY_DOWN  (3U)
#define KEY_LEFT  (4U)

#define TEMP_AND_HUMIDITY_HISTORY_LENGTH (60U)

//-------------------------------- DATA TYPES ---------------------------------

//---------------------- PRIVATE FUNCTION PROTOTYPES --------------------------
/**
 * @brief Initializes red, green and blue LED.
 * 
 */
static user_interface_error_t _leds_init(void);

/**
 * @brief Initializes all 4 buttons.
 * 
 */
static user_interface_error_t _buttons_init(void);

/**
 * @brief Performs action after debouncing button.
 * 
 */
void _button_1_callback();

/**
 * @brief Performs action after debouncing button.
 * 
 */
void _button_2_callback();

/**
 * @brief Performs action after debouncing button.
 * 
 */
void _button_3_callback();

/**
 * @brief Performs action after debouncing button.
 * 
 */
void _button_4_callback();

/**
 * @brief Event manager task that continuously monitors the event queue for incoming events.
 *
 * This function waits indefinitely (`portMAX_DELAY`) for events to be received from the `event_manager_queue`. 
 * Upon receiving an event, it calls the `_perform_action` function
 * to handle the specific event based on the data provided.
 *
 * @param[in] pvParameters Pointer to task parameters (not used in this function).
 */
void _event_manager(void *pvParameters);

/**
 * @brief Performs an action based on the received event.
 *
 * This function processes the event data contained in the `event_manager_queue_item` structure. 
 * It executes specific actions depending on the type of event, such as starting or stopping 
 * a DAC operation, running an LED pattern, or handling other defined events.
 *
 * @param[in] recived_data The data received from the event queue, containing the event type and any additional information.
 */
static void _perform_action(event_manager_queue_item recived_data);

/**
 * @brief Starts freeRTOS timer to measure temperature and humidity every 5 seconds.
 * 
 * @return user_interface_error_t USER_INTERFACE_OK is everything is ok.
 */
static user_interface_error_t _start_temp_humidity_timer(void);

/**
 * @brief Timer callback which measures temperature and humidity from sensor.
 * 
 * @param timer Timer that triggered function.
 */
static void _measure_temp_humidity(TimerHandle_t timer);

/**
 * @brief Sends event to event_manager_queue depending on Wi.Fi status.
 * 
 * @param new_status new Wi-Fi satatus (connected, disconnected...)
 */
void _wifi_status_callback(wifi_connection_status_t new_status);

/**
 * @brief Stores temperature data to lv_coord_t array. It is later udes to keep track
 * of historical temperature values.
 * 
 * @param temperature Temperature to store.
 */
void _store_temperature_data(uint32_t temperature);

/**
 * @brief Stores humidity data to lv_coord_t array. It is later udes to keep track
 * of historical humidity values.
 * 
 * @param humidity Humidity to store.
 */
void _store_humidity_data(uint32_t humidity);

//------------------------- STATIC DATA & CONSTANTS ---------------------------
static TaskHandle_t  event_manager_handle = NULL;
static TimerHandle_t temp_humididty_tiemr = NULL;
static QueueHandle_t event_manager_queue  = NULL;

/* Objects for storing temperature and humidity data. */
static lv_chart_series_t * ui_temp_series;
static lv_chart_series_t * ui_humidity_series;
static lv_coord_t temperature_data[TEMP_AND_HUMIDITY_HISTORY_LENGTH] = {};
static lv_coord_t humidity_data[TEMP_AND_HUMIDITY_HISTORY_LENGTH] = {};
static uint32_t temp_hum_index = 0;

//------------------------------- GLOBAL DATA ---------------------------------

//------------------------------ PUBLIC FUNCTIONS -----------------------------
user_interface_error_t user_interface_init(void)
{
    user_interface_error_t err = USER_INTERFACE_OK;
    err = _leds_init();
    if (USER_INTERFACE_OK != err)
    {
        return err;
    }
    ESP_LOGI("UI: ", "Green and blue LEDs initialized succesfully!");

    err = _buttons_init();
    if (USER_INTERFACE_OK != err)
    {
        return err;
    }
    ESP_LOGI("UI: ", "Buttons initialized succesfully!");

    if (ADC_INITIALIZATION_SUCCESS != adc_initialize(ADC_TO_USE, ADC_CALIBRATION_ENABLE))
    {
        ESP_LOGE("UI: ", "ADC initialization failed!");
    }
    ESP_LOGI("UI: ", "ADC initialized successfully.");

    if(pdPASS != xTaskCreate(_event_manager, "Event manager", 2 * 1024, 0, 5, &event_manager_handle))
    {
        return USER_INTERFACE_INIT_FAIL;
    }

    event_manager_queue = xQueueCreate(EVENT_MANAGER_QUEUE_LENGTH, sizeof(event_manager_queue_item));
    if(NULL == event_manager_queue)
    {
        return USER_INTERFACE_INIT_FAIL;
    }
    ESP_LOGI("UI: ", "Event manager task and queue created succesfully.");

    /* Turn on LED_GREEN to indicate power on. */
    user_interface_send_event(EVENT_DEVICE_POWERED_ON, 0);

    /* Register Wi-Fi callback. */
    wifi_register_on_status_changed(_wifi_status_callback);

    /* Connect to Wi-Fi. */
    wifi_err_t connect_err = wifi_connect();
    if (WIFI_OK != connect_err)
    {
        return USER_INTERFACE_INIT_FAIL;
    }

    /* MQTT */
    mqtt_init();

    /* Initialize oscilloscope. */
    oscilloscope_init();

    /* Start timer to measure temperature and humidity periodically. */
    err = _start_temp_humidity_timer();
    if(USER_INTERFACE_OK != err)
    {
        return USER_INTERFACE_INIT_FAIL;
    }
    ESP_LOGI("UI: ", "Temperature and humidity timer satrted.");

    return USER_INTERFACE_OK;
}

user_interface_error_t user_interface_send_event(event_t event, uint32_t value)
{
    event_manager_queue_item item_to_queue = {.event = event, .data.uint32_value = value};
    if(NULL != event_manager_queue)
    {
        xQueueSend(event_manager_queue, (void*)&item_to_queue, 0);
        return USER_INTERFACE_OK;
    }
    return USER_INTERFACE_FAIL;
}

user_interface_error_t user_interface_send_event_from_ISR(event_t event, uint32_t value)
{
    event_manager_queue_item item_to_queue = {.event = event, .data.uint32_value = value};
    if(NULL != event_manager_queue)
    {
        xQueueSendFromISR(event_manager_queue, (void*)&item_to_queue, NULL);
        return USER_INTERFACE_OK;
    }
    return USER_INTERFACE_FAIL;
}

//---------------------------- PRIVATE FUNCTIONS ------------------------------
void _event_manager(void *pvParameters)
{
    event_manager_queue_item recived_data;
    for(;;)
    {
        if(NULL != event_manager_queue)
        {
            if(pdTRUE == xQueueReceive(event_manager_queue, &recived_data, portMAX_DELAY))
            {
                _perform_action(recived_data);
            }
        }
    }
}

static void _perform_action(event_manager_queue_item recived_data)
{
    switch(recived_data.event)
    {
        case EVENT_BUTTON_1_PRESSED:
            set_last_pressed_button(KEY_UP);
            break;
        case EVENT_BUTTON_2_PRESSED:
            set_last_pressed_button(KEY_RIGHT);
            break;
        case EVENT_BUTTON_3_PRESSED:
            set_last_pressed_button(KEY_DOWN);
            break;
        case EVENT_DEVICE_POWERED_ON:
            led_pattern_run(LED_GREEN, LED_PATTERN_KEEP_ON, RUN_INDEFINETLY);
            break;
        case EVENT_TEMPERATURE_DATA_RECIVED:
            /* Check for potential overheating. */
            if(SHOUT_DOWN_TEMP_LIMIT < recived_data.data.uint32_value)
            {
                _ui_screen_change(&ui_Overheat_screen, 0, 0, 0, &ui_Overheat_screen_screen_init);
                vTaskDelete(NULL);
            }
            /* Update temperature labels on all screens. */
            lv_label_set_text_fmt(ui_TemperatureLabel1, "Temperature: %d.%02d °C", (int)(recived_data.data.uint32_value / 100), 
                                                                                   (int)(recived_data.data.uint32_value % 100));
            lv_label_set_text_fmt(ui_TemperatureLabel2, "Temperature: %d.%02d °C", (int)(recived_data.data.uint32_value / 100), 
                                                                                   (int)(recived_data.data.uint32_value % 100));
            lv_label_set_text_fmt(ui_TemperatureLabel3, "Temperature: %d.%02d °C", (int)(recived_data.data.uint32_value / 100), 
                                                                                   (int)(recived_data.data.uint32_value % 100));                                                         
            lv_label_set_text_fmt(ui_TemperatureLabel4, "Temperature: %d.%02d °C", (int)(recived_data.data.uint32_value / 100), 
                                                                                   (int)(recived_data.data.uint32_value % 100));
            _store_temperature_data(recived_data.data.uint32_value);
            break;
        case EVENT_HUMIDITY_DATA_RECIVED:
            /* Update humidity labels on all screens. */
            lv_label_set_text_fmt(ui_HumidityLabel1, "Humidity: %d %%", (int)recived_data.data.uint32_value);
            lv_label_set_text_fmt(ui_HumidityLabel2, "Humidity: %d %%", (int)recived_data.data.uint32_value);
            lv_label_set_text_fmt(ui_HumidityLabel3, "Humidity: %d %%", (int)recived_data.data.uint32_value);
            lv_label_set_text_fmt(ui_HumidityLabel4, "Humidity: %d %%", (int)recived_data.data.uint32_value);
            _store_humidity_data(recived_data.data.uint32_value);
            break;
        case EVENT_WIFI_CONNECTED:
            led_pattern_run(LED_BLUE, LED_PATTERN_SLOWBLINK, RUN_INDEFINETLY);
            break;
        case EVENT_WIFI_DISCONNECTED:
            led_pattern_run(LED_BLUE, LED_PATTERN_NONE, RUN_INDEFINETLY);
            break;
        case EVENT_SHOW_TEMP_HISTORY:
            /* Draw temperature in chart. */
            ui_temp_series = lv_chart_add_series(ui_Temperature_chart, lv_color_hex(0x20F080), LV_CHART_AXIS_PRIMARY_Y);
            lv_chart_set_ext_y_array(ui_Temperature_chart, ui_temp_series, temperature_data);
            /* Draw humidity in chart. */
            ui_humidity_series = lv_chart_add_series(ui_Humidity_chart, lv_color_hex(0x20F080), LV_CHART_AXIS_PRIMARY_Y);
            lv_chart_set_ext_y_array(ui_Humidity_chart, ui_humidity_series, humidity_data);
            break;
        case EVENT_ERROR:
            led_pattern_run(LED_BLUE, LED_PATTERN_KEEP_ON, RUN_INDEFINETLY);
            led_pattern_run(LED_GREEN, LED_PATTERN_KEEP_ON, RUN_INDEFINETLY);
            break;
        default: ESP_LOGI("PERFORM ACTION: ", "Unknown event."); break;
    }
}

static user_interface_error_t _leds_init(void)
{
    led_err_t led_err = led_create(LED_GREEN);
    if (LED_ERR_NONE != led_err)
    {
        return USER_INTERFACE_INIT_FAIL;
    }

    led_err = led_create(LED_BLUE);
    if (LED_ERR_NONE != led_err)
    {
        return USER_INTERFACE_INIT_FAIL;
    }

    return USER_INTERFACE_OK;
}

static user_interface_error_t _buttons_init(void)
{
    button_err_t button_err = button_create(BUTTON_1, _button_1_callback);
    if (BUTTON_ERR_NONE != button_err)
    {
        return USER_INTERFACE_INIT_FAIL;
    }

    button_err = button_create(BUTTON_2, _button_2_callback);
    if (BUTTON_ERR_NONE != button_err)
    {
        return USER_INTERFACE_INIT_FAIL;
    }

    button_err = button_create(BUTTON_3, _button_3_callback);
    if (BUTTON_ERR_NONE != button_err)
    {
        return USER_INTERFACE_INIT_FAIL;
    }

    return USER_INTERFACE_OK;
}

void _button_1_callback(void)
{
    user_interface_send_event_from_ISR(EVENT_BUTTON_1_PRESSED, 0);
}

void _button_2_callback(void)
{
    user_interface_send_event_from_ISR(EVENT_BUTTON_2_PRESSED, 0);
}

void _button_3_callback(void)
{
    user_interface_send_event_from_ISR(EVENT_BUTTON_3_PRESSED, 0);
}

static user_interface_error_t _start_temp_humidity_timer(void)
{
    temp_humididty_tiemr = xTimerCreate("Temperature and humidity timer", pdMS_TO_TICKS(TEMP_HUMIDITY_MEASURE_PERIOD), 
                        pdTRUE, (void *)0, _measure_temp_humidity);
    if (NULL == temp_humididty_tiemr)
    {
        return USER_INTERFACE_INIT_FAIL;
    }

    if(pdPASS != xTimerStart(temp_humididty_tiemr, 0))
    {
        return USER_INTERFACE_INIT_FAIL;
    }

    return USER_INTERFACE_OK;
}

static void _measure_temp_humidity(TimerHandle_t timer)
{
    user_interface_send_event_from_ISR(EVENT_TEMPERATURE_DATA_RECIVED, temp_humidity_get_temperature());
    user_interface_send_event_from_ISR(EVENT_HUMIDITY_DATA_RECIVED, temp_humidity_get_humidity());
}

void _wifi_status_callback(wifi_connection_status_t new_status)
{
    if (new_status == WIFI_CONNECTION_STATUS_CONNECTED)
    {
        user_interface_send_event(EVENT_WIFI_CONNECTED, 0);
    }
    else if (new_status == WIFI_CONNECTION_STATUS_DISCONNECTED)
    {
        user_interface_send_event(EVENT_WIFI_DISCONNECTED, 0);
    }
}

void _store_temperature_data(uint32_t temperature)
{
    temperature_data[temp_hum_index] = temperature;
}

void _store_humidity_data(uint32_t humidity)
{
    humidity_data[temp_hum_index++] = humidity;
    if(temp_hum_index == TEMP_AND_HUMIDITY_HISTORY_LENGTH)
    {
        temp_hum_index = 0;
    }
}

//---------------------------- INTERRUPT HANDLERS -----------------------------
