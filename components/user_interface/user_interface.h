/**
* @file user_interface.h
*
* @brief See the source file.
* 
* COPYRIGHT NOTICE: (c) 2024 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------- INCLUDES ----------------------------------
#include "led.h"
#include "button.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
//---------------------------------- MACROS -----------------------------------
#define FAST_BLINK_TIMEOUT_MS        (4000U)
#define TEMP_HUMIDITY_MEASURE_PERIOD (5000U)
#define SHOUT_DOWN_TEMP_LIMIT        (3000U)  // in centi °C (represents 30°C)
//-------------------------------- DATA TYPES ---------------------------------
typedef enum{
    USER_INTERFACE_OK,
    USER_INTERFACE_INIT_FAIL,
    USER_INTERFACE_FAIL,

} user_interface_error_t;

typedef enum
{
    EVENT_BUTTON_1_PRESSED,
    EVENT_BUTTON_2_PRESSED,
    EVENT_BUTTON_3_PRESSED,
    EVENT_BUTTON_4_PRESSED,
    EVENT_DEVICE_POWERED_ON,
    EVENT_TEMPERATURE_DATA_RECIVED,
    EVENT_HUMIDITY_DATA_RECIVED,
    EVENT_WIFI_DISCONNECTED,
    EVENT_WIFI_CONNECTED,
    EVENT_SHOW_TEMP_HISTORY,
    EVENT_ERROR,

} event_t;

typedef struct
{
    event_t event;

    union
    {
        uint32_t uint32_value;
        float    float_value;
    } data;

} event_manager_queue_item;

//---------------------- PUBLIC FUNCTION PROTOTYPES --------------------------
/**
 * @brief Initializes the user interface components, including LEDs, buttons, oscilloscope, adc...
 *
 * - Initializes the LEDs and logs the success status.
 * - Initializes the buttons and logs the success status.
 * - Creates the event manager task, which handles events from the user interface.
 * - Creates a queue for managing events and sends an initial power-on event to indicate the device is powered on.
 *
 * If any initialization step fails, the function returns an appropriate error code.
 *
 * @return user_interface_error_t Returns USER_INTERFACE_OK if all components are initialized successfully,
 *         otherwise returns USER_INTERFACE_INIT_FAIL.
 */
user_interface_error_t user_interface_init(void);

/**
 * @brief Sends an event to the event manager queue from a standard FreeRTOS task.
 *
 * This function enqueues an event and its associated value into the `event_manager_queue` for processing. 
 * It can be used by any FreeRTOS task to signal various events, such as button presses or sensor updates.
 *
 * @param[in] event The event to be sent, defined in the `event_t` enum.
 * @param[in] value The 32-bit value associated with the event, which can be used to pass additional information.
 *
 * @return user_interface_error_t Returns `USER_INTERFACE_OK` if the event was successfully sent to the queue,
 *         or `USER_INTERFACE_FAIL` if the queue is unavailable or the event could not be sent.
 */
user_interface_error_t user_interface_send_event(event_t event, uint32_t value);

/**
 * @brief Sends an event to the event manager queue from an ISR context.
 *
 * This function enqueues an event and its associated value into the `event_manager_queue` from an Interrupt Service 
 * Routine (ISR). It is designed to be safe to call from within an ISR, using the `xQueueSendFromISR` API to avoid 
 * context switching issues.
 *
 * @param[in] event The event to be sent, defined in the `event_t` enum.
 * @param[in] value The 32-bit value associated with the event, which can be used to pass additional information.
 *
 * @return user_interface_error_t Returns `USER_INTERFACE_OK` if the event was successfully sent to the queue,
 *         or `USER_INTERFACE_FAIL` if the queue is unavailable or the event could not be sent.
 */
user_interface_error_t user_interface_send_event_from_ISR(event_t event, uint32_t value);


#ifdef __cplusplus
}
#endif

#endif // __USER_INTERFACE_H__
