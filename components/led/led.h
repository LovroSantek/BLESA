/**
* @file led.h

* @brief See the source file.

* @par
*
* COPYRIGHT NOTICE: (c) 2022 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------- INCLUDES ----------------------------------
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "led_gpio.h"
#include "esp_log.h"
//---------------------------------- MACROS -----------------------------------
#define NO_TIMEOUT (UINT_MAX)
#define LED_FASTBLINK_PERIOD  (100U)
#define LED_SLOWBLINK_PERIOD  (2000U)
//-------------------------------- DATA TYPES ---------------------------------

typedef enum
{
    LED_PATTERN_NONE,      /* LED is off */

    LED_PATTERN_KEEP_ON,   /* LED is on   */
    LED_PATTERN_SLOWBLINK, /* 1000 ms on, 1000 ms off */
    LED_PATTERN_FASTBLINK, /* 100 ms on, 100 ms off */

    LED_PATTERN_COUNT
} led_pattern_t;

typedef enum
{
    LED_RED,
    LED_GREEN,
    LED_BLUE,

    LED_COUNT
} led_name_t;

typedef enum
{
    LED_ERR_NONE = 0,

    LED_ERR_CREATE = -1,
    LED_ERR_INVALID_PATTERN = -2,
    LED_ERR_INVALID_LED = -3,
    LED_ERR_NOT_CREATED = -4,
    LED_ERR_TIMER_CREATE = -5,
    LED_ERR_TIMER = -6,

} led_err_t;

typedef enum
{
    LED_STATE_OFF,
    LED_STATE_ON,

    LED_STATE_COUNT
} led_state;
//---------------------- PUBLIC FUNCTION PROTOTYPES --------------------------
/**
 * @brief Create LED driver and configure all needed functionalities.
 * 
 * @param [in] led - Name of the LED to be created.
 * 
 * @return Status of creation.
 */
led_err_t led_create(led_name_t led);

/**
 * @brief Set led pattern.
 *        Don't call this function from ISR.
 * 
 * @param [in] led - LED name.
 * @param [in] led_pattern - LED pattern to be run.
 * @param [in] timeout_ms - timeout in milliseconds (if 0 run infinitely).
 * 
 * @return Status of pattern starting.
 */
led_err_t led_pattern_run(led_name_t led, led_pattern_t led_pattern, uint32_t timeout_ms);

/**
 * @brief Reset LED pattern.
 * 
 * @param [in] led - LED name.
 * .
 * @return Status of pattern reseting.
 */
led_err_t led_pattern_reset(led_name_t led);


#ifdef __cplusplus
}
#endif

#endif // __LED_H__
