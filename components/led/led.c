/**
* @file led.c

* @brief

* @par
*
* COPYRIGHT NOTICE: (c) 2022 Byte Lab Grupa d.o.o.
* All rights reserved.
*/

//--------------------------------- INCLUDES ----------------------------------
#include "led.h"
//---------------------------------- MACROS -----------------------------------
#define GPIO_LED_GREEN_PIN   (27U)
#define GPIO_LED_BLUE_PIN    (14U)
#define GPIO_LED_RED_PIN     (26U)

#define DEFAULT_TIMER_PERIOD (1000U)
#define NO_BLOCK_TIME        (0U)
//-------------------------------- DATA TYPES ---------------------------------
typedef struct
{
    uint8_t pin;
    led_gpio_t* p_led_gpio;
    led_state state;
    TimerHandle_t timer;
    led_pattern_t pattern;
    uint32_t timeout_counter;

} _led_config_t;
//---------------------- PRIVATE FUNCTION PROTOTYPES --------------------------
/**
 * @brief Callback which toggles LED state.
 * 
 * @param xTimer Timer handle.
 */
void _timer_callback(TimerHandle_t xTimer);

/**
 * @brief Sets tiemrs perido and starts counting.
 * 
 * @param led LED name.
 * @param led_pattern LED pattern to be run indefinetly.
 * @return led_err_t Status of pattern starting.
 */
led_err_t _timer_start_no_timeout(led_name_t led, led_pattern_t led_pattern);

/**
 * @brief Sets timers period and timeout and starts counting.
 * 
 * @param led LED name.
 * @param led_pattern LED pattern to be run.
 * @param timeout_ms Timeout in milliseconds.
 * @return led_err_t Status of pattern starting.
 */
led_err_t _timer_start_with_timeout(led_name_t led, led_pattern_t led_pattern, uint32_t timeout_ms);

/**
 * @brief Toggles led state.
 * 
 * @param led LED name.
 */
static void _toggle_led(led_name_t led);
//------------------------- STATIC DATA & CONSTANTS ---------------------------
static _led_config_t _led_info[LED_COUNT] = {
    // GPIO LEDS
    { .pin = GPIO_LED_RED_PIN, .state = LED_STATE_OFF, .p_led_gpio = NULL, .timer = NULL, .pattern = LED_PATTERN_NONE, .timeout_counter = 0 },   /* LED_RED */
    { .pin = GPIO_LED_GREEN_PIN, .state = LED_STATE_OFF, .p_led_gpio = NULL, .timer = NULL, .pattern = LED_PATTERN_NONE, .timeout_counter = 0 }, /* LED_GREEN */
    { .pin = GPIO_LED_BLUE_PIN, .state = LED_STATE_OFF, .p_led_gpio = NULL, .timer = NULL, .pattern = LED_PATTERN_NONE, .timeout_counter = 0 },  /* LED_BLUE */
};
//------------------------------- GLOBAL DATA ---------------------------------

//------------------------------ PUBLIC FUNCTIONS -----------------------------

led_err_t led_create(led_name_t led)
{
    if(LED_COUNT <= led)
    {
        return LED_ERR_INVALID_LED;
    }

    (_led_info[(uint8_t)led]).p_led_gpio = led_gpio_create((_led_info[(uint8_t)led]).pin);

    if (NULL == (_led_info[(uint8_t)led]).p_led_gpio)
    {
        return LED_ERR_CREATE;
    }

    (_led_info[(uint8_t)led]).timer = xTimerCreate("LED TIMER", DEFAULT_TIMER_PERIOD, pdTRUE, (void *) led, _timer_callback);

    if (NULL == (_led_info[(uint8_t)led]).timer)
    {
        return LED_ERR_TIMER_CREATE;
    }

    return LED_ERR_NONE;
}

led_err_t led_pattern_run(led_name_t led,
                          led_pattern_t led_pattern,
                          uint32_t timeout_ms)
{
    led_err_t err = LED_ERR_NONE;

    if(LED_COUNT <= led)
    {
        return LED_ERR_INVALID_LED;
    }

    if(LED_PATTERN_COUNT <= led_pattern)
    {
        return LED_ERR_INVALID_PATTERN;
    }

    if(NULL == (_led_info[(uint8_t)led]).p_led_gpio)
    {
        return LED_ERR_NOT_CREATED;
    }

    err = led_pattern_reset(led);
    if(LED_ERR_NONE != err)
    {
        return err;
    }

    if (LED_PATTERN_NONE == led_pattern)
    {
        led_gpio_off((_led_info[(uint8_t)led]).p_led_gpio);

        return LED_ERR_NONE;
    }

    (_led_info[(uint8_t)led]).pattern = led_pattern;

    if (0 == timeout_ms)
    {
        err = _timer_start_no_timeout(led, led_pattern);
    }
    else
    {
        err = _timer_start_with_timeout(led, led_pattern, timeout_ms);
    }

    if (LED_ERR_NONE != err)
    {
        return err;
    }
    return LED_ERR_NONE;
}

led_err_t led_pattern_reset(led_name_t led)
{
    if(LED_COUNT <= led)
    {
        return LED_ERR_INVALID_LED;
    }

    if((_led_info[(uint8_t)led]).p_led_gpio == NULL)
    {
        return LED_ERR_NOT_CREATED;
    }

    if (pdFAIL == xTimerStop((_led_info[(uint8_t)led]).timer, NO_BLOCK_TIME))
    {
        return LED_ERR_TIMER;
    }

    (_led_info[(uint8_t)led]).state = LED_STATE_OFF;
    (_led_info[(uint8_t)led]).pattern = LED_PATTERN_NONE;
    (_led_info[(uint8_t)led]).timeout_counter = 0;

    led_gpio_off((_led_info[(uint8_t)led]).p_led_gpio);
    
    return LED_ERR_NONE;
}
//---------------------------- PRIVATE FUNCTIONS ------------------------------

void _timer_callback(TimerHandle_t xTimer)
{
    led_name_t led = (led_name_t) pvTimerGetTimerID(xTimer);

    /* Timout for LED_PATTERN_KEEP_ON state. */
    if (LED_PATTERN_KEEP_ON == (_led_info[(uint8_t)led]).pattern)
    {
        led_pattern_reset(led);
        return;
    }

    if (0 == (_led_info[(uint8_t)led]).timeout_counter)
    {
        led_pattern_reset(led);
        return;
    }

    (_led_info[(uint8_t)led]).timeout_counter--;

    _toggle_led(led);
}

led_err_t _timer_start_no_timeout(led_name_t led, led_pattern_t led_pattern)
{
    led_gpio_on((_led_info[(uint8_t)led]).p_led_gpio);
    (_led_info[(uint8_t)led]).state = LED_STATE_ON;
    (_led_info[(uint8_t)led]).timeout_counter = NO_TIMEOUT;

    /* If pattern is LED_PATTERN_KEEP_ON no need to start a timer. */
    if (LED_PATTERN_KEEP_ON == led_pattern)
    {
        return LED_ERR_NONE;
    }

    if (LED_PATTERN_FASTBLINK == led_pattern)
    {
        if (pdFAIL == xTimerChangePeriod((_led_info[(uint8_t)led]).timer, pdMS_TO_TICKS(LED_FASTBLINK_PERIOD), NO_BLOCK_TIME))
        {
            return LED_ERR_TIMER;
        }
    }

    if (LED_PATTERN_SLOWBLINK == led_pattern)
    {
        if (pdFAIL == xTimerChangePeriod((_led_info[(uint8_t)led]).timer, pdMS_TO_TICKS(LED_SLOWBLINK_PERIOD), NO_BLOCK_TIME))
        {
            return LED_ERR_TIMER;
        }
    }

    if (pdFAIL == xTimerStart((_led_info[(uint8_t)led]).timer, NO_BLOCK_TIME))
    {
        return LED_ERR_TIMER;
    }

    return LED_ERR_NONE;
}

led_err_t _timer_start_with_timeout(led_name_t led, led_pattern_t led_pattern, uint32_t timeout_ms)
{
    led_gpio_on((_led_info[(uint8_t)led]).p_led_gpio);
    (_led_info[(uint8_t)led]).state = LED_STATE_ON;

    /* With KEEP_ON pattern period is set to timeout. */
    if (LED_PATTERN_KEEP_ON == led_pattern)
    {
        if (pdFAIL == xTimerChangePeriod((_led_info[(uint8_t)led]).timer, pdMS_TO_TICKS(timeout_ms), NO_BLOCK_TIME))
        {
            return LED_ERR_TIMER;
        }
    }
    else if (LED_PATTERN_FASTBLINK == led_pattern)
    {
        if (pdFAIL == xTimerChangePeriod((_led_info[(uint8_t)led]).timer, pdMS_TO_TICKS(LED_FASTBLINK_PERIOD), NO_BLOCK_TIME))
        {
            return LED_ERR_TIMER;
        }

        (_led_info[(uint8_t)led]).timeout_counter = timeout_ms / LED_FASTBLINK_PERIOD - 1;
    }
    else if (LED_PATTERN_SLOWBLINK == led_pattern)
    {
        if (pdFAIL == xTimerChangePeriod((_led_info[(uint8_t)led]).timer, pdMS_TO_TICKS(LED_SLOWBLINK_PERIOD), NO_BLOCK_TIME))
        {
            return LED_ERR_TIMER;
        }
        (_led_info[(uint8_t)led]).timeout_counter = timeout_ms / LED_SLOWBLINK_PERIOD - 1;    
    }

    /* If timeout is less than one period of a pattern. */
    if ((_led_info[(uint8_t)led]).timeout_counter == -1)
    {
        (_led_info[(uint8_t)led]).timeout_counter = 0;
        if (pdFAIL == xTimerChangePeriod((_led_info[(uint8_t)led]).timer, pdMS_TO_TICKS(timeout_ms), NO_BLOCK_TIME))
        {
            return LED_ERR_TIMER;
        }
    }

    if (pdFAIL == xTimerStart((_led_info[(uint8_t)led]).timer, NO_BLOCK_TIME))
    {
        return LED_ERR_TIMER;
    }

    return LED_ERR_NONE;
}

static void _toggle_led(led_name_t led)
{
    (LED_STATE_ON == (_led_info[(uint8_t)led]).state) ? 
    ((_led_info[(uint8_t)led]).state = LED_STATE_OFF) : 
    ((_led_info[(uint8_t)led]).state = LED_STATE_ON);

    if (LED_STATE_ON == (_led_info[(uint8_t)led]).state)
    {
        led_gpio_on((_led_info[(uint8_t)led]).p_led_gpio);
    }
    else
    {
        led_gpio_off((_led_info[(uint8_t)led]).p_led_gpio);
    }
}
//---------------------------- INTERRUPT HANDLERS -----------------------------
